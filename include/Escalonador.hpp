#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "Evento.hpp"
#include <stdexcept>

#define HEAP_SIZE 1000000

class Escalonador {
private:
    Evento* heap;      
    int capacidade;    
    int tamanho;       

    int pai(int i) { return (i - 1) / 2; }
    int filhoEsquerda(int i) { return 2 * i + 1; }
    int filhoDireita(int i) { return 2 * i + 2; }

    void swap(int a, int b) {
        Evento temp = heap[b];
        heap[b] = heap[a];
        heap[a] = temp;
    }

    void heapifyUp(int i) {
        // --- ALTERAÇÃO: Comparação usa o operador > sobrecarregado de Evento ---
        while (i > 0 && heap[pai(i)] > heap[i]) {
           swap(i, pai(i));
           i = pai(i);
        }
    }

    void heapifyDown(int i) {
        int menor = i;
        int esq = filhoEsquerda(i);
        int dir = filhoDireita(i);

        // --- ALTERAÇÃO: Comparações usam o operador < sobrecarregado de Evento ---
        if (esq < tamanho && heap[esq] < heap[menor]) {
            menor = esq;
        }

        if (dir < tamanho && heap[dir] < heap[menor]) {
            menor = dir;
        }

        if (menor != i) {
            swap(i, menor);
            heapifyDown(menor);
        }
    }
     
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

    void insereEvento(const Evento& novoEvento) {
        if (is_full()) {
            redimensionar();
        }
        heap[tamanho] = novoEvento;
        tamanho++;
        heapifyUp(tamanho - 1);
    }

    Evento retiraProximoEvento() {
        if (is_empty()) {
            throw std::out_of_range("Não há eventos.");
        }
        Evento temp = heap[0];
        heap[0] = heap[tamanho - 1];
        tamanho--;
        if (tamanho > 0) {
            heapifyDown(0);
        }
        return temp;
    }

    bool is_empty() const { return tamanho == 0; }
    bool is_full() const { return tamanho == capacidade; }
};

#endif