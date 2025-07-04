#ifndef STACK_HPP
#define STACK_HPP

/**
 * @class Stack
 * @brief Template para uma pilha (Stack) baseada em array.
 * * Implementa a lógica LIFO (Last-In, First-Out).
 * * Utilizada para modelar as seções de armazenamento nos Armazéns, conforme
 * a especificação do problema.
 */
template <typename T>
class Stack {
    public:
        Stack(int maxsize = 1000000) : maxsize(maxsize), arr(new T[maxsize]), top(-1) {}
        ~Stack() { delete[] arr; }

        // Proíbe a cópia de pilhas.
        Stack(const Stack&) = delete;
        Stack& operator=(const Stack&) = delete;

        /**
         * @brief Insere um elemento no topo da pilha.
         */
        void push (T elemento) {
            if (top >= maxsize - 1) { // Verifica se a pilha está cheia
                return;
            }
            arr[++top] = elemento;
        }

        bool is_empty() const { return (top == -1); }

        /**
         * @brief Remove e retorna o elemento do topo da pilha.
         */
        T pop() {
            if (is_empty()) {
                return T(); // Retorna valor padrão se a pilha estiver vazia
            }
            return arr[top--];
        }

        /**
         * @brief Retorna o elemento do topo sem removê-lo.
         */
        T peek() const {
            if (is_empty()) {return T();}
            return arr[top];
        }

        int size() const {return top + 1;}

    private:
        int maxsize; // Capacidade máxima da pilha.
        T* arr;      // Array que armazena os elementos.
        int top;     // Índice do elemento no topo.
};

#endif