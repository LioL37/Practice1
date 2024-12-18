import json
import socket
from flask import Flask, request, jsonify
from itertools import combinations
import uuid  # Для генерации ключа пользователя
import time  # Для генерации времени закрытия ордера

app = Flask(__name__)

# Конфигурация сервера
DB_SERVER_HOST = 'localhost'
DB_SERVER_PORT = 7432

# Чтение конфигурации из config.json
def load_config():
    with open('config.json', 'r') as f:
        config = json.load(f)
    return config

def send_sql_query(query):
    print(f"Sending query: {query}")  # Логирование запроса
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((DB_SERVER_HOST, DB_SERVER_PORT))
            s.sendall(query.encode())

            # Ожидаем ответ только для SELECT
            if query.strip().upper().startswith("SELECT"):
                response = s.recv(1024).decode()
                print(f"Received response: {response}")  # Логирование ответа
                return response.strip()  # Возвращаем текстовый ответ, удаляя лишние пробелы и символы новой строки
            else:
                print("No response expected for this query.")
                return None  # Для других команд возвращаем None
    except Exception as e:
        print(f"Error sending SQL query: {e}")
        return None

def parse_db_response(response):
    lines = response.strip().split('\n')
    if len(lines) < 2:  # Если меньше двух строк, то ответ пустой или некорректный
        return []

    # Первая строка - заголовки
    headers = lines[0].split()

    # Остальные строки - данные
    result = []
    for line in lines[1:]:
        values = line.split()
        if len(values) == len(headers):  # Проверяем, что количество значений совпадает с количеством заголовков
            row = {headers[i]: values[i] for i in range(len(headers))}
            result.append(row)
    return result

# Инициализация базы данных на основе config.json
def initialize_database(config):
    lots = config['lots']
    database_ip = config['database_ip']
    database_port = config['database_port']

    # Устанавливаем соединение с сервером базы данных
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

# Маршрут для создания пользователя
@app.route('/create_user', methods=['POST'])
def create_user():
    data = request.get_json()
    username = data.get('username')
    
    if not username:
        return jsonify({'error': 'Username is required'}), 400
    
    # Генерируем случайный ключ пользователя
    user_key = str(uuid.uuid4())
    
    # Отправляем SQL-запрос на сервер для создания пользователя
    send_sql_query(f"INSERT INTO user VALUES ('{username}','{user_key}')")
    
    # Получаем user_id только что созданного пользователя
    user_id_response = send_sql_query(f"SELECT user.user_pk,user.username FROM user WHERE user.username = '{username}'")
    if not user_id_response:
        return jsonify({'error': 'Failed to retrieve user ID'}), 500
    
    try:
        user_id_data = parse_db_response(user_id_response)
        user_id = user_id_data[0]['user_pk']
    except (IndexError, KeyError) as e:
        print(f"Error parsing response from database: {e}")
        return jsonify({'error': 'Failed to parse response from database'}), 500
    
    # Добавляем баланс для пользователя
    lots_response = send_sql_query("SELECT lot.lot_pk FROM lot")
    if lots_response is None:
        return jsonify({'error': 'Failed to retrieve lots'}), 500
    
    try:
        lots_data = parse_db_response(lots_response)
        for lot in lots_data:
            lot_id = lot['lot_pk']
            send_sql_query(f"INSERT INTO user_lot VALUES ('{user_id}','{lot_id}','1000')")
    except (IndexError, KeyError) as e:
        print(f"Error parsing response from database: {e}")
        return jsonify({'error': 'Failed to parse response from database'}), 500
    
    return jsonify({'key': user_key}), 200  # Возвращаем ключ пользователя

# Маршрут для создания ордера
@app.route('/order', methods=['POST'])
def create_order():
    user_key = request.headers.get('X-USER-KEY')
    if not user_key:
        return jsonify({'error': 'X-USER-KEY is required'}), 400
    
    # Проверяем, что ключ пользователя действителен
    user_id_response = send_sql_query(f"SELECT user.user_pk FROM user WHERE user.key = '{user_key}'")
    if not user_id_response:
        return jsonify({'error': 'Invalid user key'}), 401
    
    try:
        user_id_data = parse_db_response(user_id_response)
        user_id = user_id_data[0]['user_pk']
    except (IndexError, KeyError) as e:
        print(f"Error parsing response from database: {e}")
        return jsonify({'error': 'Failed to parse response from database'}), 500
    
    data = request.get_json()
    pair_id = data.get('pair_id')
    quantity = data.get('quantity')
    price = data.get('price')
    order_type = data.get('type')
    
    if not all([pair_id, quantity, price, order_type]):
        return jsonify({'error': 'All fields are required'}), 400
    
    # Отправляем SQL-запрос на сервер
    send_sql_query(f"INSERT INTO order VALUES ('{user_id}','{pair_id}','{quantity}','{price}','{order_type}')")
    
    return jsonify({'message': 'Order created successfully'}), 200

# Маршрут для просмотра ордеров
@app.route('/orders', methods=['GET'])
def view_orders():
    response = send_sql_query("SELECT order.order_pk,order.user_id,order.pair_id,order.quantity,order.price,order.type,order.closed FROM order")
    try:
        orders = parse_db_response(response)
    except json.JSONDecodeError as e:
        print(f"Error parsing response from database: {e}")
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
    
    # Проверяем, что ордер принадлежит пользователю
    user_id_response = send_sql_query(f"SELECT user.user_pk FROM user WHERE user.key = '{user_key}'")
    if not user_id_response:
        return jsonify({'error': 'Invalid user key'}), 401
    
    try:
        user_id_data = parse_db_response(user_id_response)
        user_id = user_id_data[0]['user_pk']
    except (IndexError, KeyError) as e:
        print(f"Error parsing response from database: {e}")
        return jsonify({'error': 'Failed to parse response from database'}), 500
    
    # Удаляем ордер
    send_sql_query(f"DELETE FROM order WHERE order.order_pk = '{order_id}' AND order.user_id = '{user_id}'")
    
    return jsonify({'message': 'Order deleted successfully'}), 200

# Маршрут для получения информации о лотах
@app.route('/lots', methods=['GET'])
def get_lots():
    response = send_sql_query("SELECT lot.lot_pk,lot.name FROM lot")
    try:
        lots = parse_db_response(response)
    except json.JSONDecodeError as e:
        print(f"Error parsing response from database: {e}")
        return jsonify({'error': 'Failed to parse response from database'}), 500
    
    return jsonify(lots), 200

# Маршрут для получения информации о парах
@app.route('/pairs', methods=['GET'])
def get_pairs():
    response = send_sql_query("SELECT pair.pair_pk,pair.first_lot_id,pair.second_lot_id FROM pair")
    try:
        pairs = parse_db_response(response)
    except json.JSONDecodeError as e:
        print(f"Error parsing response from database: {e}")
        return jsonify({'error': 'Failed to parse response from database'}), 500
    
    return jsonify(pairs), 200

if __name__ == '__main__':
    # Загружаем конфигурацию и инициализируем базу данных
    config = load_config()
    initialize_database(config)
    
    # Запускаем HTTP-сервер
    app.run(host='0.0.0.0', port=8080)