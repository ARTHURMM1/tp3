#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include "Stack.hpp"
#include "Pacote.hpp"
#include "List.hpp"


struct PilhaPorDestino {
    int idDestino;
    Stack<Pacote*> pilha;

    PilhaPorDestino(int id = -1) : idDestino(id) {}

    // Construtor de cópia: copia só o idDestino, pilha vazia
    PilhaPorDestino(const PilhaPorDestino& other) : idDestino(other.idDestino), pilha() {}

    bool operator==(const PilhaPorDestino& other) const {
        return idDestino == other.idDestino;
    }
};

class Armazem {
    

public:
    int id;
    List<PilhaPorDestino> secoes; 
    Armazem(int id = -1) : id(id) {}
    ~Armazem() {}

    int getId() const { return id; }

    
    void armazenarPacote(Pacote* pacote, int idProximoDestino) {
        
        List<PilhaPorDestino>::Node* curr = secoes.get_head(); 
        while (curr != nullptr) {
            if (curr->data.idDestino == idProximoDestino) {
                curr->data.pilha.push(pacote);
                return;
            }
            curr = curr->next;
        }

        
        PilhaPorDestino novaSecao(idProximoDestino);
        novaSecao.pilha.push(pacote);
        secoes.push_back(novaSecao);
    }

    
    Pacote* retirarPacote(int idProximoDestino) {
        List<PilhaPorDestino>::Node* curr = secoes.get_head(); 
        while (curr != nullptr) {
            if (curr->data.idDestino == idProximoDestino) {
                if (!curr->data.pilha.is_empty()) {
                    return curr->data.pilha.pop();
                }
                return nullptr; 
            }
            curr = curr->next;
        }
        return nullptr; 
    }
};

#endif