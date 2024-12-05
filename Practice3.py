from flask import Flask, request, jsonify
import socket
import json
import random
import string

app = Flask(__name__)

# Загрузка конфигурации
with open('config.json', 'r') as f:
    config = json.load(f)

DATABASE_IP = config['database_ip']
DATABASE_PORT = config['database_port']

# Генерация случайного ключа пользователя
def generate_user_key():
    return ''.join(random.choices(string.ascii_letters + string.digits, k=32))

# Отправка SQL-запроса на сервер СУБД
def send_sql_query(query):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((DATABASE_IP, DATABASE_PORT))
        s.sendall(query.encode())
        s.sendall(b"exit\n")  # Отправка команды exit для завершения сессии

# Загрузка лотов и пар из конфигурации
def load_lots_and_pairs():
    lots = config['lots']
    lot_id = 1
    for lot in lots:
        sql = f"INSERT INTO lot VALUES ('{lot_id}','{lot}')\n"
        send_sql_query(sql)
        lot_id += 1
    # Формирование пар
    pair_id = 1
    for i in range(len(lots)):
        for j in range(i + 1, len(lots)):
            sql = f"INSERT INTO pair VALUES ('{pair_id}','{i + 1}','{j + 1}')\n"
            send_sql_query(sql)
            pair_id += 1

# Проверка ключа пользователя
def check_user_key(user_key):
    sql = f"SELECT user_id FROM user WHERE key = '{user_key}'\n"
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((DATABASE_IP, DATABASE_PORT))
        s.sendall(sql.encode())
        response = s.recv(1024).decode()
        s.sendall(b"exit\n")  # Отправка команды exit для завершения сессии
        if response:
            return response.strip()
    return None

# Создание пользователя
@app.route('/create_user', methods=['POST'])
def create_user():
    data = request.json
    username = data.get('username')
    key = generate_user_key()

    if not username:
        return jsonify({"error": "Username is required"}), 400

    # Отправка запроса на сервер СУБД
    sql = f"INSERT INTO user VALUES('{username}', '{key}')\n"
    send_sql_query(sql)

    # Пополнение баланса пользователя
    user_id = check_user_key(key)
    if user_id:
        for lot in config['lots']:
            sql = f"INSERT INTO user_lot VALUES('{user_id}', '{lot['lot_id']}', '{lot['quantity']}')\n"
            send_sql_query(sql)
        return jsonify({"message": "User created successfully", "key": key}), 201
    else:
        return jsonify({"error": "Failed to create user"}), 500

# Создание ордера
@app.route('/create_order', methods=['POST'])
def create_order():
    user_key = request.headers.get('X-USER-KEY')
    if not user_key:
        return jsonify({"error": "User key is required"}), 400

    user_id = check_user_key(user_key)
    if not user_id:
        return jsonify({"error": "Invalid user key"}), 401

    data = request.json
    pair_id = data.get('pair_id')
    quantity = data.get('quantity')
    price = data.get('price')
    order_type = data.get('type')
    closed = data.get('closed', False)

    if not pair_id or not quantity or not price or not order_type:
        return jsonify({"error": "Missing required fields"}), 400

    # Отправка запроса на сервер СУБД
    sql = f"INSERT INTO order VALUES('{user_id}', '{pair_id}', '{quantity}', '{price}', '{order_type}', '{closed}')\n"
    send_sql_query(sql)

    return jsonify({"message": "Order created successfully"}), 201

# Удаление ордера
@app.route('/delete_order', methods=['DELETE'])
def delete_order():
    user_key = request.headers.get('X-USER-KEY')
    if not user_key:
        return jsonify({"error": "User key is required"}), 400

    user_id = check_user_key(user_key)
    if not user_id:
        return jsonify({"error": "Invalid user key"}), 401

    data = request.json
    order_id = data.get('order_id')

    if not order_id:
        return jsonify({"error": "Order ID is required"}), 400

    # Отправка запроса на сервер СУБД
    sql = f"DELETE FROM order WHERE order_id = '{order_id}' AND user_id = '{user_id}'\n"
    send_sql_query(sql)

    return jsonify({"message": "Order deleted successfully"}), 200

# Получение списка ордеров
@app.route('/get_orders', methods=['GET'])
def get_orders():
    user_key = request.headers.get('X-USER-KEY')
    if not user_key:
        return jsonify({"error": "User key is required"}), 400

    user_id = check_user_key(user_key)
    if not user_id:
        return jsonify({"error": "Invalid user key"}), 401

    # Отправка запроса на сервер СУБД
    sql = f"SELECT * FROM order WHERE user_id = '{user_id}'\n"
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((DATABASE_IP, DATABASE_PORT))
        s.sendall(sql.encode())
        response = s.recv(1024).decode()
        s.sendall(b"exit\n")  # Отправка команды exit для завершения сессии

    if response:
        return jsonify(response), 200
    else:
        return jsonify({"error": "Failed to get orders"}), 500

# Получение информации о лотах
@app.route('/get_lots', methods=['GET'])
def get_lots():
    # Отправка запроса на сервер СУБД
    sql = "SELECT * FROM lot\n"
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((DATABASE_IP, DATABASE_PORT))
        s.sendall(sql.encode())
        response = s.recv(1024).decode()
        s.sendall(b"exit\n")  # Отправка команды exit для завершения сессии

    if response:
        return jsonify(response), 200
    else:
        return jsonify({"error": "Failed to get lots"}), 500

# Получение информации о парах
@app.route('/get_pairs', methods=['GET'])
def get_pairs():
    # Отправка запроса на сервер СУБД
    sql = "SELECT * FROM pair\n"
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((DATABASE_IP, DATABASE_PORT))
        s.sendall(sql.encode())
        response = s.recv(1024).decode()
        s.sendall(b"exit\n")  # Отправка команды exit для завершения сессии

    if response:
        return jsonify(response), 200
    else:
        return jsonify({"error": "Failed to get pairs"}), 500

# Получение информации об активах пользователя
@app.route('/get_user_assets', methods=['GET'])
def get_user_assets():
    user_key = request.headers.get('X-USER-KEY')
    if not user_key:
        return jsonify({"error": "User key is required"}), 400

    user_id = check_user_key(user_key)
    if not user_id:
        return jsonify({"error": "Invalid user key"}), 401

    # Отправка запроса на сервер СУБД
    sql = f"SELECT * FROM user_lot WHERE user_id = '{user_id}'\n"
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((DATABASE_IP, DATABASE_PORT))
        s.sendall(sql.encode())
        response = s.recv(1024).decode()
        s.sendall(b"exit\n")  # Отправка команды exit для завершения сессии

    if response:
        return jsonify(response), 200
    else:
        return jsonify({"error": "Failed to get user assets"}), 500

if __name__ == '__main__':
    load_lots_and_pairs()
    app.run(host='0.0.0.0', port=5000)