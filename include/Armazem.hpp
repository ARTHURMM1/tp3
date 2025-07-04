#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include "Stack.hpp"
#include "Pacote.hpp"
#include "List.hpp"

/**
 * @struct PilhaPorDestino
 * @brief Estrutura que representa uma seção de um armazém.
 * * Cada seção armazena pacotes que têm o mesmo próximo destino.
 * A lógica de armazenamento é LIFO (Last-In, First-Out), implementada com uma pilha.
 */
struct PilhaPorDestino {
    int idDestino;        // ID do armazém para o qual os pacotes desta pilha se destinam
    Stack<Pacote*> pilha; // Pilha de pacotes

    PilhaPorDestino(int id = -1) : idDestino(id) {}
    bool operator==(const PilhaPorDestino& other) const {
        return idDestino == other.idDestino;
    }
};

/**
 * @class Armazem
 * @brief Modela um armazém na rede logística.
 * * Contém múltiplas seções (PilhaPorDestino), uma para cada armazém vizinho
 * para o qual há pacotes a serem enviados.
 */
class Armazem {
public:
    int id; // Identificador único do armazém
    List<PilhaPorDestino*> secoes; // Lista das seções de armazenamento do armazém

    Armazem(int id = -1) : id(id) {}
    ~Armazem() {
        // Libera a memória de todas as seções e dos pacotes que ainda estão nelas
        auto curr = secoes.get_head();
        while (curr) {
            // Libera a memória dos pacotes restantes na pilha
            while (!curr->data->pilha.is_empty()) {
                delete curr->data->pilha.pop();
            }
            delete curr->data; // Deleta a estrutura PilhaPorDestino
            curr = curr->next;
        }
    }

    int getId() const { return id; }

    /**
     * @brief Armazena um pacote em sua seção correspondente.
     * Se a seção para o destino do pacote não existir, ela é criada.
     * @param pacote O pacote a ser armazenado.
     * @param idProximoDestino O ID do próximo armazém na rota do pacote.
     */
    void armazenarPacote(Pacote* pacote, int idProximoDestino) {
        // Procura se a seção para o destino já existe
        auto curr = secoes.get_head(); 
        while (curr != nullptr) {
            if (curr->data->idDestino == idProximoDestino) {
                curr->data->pilha.push(pacote); // Encontrou, apenas empilha
                return;
            }
            curr = curr->next;
        }
        // Se não encontrou, cria uma nova seção e armazena o pacote
        PilhaPorDestino* novaSecao = new PilhaPorDestino(idProximoDestino);
        novaSecao->pilha.push(pacote);
        secoes.push_back(novaSecao);
    }

    /**
     * @brief Retorna um ponteiro para a pilha de uma seção específica.
     * Usado no main para manipular os pacotes durante um evento de transporte.
     * @param idProximoDestino O ID do destino da seção desejada.
     * @return Ponteiro para a Stack de pacotes, ou nullptr se a seção não existir.
     */
    Stack<Pacote*>* getPilhaPorDestino(int idProximoDestino) {
        auto curr = secoes.get_head();
        while (curr != nullptr) {
            if (curr->data->idDestino == idProximoDestino) {
                return &(curr->data->pilha);
            }
            curr = curr->next;
        }
        return nullptr;
    }
};

#endif