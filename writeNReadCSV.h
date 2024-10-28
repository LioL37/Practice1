using namespace std;

SinglyLinkedList<SinglyLinkedList<string>> readTableFromFile(const string& filePath, SinglyLinkedList<string>& headers) {
    ifstream file(filePath);
    string line;
    SinglyLinkedList<SinglyLinkedList<string>> table;

    // Читаем заголовки
    if (getline(file, line)) {
        stringstream ss(line);
        string header;
        while (getline(ss, header, ',')) {
            headers.addToTail(header);
        }
    }

    // Читаем данные
    while (getline(file, line)) {
        SinglyLinkedList<string> row;
        stringstream ss(line);
        string item;
        while (getline(ss, item, ',')) {
            row.addToTail(item);
        }
        table.addToTail(row);
    }

    file.close();
    return table;
}
void writeTableToFile(const string& filePath, const SinglyLinkedList<SinglyLinkedList<string>>& table, const SinglyLinkedList<string>& headers) {
    ofstream outFile(filePath);
    if (outFile.is_open()) {
        // Записываем заголовки
        SinglyLinkedList<string>::FLNode* headerNode = headers.head;
        while (headerNode) {
            outFile << headerNode->value;
            if (headerNode->next) {
                outFile << ",";
            }
            headerNode = headerNode->next;
        }
        outFile << endl;

        // Записываем данные
        SinglyLinkedList<SinglyLinkedList<string>>::FLNode* rowNode = table.head;
        while (rowNode) {
            SinglyLinkedList<string>::FLNode* cellNode = rowNode->value.head;
            while (cellNode) {
                outFile << cellNode->value;
                if (cellNode->next) {
                    outFile << ",";
                }
                cellNode = cellNode->next;
            }
            outFile << endl;
            rowNode = rowNode->next;
        }

        outFile.close();
        cout << "Deleted rows from table: " << filePath << endl;
    } else {
        cerr << "Failed to open file: " << filePath << endl;
    }
}