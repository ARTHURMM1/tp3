#ifndef VERTICEARMAZEM_HPP
#define VERTICEARMAZEM_HPP
#include "Armazem.hpp"
#include "List.hpp"

/**
 * @class VerticeArmazem
 * @brief Representa um vértice no grafo da rede logística.
 * * Esta classe serve como um nó no grafo, conectando a estrutura de dados
 * `Armazem` (que contém a lógica de armazenamento) com a estrutura do grafo
 * (através da lista de vizinhos).
 */
class VerticeArmazem {
public: 
    /**
     * @brief Construtor que associa um armazém a um vértice.
     */
    VerticeArmazem(Armazem* a) : storage(a) {}
    ~VerticeArmazem() {
        // O destrutor do vértice também deleta o armazém associado,
        // garantindo o gerenciamento correto da memória.
        delete storage; 
    }

    Armazem* storage;                       // Ponteiro para o armazém que este vértice representa.
    List<VerticeArmazem*> vizinhos;         // Lista de adjacências: ponteiros para os vértices vizinhos.
};

#endif