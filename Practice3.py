import json
import socket
from flask import Flask, request, jsonify
from itertools import combinations
import uuid  # Для генерации ключа пользователя
import time  # Для генерации времени закрытия ордера
import logging

app = Flask(__name__)

# Конфигурация сервера
DB_SERVER_HOST = 'localhost'
DB_SERVER_PORT = 7432

# Логирование
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

# Чтение конфигурации из config.json
def load_config():
    with open('config.json', 'r') as f:
        config = json.load(f)
    return config

def send_sql_query(query):
    logging.info(f"Sending query: {query}")
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((DB_SERVER_HOST, DB_SERVER_PORT))
            s.sendall(query.encode())

            if query.strip().upper().startswith("SELECT"):
                response = s.recv(1024).decode()
                logging.info(f"Received response: {response}")
                return response.strip()
            else:
                logging.info("No response expected for this query.")
                return None
    except Exception as e:
        logging.error(f"Error sending SQL query: {e}")
        return None

def parse_db_response(response):
    if not response:
        return []
    lines = response.strip().split('\n')
    if len(lines) < 2:
        return []

    headers = lines[0].split()
    result = []
    for line in lines[1:]:
        values = line.split()
        if len(values) == len(headers):
            row = {headers[i]: values[i] for i in range(len(headers))}
            result.append(row)
    return result

# Инициализация базы данных на основе config.json
def initialize_database(config):
    lots = config['lots']
    database_ip = config['database_ip']
    database_port = config['database_port']

    global DB_SERVER_HOST, DB_SERVER_PORT
    DB_SERVER_HOST = database_ip
    DB_SERVER_PORT = database_port

    # Добавляем активы в таблицу lot
    for lot in lots:
        send_sql_query(f"INSERT INTO lot VALUES ('{lot}')")

    # Формируем валютные пары (каждый с каждым) и добавляем их в таблицу pair
    lot_pairs = list(combinations(lots, 2))
    for pair in lot_pairs:
        send_sql_query(f"INSERT INTO pair VALUES ('{pair[0]}','{pair[1]}')")

# Функция для выполнения ордеров
def execute_orders(user_id, pair_id, quantity, price, order_type):
    logging.info(f"Executing orders for user_id={user_id}, pair_id={pair_id}, quantity={quantity}, price={price}, type={order_type}")

    if order_type == "buy":
        opposite_type = "sell"
    else:
        opposite_type = "buy"

    response = send_sql_query(f"SELECT order.order_id,order.user_id,order.pair_id,order.quantity,order.price,order.type,order.closed FROM order WHERE order.pair_id = '{pair_id}' AND order.type = '{opposite_type}' AND order.closed = ''")
    if not response:
        return False

    try:
        orders = parse_db_response(response)
    except (IndexError, KeyError) as e:
        logging.error(f"Error parsing response from database: {e}")
        return False

    # Сортируем ордера на стороне клиента
    if order_type == "buy":
        orders = sorted(orders, key=lambda x: float(x['price']))
    else:
        orders = sorted(orders, key=lambda x: float(x['price']), reverse=True)

    for order in orders:
        if order_type == "buy" and float(order['price']) > price:
            continue
        if order_type == "sell" and float(order['price']) < price:
            continue

        executed_quantity = min(float(order['quantity']), quantity)
        quantity -= executed_quantity

        # Обновляем балансы пользователей
        update_user_balance(user_id, pair_id, executed_quantity, price, order_type)
        update_user_balance(order['user_id'], pair_id, executed_quantity, price, opposite_type)

        # Закрываем ордер
        close_time = str(int(time.time()))
        send_sql_query(f"INSERT INTO order VALUES ('{order['order_id']}','{order['user_id']}','{pair_id}','{executed_quantity}','{order['price']}','{order['type']}','{close_time}')")

        if quantity <= 0:
            break

    return quantity == 0

