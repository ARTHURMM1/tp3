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
        while (i > 0 && heap[i].tempo < heap[pai(i)].tempo) {
           swap(i, pai(i));
           i = pai(i);
        }
    }

    void heapifyDown(int i) {
    int menor;
    while (true) {
        menor = i; 
        int esq = filhoEsquerda(i);
        int dir = filhoDireita(i);

        if (esq < tamanho && heap[esq].tempo < heap[menor].tempo) {
            menor = esq;
        }

        if (dir < tamanho && heap[dir].tempo < heap[menor].tempo) {
            menor = dir;
        }

        if (menor == i) {
            break;
        }
        
        swap(i, menor);
        i = menor;
    }
}
     
    void redimensionar() {
        int novaCapacidade = capacidade * 2;
        Evento* aux = new Evento[novaCapacidade];
        for (int i = 0; i < tamanho; i++) {
            aux[i] = heap[i];
        }
        delete[]heap;
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
        if (is_full()) {redimensionar();}
        heap[tamanho] = novoEvento;
        heapifyUp(tamanho);
        tamanho++;
    }


    Evento retiraProximoEvento() {
        if (is_empty()) {throw std::out_of_range ("Não há eventos.");}
        Evento temp = heap[0];
        heap[0] = heap[tamanho - 1];
        tamanho--;
        if (tamanho > 0) {heapifyDown(0);}
       
        return temp;
    }


    bool is_empty() const {return tamanho == 0;}
    bool is_full() const {return tamanho == capacidade;}
};

#endif