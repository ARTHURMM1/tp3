#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include "Stack.hpp"
#include "Pacote.hpp"
#include "List.hpp"

// Estrutura para associar uma pilha a um armazém de destino
struct PilhaPorDestino {
    int idDestino;
    Stack<Pacote*> pilha;

    PilhaPorDestino(int id = -1) : idDestino(id) {}
    // Necessário para a List<T> encontrar elementos
    bool operator==(const PilhaPorDestino& other) const {
        return idDestino == other.idDestino;
    }
};

class Armazem {
private:
    int id;
    List<PilhaPorDestino> secoes; // Uma lista de pilhas, uma para cada destino

public:
    Armazem(int id = -1) : id(id) {}
    ~Armazem() {}

    int getId() const { return id; }

    // Adiciona um pacote à seção correta (pilha)
    void armazenarPacote(Pacote* pacote, int idProximoDestino) {
        // Encontra a seção para o próximo destino
        List<PilhaPorDestino>::Node* curr = secoes.get_head(); // Correção: head() e tipo Node explícito
        while (curr != nullptr) {
            if (curr->data.idDestino == idProximoDestino) {
                curr->data.pilha.push(pacote);
                return;
            }
            curr = curr->next;
        }

        // Se a seção não existe, cria uma nova
        PilhaPorDestino novaSecao(idProximoDestino);
        novaSecao.pilha.push(pacote);
        secoes.push_back(novaSecao);
    }

    // Retira um pacote da seção especificada
    Pacote* retirarPacote(int idProximoDestino) {
        List<PilhaPorDestino>::Node* curr = secoes.get_head(); // Correção: head() e tipo Node explícito
        while (curr != nullptr) {
            if (curr->data.idDestino == idProximoDestino) {
                if (!curr->data.pilha.is_empty()) {
                    return curr->data.pilha.pop();
                }
                return nullptr; // Pilha vazia
            }
            curr = curr->next;
        }
        return nullptr; // Seção não encontrada
    }
};

#endif