# Функция для обновления баланса пользователя
def update_user_balance(user_id, pair_id, quantity, price, order_type):
    pair_response = send_sql_query(f"SELECT pair.first_lot_id,pair.second_lot_id FROM pair WHERE pair.pair_id = '{pair_id}'")
    if not pair_response:
        return

    try:
        pair_data = parse_db_response(pair_response)[0]
        first_lot_id = pair_data['first_lot_id']
        second_lot_id = pair_data['second_lot_id']
    except (IndexError, KeyError) as e:
        logging.error(f"Error parsing response from database: {e}")
        return

    if order_type == "buy":
        # Удаляем старую запись о балансе
        send_sql_query(f"DELETE FROM user_lot WHERE user_lot.user_id = '{user_id}' AND user_lot.lot_id = '{first_lot_id}'")
        send_sql_query(f"DELETE FROM user_lot WHERE user_lot.user_id = '{user_id}' AND user_lot.lot_id = '{second_lot_id}'")

        # Добавляем новую запись о балансе
        send_sql_query(f"INSERT INTO user_lot VALUES ('{user_id}','{first_lot_id}','{quantity}')")
        send_sql_query(f"INSERT INTO user_lot VALUES ('{user_id}','{second_lot_id}','{quantity * price}')")
    else:
        # Удаляем старую запись о балансе
        send_sql_query(f"DELETE FROM user_lot WHERE user_lot.user_id = '{user_id}' AND user_lot.lot_id = '{first_lot_id}'")
        send_sql_query(f"DELETE FROM user_lot WHERE user_lot.user_id = '{user_id}' AND user_lot.lot_id = '{second_lot_id}'")

        # Добавляем новую запись о балансе
        send_sql_query(f"INSERT INTO user_lot VALUES ('{user_id}','{first_lot_id}','{quantity}')")
        send_sql_query(f"INSERT INTO user_lot VALUES ('{user_id}','{second_lot_id}','{quantity * price}')")

# Функция для получения id валюты по ее имени
def get_lot_id_by_name(lot_name):
    lot_response = send_sql_query(f"SELECT lot.lot_id FROM lot WHERE lot.name = '{lot_name}'")
    if not lot_response:
        logging.error(f"Lot '{lot_name}' not found")
        return None

    try:
        lot_data = parse_db_response(lot_response)[0]
        return lot_data['lot_id']
    except (IndexError, KeyError) as e:
        logging.error(f"Error parsing lot response for '{lot_name}': {e}")
        return None
    
# Функция для проверки баланса пользователя
def check_user_balance(user_id, pair_id, quantity, price, order_type):
    logging.info(f"Checking balance for user_id={user_id}, pair_id={pair_id}, quantity={quantity}, price={price}, type={order_type}")
    
    # Получаем идентификаторы лотов для пары
    pair_response = send_sql_query(f"SELECT pair.first_lot_id,pair.second_lot_id FROM pair WHERE pair.pair_id = '{pair_id}'")
    if not pair_response:
        logging.error("Pair response is empty")
        return False

    try:
        pair_data = parse_db_response(pair_response)[0]
        first_lot_name = pair_data['first_lot_id']  # Имя первого лота (например, "RUB")
        second_lot_name = pair_data['second_lot_id']  # Имя второго лота (например, "BTC")
    except (IndexError, KeyError) as e:
        logging.error(f"Error parsing pair response: {e}")
        return False

    # Получаем числовые идентификаторы лотов по их именам
    first_lot_id = get_lot_id_by_name(first_lot_name)
    second_lot_id = get_lot_id_by_name(second_lot_name)

    if not first_lot_id or not second_lot_id:
        logging.error("Failed to get lot IDs")
        return False

    # Проверяем баланс для покупки или продажи
    if order_type == "buy":
        # Для покупки проверяем баланс второго лота (например, BTC для пары RUB/BTC)
        balance_response = send_sql_query(f"SELECT user_lot.quantity FROM user_lot WHERE user_lot.user_id = '{user_id}' AND user_lot.lot_id = '{second_lot_id}'")
        if not balance_response:
            logging.error("Balance response is empty for buy order")
            return False
        balance_data = parse_db_response(balance_response)
        if not balance_data or float(balance_data[0]['quantity']) < quantity * price:
            logging.error(f"Insufficient balance for buy order: {balance_data}")
            return False
    else:
        # Для продажи проверяем баланс первого лота (например, RUB для пары RUB/BTC)
        balance_response = send_sql_query(f"SELECT user_lot.quantity FROM user_lot WHERE user_lot.user_id = '{user_id}' AND user_lot.lot_id = '{first_lot_id}'")
        if not balance_response:
            logging.error("Balance response is empty for sell order")
            return False
        balance_data = parse_db_response(balance_response)
        if not balance_data or float(balance_data[0]['quantity']) < quantity:
            logging.error(f"Insufficient balance for sell order: {balance_data}")
            return False

    return True

