#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "Evento.hpp"
#include <stdexcept>


#define HEAP_SIZE 100

class Escalonador {
private:
    Evento* heap;      
    int capacidade;    
    int tamanho;       

    
    int pai(int i) { return (i - 1) / 2; }
    int filhoEsquerda(int i) { return 2 * i + 1; }
    int filhoDireita(int i) { return 2 * i + 2; }
    void swap(Evento& a, Evento& b) {
        Evento temp = b;
        b = a;
        a = temp;
    }

    void heapifyUp(int i);    // Corrige a propriedade do heap "subindo"
    void heapifyDown(int i);  // Corrige a propriedade do heap "descendo"
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
        if (capacidade == tamanho) {redimensionar();}
        heap[tamanho] = novoEvento;
        heapifyUp(tamanho);
        tamanho++;
    }
    Evento retiraProximoEvento() {
        
    }
    bool is_empty() const {return tamanho == 0;}
};

#endif