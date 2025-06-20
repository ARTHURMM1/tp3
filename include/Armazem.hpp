#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include "Stack.hpp"
#include "Pacote.hpp"
#include "List.hpp"

struct PilhaPorDestino {
    int idDestino;
    Stack<Pacote*> pilha;

    PilhaPorDestino(int id = -1) : idDestino(id) {}
    bool operator==(const PilhaPorDestino& other) const {
        return idDestino == other.idDestino;
    }
};

class Armazem {
public:
    int id;
    List<PilhaPorDestino*> secoes; 
    Armazem(int id = -1) : id(id) {}
    ~Armazem() {
        auto curr = secoes.get_head();
        while (curr) {
            delete curr->data;
            curr = curr->next;
        }
    }

    int getId() const { return id; }

    void armazenarPacote(Pacote* pacote, int idProximoDestino) {
        auto curr = secoes.get_head(); 
        while (curr != nullptr) {
            if (curr->data->idDestino == idProximoDestino) {
                curr->data->pilha.push(pacote);
                return;
            }
            curr = curr->next;
        }
        // Se não existe, cria nova seção
        PilhaPorDestino* novaSecao = new PilhaPorDestino(idProximoDestino);
        novaSecao->pilha.push(pacote);
        secoes.push_back(novaSecao);
    }

    Pacote* retirarPacote(int idProximoDestino) {
        auto curr = secoes.get_head(); 
        while (curr != nullptr) {
            if (curr->data->idDestino == idProximoDestino) {
                if (!curr->data->pilha.is_empty()) {
                    return curr->data->pilha.pop();
                }
                return nullptr; 
            }
            curr = curr->next;
        }
        return nullptr; 
    }
};

#endif