#ifndef LIST_HPP
#define LIST_HPP

#include <stdexcept> 

template <typename T>
class List {
public:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    List() : head(nullptr), tail(nullptr), count(0) {}
    ~List() {
        clear();
    }

    void push_back(const T& value) {
        Node* new_node = new Node(value);
        if (!head) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        ++count;
    }

    // NOVO: Método para inserção ordenada
    template <typename Compare>
    void push_sorted(const T& value, Compare comp) {
        Node* new_node = new Node(value);
        if (!head || comp(value, head->data)) {
            new_node->next = head;
            head = new_node;
            if (!tail) {
                tail = head;
            }
        } else {
            Node* curr = head;
            while (curr->next && !comp(value, curr->next->data)) {
                curr = curr->next;
            }
            new_node->next = curr->next;
            curr->next = new_node;
            if (!new_node->next) { // se o novo nó é o último
                tail = new_node;
            }
        }
        ++count;
    }


    bool is_empty() const {
        return head == nullptr;
    }

    int size() const {
        return count;
    }
    
    void clear() {
         while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }

    Node* get_head() const { 
        return head;
    }

    // Proíbe cópia para evitar problemas de gerenciamento de memória
    List(const List&) = delete;
    List& operator=(const List&) = delete;

private:
    Node* head;
    Node* tail;
    int count;
};

#endif