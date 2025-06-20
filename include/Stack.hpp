#ifndef STACK_HPP
#define STACK_HPP

template <typename T>

class Stack {
    public:
        Stack(int maxsize = 1000000) : maxsize(maxsize), arr(new T[maxsize]), top(-1) {}
        ~Stack() { delete[] arr; }

        // PROÍBE CÓPIA
        Stack(const Stack&) = delete;
        Stack& operator=(const Stack&) = delete;

        void push (T elemento) {
            if (top >= maxsize - 1) {
                return;
            }
            arr[++top] = elemento;
        }

        bool is_empty() const { return (top == -1); }

        T pop() {
            if (is_empty()) {
                return T();
            }
            return arr[top--];
        }

        T peek() const {
            if (is_empty()) {return T();}
            return arr[top];
        }

        int size() const {return top + 1;}
    private:
        int maxsize;
        T* arr;
        int top;
};

#endif