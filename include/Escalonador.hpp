#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "Evento.hpp"
#include <stdexcept>

// Capacidade inicial do heap
#define HEAP_SIZE 1000000

/**
 * @class Escalonador
 * @brief Implementa uma fila de prioridade (Min-Heap) para gerenciar os eventos da simulação.
 * * Garante que os eventos sejam processados em ordem estritamente cronológica,
 * sempre retirando o evento com o menor tempo/chave de prioridade.
 */
class Escalonador {
private:
    Evento* heap;      // Array que armazena os elementos do heap
    int capacidade;    // Capacidade atual do array
    int tamanho;       // Número de elementos atualmente no heap

    // Funções auxiliares para navegação no heap
    int pai(int i) { return (i - 1) / 2; }
    int filhoEsquerda(int i) { return 2 * i + 1; }
    int filhoDireita(int i) { return 2 * i + 2; }

    void swap(int a, int b) {
        Evento temp = heap[b];
        heap[b] = heap[a];
        heap[a] = temp;
    }

    /**
     * @brief Corrige a propriedade do min-heap subindo um elemento.
     * Usado após a inserção.
     */
    void heapifyUp(int i) {
        // Compara o elemento com seu pai e troca se necessário, usando o operador '>' sobrecarregado da classe Evento
        while (i > 0 && heap[pai(i)] > heap[i]) {
           swap(i, pai(i));
           i = pai(i);
        }
    }

    /**
     * @brief Corrige a propriedade do min-heap descendo um elemento.
     * Usado após a remoção do topo.
     */
    void heapifyDown(int i) {
        int menor = i;
        int esq = filhoEsquerda(i);
        int dir = filhoDireita(i);

        // Encontra o menor entre o nó atual e seus filhos, usando o operador '<' sobrecarregado de Evento
        if (esq < tamanho && heap[esq] < heap[menor]) {
            menor = esq;
        }

        if (dir < tamanho && heap[dir] < heap[menor]) {
            menor = dir;
        }

        // Se o menor não for o nó atual, troca e continua o heapifyDown recursivamente
        if (menor != i) {
            swap(i, menor);
            heapifyDown(menor);
        }
    }
     
    /**
     * @brief Dobra a capacidade do heap quando ele está cheio.
     */
    void redimensionar() {
        int novaCapacidade = capacidade * 2;
        Evento* aux = new Evento[novaCapacidade];
        for (int i = 0; i < tamanho; i++) {
            aux[i] = heap[i];
        }
        delete[] heap;
        heap = aux;
        capacidade = novaCapacidade;
    }     

public:
    Escalonador() : capacidade(HEAP_SIZE), tamanho(0) {
        heap = new Evento[capacidade];
    }
    ~Escalonador() {
        delete [] heap;
    }

    /**
     * @brief Insere um novo evento na fila de prioridade.
     */
    void insereEvento(const Evento& novoEvento) {
        if (is_full()) {
            redimensionar();
        }
        heap[tamanho] = novoEvento;
        tamanho++;
        heapifyUp(tamanho - 1);
    }

    /**
     * @brief Remove e retorna o próximo evento a ser processado (o de menor prioridade).
     */
    Evento retiraProximoEvento() {
        if (is_empty()) {
            throw std::out_of_range("Não há eventos.");
        }
        Evento temp = heap[0];
        heap[0] = heap[tamanho - 1]; // Move o último elemento para o topo
        tamanho--;
        if (tamanho > 0) {
            heapifyDown(0); // Corrige a propriedade do heap
        }
        return temp;
    }

    bool is_empty() const { return tamanho == 0; }
    bool is_full() const { return tamanho == capacidade; }
};

#endif