# Маршрут для создания пользователя
@app.route('/user', methods=['POST'])
def create_user():
    data = request.get_json()
    username = data.get('username')
    
    if not username:
        return jsonify({'error': 'Username is required'}), 400
    
    user_key = str(uuid.uuid4())
    
    send_sql_query(f"INSERT INTO user VALUES ('{username}','{user_key}')")
    
    user_id_response = send_sql_query(f"SELECT user.user_id FROM user WHERE user.username = '{username}'")
    if not user_id_response:
        return jsonify({'error': 'Failed to retrieve user ID'}), 500
    
    try:
        user_id_data = parse_db_response(user_id_response)
        user_id = user_id_data[0]['user_id']
    except (IndexError, KeyError) as e:
        logging.error(f"Error parsing response from database: {e}")
        return jsonify({'error': 'Failed to parse response from database'}), 500
    
    lots_response = send_sql_query("SELECT lot.lot_id FROM lot")
    if lots_response is None:
        return jsonify({'error': 'Failed to retrieve lots'}), 500
    
    try:
        lots_data = parse_db_response(lots_response)
        for lot in lots_data:
            lot_id = lot['lot_id']
            send_sql_query(f"INSERT INTO user_lot VALUES ('{user_id}','{lot_id}','1000')")
    except (IndexError, KeyError) as e:
        logging.error(f"Error parsing response from database: {e}")
        return jsonify({'error': 'Failed to parse response from database'}), 500
    
    return jsonify({'key': user_key}), 200

# Маршрут для создания ордера
@app.route('/order', methods=['POST'])
def create_order():
    user_key = request.headers.get('X-USER-KEY')
    if not user_key:
        return jsonify({'error': 'X-USER-KEY is required'}), 400
    
    user_id_response = send_sql_query(f"SELECT user.user_id FROM user WHERE user.key = '{user_key}'")
    if not user_id_response:
        return jsonify({'error': 'Invalid user key'}), 401
    
    try:
        user_id_data = parse_db_response(user_id_response)
        user_id = user_id_data[0]['user_id']
    except (IndexError, KeyError) as e:
        logging.error(f"Error parsing response from database: {e}")
        return jsonify({'error': 'Failed to parse response from database'}), 500
    
    data = request.get_json()
    pair_id = data.get('pair_id')
    quantity = data.get('quantity')
    price = data.get('price')
    order_type = data.get('type')
    
    if not all([pair_id, quantity, price, order_type]):
        return jsonify({'error': 'All fields are required'}), 400
    
    if quantity <= 0 or price <= 0:
        return jsonify({'error': 'Quantity and price must be positive'}), 400
    
    if not check_user_balance(user_id, pair_id, quantity, price, order_type):
        return jsonify({'error': 'Insufficient balance'}), 400
    
    remaining_quantity = execute_orders(user_id, pair_id, quantity, price, order_type)
    
    if remaining_quantity > 0:
        send_sql_query(f"INSERT INTO order VALUES ('{user_id}','{pair_id}','{remaining_quantity}','{price}','{order_type}','')")
    
    return jsonify({'message': 'Order created successfully'}), 200

