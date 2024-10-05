#include <string>

class Node {
public:
    std::string data;
    Node* next;

    Node(const std::string& data) : data(data), next(nullptr) {}
};