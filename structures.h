using namespace std;

template<typename T>
struct SinglyLinkedList {
    struct FLNode {
        FLNode* next;
        T value;
        FLNode(const T& val) : value(val), next(nullptr) {}
    };

    FLNode* head;
    SinglyLinkedList() : head(nullptr) {}

    // Добавление элемента в голову
    void addToHead(const T& value) {
        FLNode* newNode = new FLNode(value);
        newNode->next = head;
        head = newNode;
    }

    // Добавление элемента в хвост (сложность O(n))
    void addToTail(const T& value) {
        FLNode* newNode = new FLNode(value);
        if (!head) {
            head = newNode;
        } else {
            FLNode* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    // Удаление элемента с головы
    void removeFromHead() {
        if (!head) {
            throw runtime_error("List is empty");
        }
        FLNode* temp = head;
        head = head->next;
        delete temp;
    }

    // Удаление элемента с хвоста (сложность O(n))
    void removeFromTail() {
        if (!head) {
            throw runtime_error("List is empty");
        }
        if (!head->next) {
            delete head;
            head = nullptr;
        } else {
            FLNode* temp = head;
            while (temp->next->next) {
                temp = temp->next;
            }
            delete temp->next;
            temp->next = nullptr;
        }
    }

    // Удаление элемента по значению
    void removeByValue(const T& value) {
        FLNode* current = head;
        FLNode* previous = nullptr;
        while (current) {
            if (current->value == value) {
                if (previous) {
                    previous->next = current->next;
                } else {
                    head = current->next;
                }
                delete current;
                return;
            }
            previous = current;
            current = current->next;
        }
        throw runtime_error("Value not found");
    }

    // Поиск элемента по значению
    bool search(const T& value) const {
        FLNode* current = head;
        while (current) {
            if (current->value == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Чтение списка
    void print() const {
        FLNode* current = head;
        while (current) {
            cout << current->value << " ";
            current = current->next;
        }
        cout << endl;
    }

    // Установка элемента по индексу
    void set(int index, const T& value) {
        FLNode* current = head;
        for (int i = 0; i < index && current; ++i) {
            current = current->next;
        }
        if (current) {
            current->value = value;
        }
    }

    // Получение размера списка
    int size() const {
        int count = 0;
        FLNode* current = head;
        while (current) {
            ++count;
            current = current->next;
        }
        return count;
    }
};
enum ExpressionType {
    Comparison,
    Binary
};

struct Expression {
    ExpressionType type;
    string columnName;
    string operatorType;
    string value;
    string op;
    Expression* left;
    Expression* right;

    Expression(ExpressionType type, const string& column, const string& op, const string& val)
        : type(type), columnName(column), operatorType(op), value(val), left(nullptr), right(nullptr) {}

    Expression(ExpressionType type, const string& op, Expression* left, Expression* right)
        : type(type), op(op), left(left), right(right) {}

    bool evaluate(const SinglyLinkedList<string>& row, const SinglyLinkedList<string>& headers) {
        if (type == ExpressionType::Comparison) {
            // Ищем индекс колонки в заголовках
            int index = 0;
            SinglyLinkedList<string>::FLNode* headerNode = headers.head;
            while (headerNode) {
                if (headerNode->value == columnName) {
                    break;
                }
                headerNode = headerNode->next;
                index++;
            }

            // Проверяем значение строки по индексу колонки
            SinglyLinkedList<string>::FLNode* rowNode = row.head;
            for (int i = 0; i < index && rowNode; ++i) {
                rowNode = rowNode->next;
            }
            if (rowNode) {
                if (operatorType == "=") {
                    return rowNode->value == value;
                }
            }
            return false;
        } else if (type == ExpressionType::Binary) {
            if (op == "AND") {
                return left->evaluate(row, headers) && right->evaluate(row, headers);
            } else if (op == "OR") {
                return left->evaluate(row, headers) || right->evaluate(row, headers);
            }
        }
        return false;
    }
};