# Маршрут для просмотра ордеров
@app.route('/orders', methods=['GET'])
def view_orders():
    response = send_sql_query("SELECT order.order_id,order.user_id,order.pair_id,order.quantity,order.price,order.type,order.closed FROM order")
    try:
        orders = parse_db_response(response)
    except json.JSONDecodeError as e:
        logging.error(f"Error parsing response from database: {e}")
        return jsonify({'error': 'Failed to parse response from database'}), 500
    
    return jsonify(orders), 200

# Маршрут для удаления ордера
@app.route('/order', methods=['DELETE'])
def delete_order():
    user_key = request.headers.get('X-USER-KEY')
    data = request.get_json()
    order_id = data.get('order_id')
    
    if not user_key or not order_id:
        return jsonify({'error': 'X-USER-KEY and order_id are required'}), 400
    
    user_id_response = send_sql_query(f"SELECT user.user_id FROM user WHERE user.key = '{user_key}'")
    if not user_id_response:
        return jsonify({'error': 'Invalid user key'}), 401
    
    try:
        user_id_data = parse_db_response(user_id_response)
        user_id = user_id_data[0]['user_id']
    except (IndexError, KeyError) as e:
        logging.error(f"Error parsing response from database: {e}")
        return jsonify({'error': 'Failed to parse response from database'}), 500
    
    send_sql_query(f"DELETE FROM order WHERE order.order_id = '{order_id}' AND order.user_id = '{user_id}'")
    
    return jsonify({'message': 'Order deleted successfully'}), 200

# Маршрут для получения информации о лотах
@app.route('/lots', methods=['GET'])
def get_lots():
    response = send_sql_query("SELECT lot.lot_id,lot.name FROM lot")
    try:
        lots = parse_db_response(response)
    except json.JSONDecodeError as e:
        logging.error(f"Error parsing response from database: {e}")
        return jsonify({'error': 'Failed to parse response from database'}), 500
    
    return jsonify(lots), 200

# Маршрут для получения информации о парах
@app.route('/pairs', methods=['GET'])
def get_pairs():
    response = send_sql_query("SELECT pair.pair_id,pair.first_lot_id,pair.second_lot_id FROM pair")
    try:
        pairs = parse_db_response(response)
    except json.JSONDecodeError as e:
        logging.error(f"Error parsing response from database: {e}")
        return jsonify({'error': 'Failed to parse response from database'}), 500
    
    return jsonify(pairs), 200

# Маршрут для получения информации об активах пользователя
@app.route('/balance', methods=['GET'])
def get_balance():
    user_key = request.headers.get('X-USER-KEY')
    if not user_key:
        return jsonify({'error': 'X-USER-KEY is required'}), 400
    
    user_id_response = send_sql_query(f"SELECT user.user_id FROM user WHERE user.key = '{user_key}'")
    if not user_id_response:
        return jsonify({'error': 'Invalid user key'}), 401
    
    try:
        user_id_data = parse_db_response(user_id_response)
        user_id = user_id_data[0]['user_id']
    except (IndexError, KeyError) as e:
        logging.error(f"Error parsing response from database: {e}")
        return jsonify({'error': 'Failed to parse response from database'}), 500
    
    response = send_sql_query(f"SELECT user_lot.lot_id,user_lot.quantity FROM user_lot WHERE user_lot.user_id = '{user_id}'")
    try:
        balance = parse_db_response(response)
    except json.JSONDecodeError as e:
        logging.error(f"Error parsing response from database: {e}")
        return jsonify({'error': 'Failed to parse response from database'}), 500
    
    return jsonify(balance), 200

if __name__ == '__main__':
    config = load_config()
    initialize_database(config)
    app.run(host='0.0.0.0', port=8080)