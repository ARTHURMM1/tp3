#ifndef LIST_HPP
#define LIST_HPP

#include <stdexcept> // Para std::runtime_error

template <typename T>
class List {
    public:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    List() : head(nullptr), count(0) {}
    ~List() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push_front(const T& value) {
        Node* new_node = new Node(value);
        new_node->next = head;
        head = new_node;
        ++count;
    }

    void push_back(const T& value) {
        Node* new_node = new Node(value);
        if (!head) {
            head = new_node;
        } else {
            Node* curr = head;
            while (curr->next) {
                curr = curr->next;
            }
            curr->next = new_node;
        }
        ++count;
    }

    bool is_empty() const {
        return head == nullptr;
    }

    int size() const {
        return count;
    }

    bool pop_front() {
        if (!head) return false;
        Node* temp = head;
        head = head->next;
        delete temp;
        --count;
        return true;
    }

    bool pop_back() {
        if (!head) return false;
        if(!head->next) {
            delete head;
            head = nullptr;
            --count;
            return true;
        }
        Node* curr = head;
        while (curr->next->next) {
            curr = curr->next;
        }
        delete curr->next;
        curr->next = nullptr;
        --count;    
        return true;
    }

    T& front() {
        if (is_empty()) throw std::runtime_error("Lista vazia");
        return head->data;
    }

    const T& front() const {
        if (is_empty()) throw std::runtime_error("Lista vazia");
        return head->data;
    }

    Node* get_head() { // Correção: renomear para evitar conflito
        return head;
    }
private:
    
    Node* head;
    int count;


};

#endif