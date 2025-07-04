#ifndef PACOTE_HPP
#define PACOTE_HPP

#include <string>
#include "List.hpp"

// Enumeração dos estados possíveis de um pacote durante a simulação.
enum EstadoPacote {
    NAO_POSTADO,            // Estado inicial antes da postagem.
    CHEGADA_ESCALONADA,     // Evento de chegada agendado no escalonador.
    AGUARDANDO_ARMAZENAMENTO, // Estado intermediário (não utilizado nesta implementação).
    ARMAZENADO,             // Pacote guardado em uma seção de armazém.
    ALOCADO_TRANSPORTE,     // Pacote selecionado para transporte (não utilizado nesta implementação).
    ENTREGUE                // Pacote chegou ao seu destino final.
};

/**
 * @class Pacote
 * @brief Representa um pacote no sistema logístico.
 * * Armazena todas as informações relevantes de um pacote, como seus identificadores,
 * origem, destino, estado atual e a rota a ser percorrida.
 */
class Pacote {
public:
    int id;                 // ID original do pacote, lido do arquivo. Usado para lógica e desempate.
    int idImpressao;        // ID sequencial (0, 1, ...), usado para a impressão da saída, conforme o exemplo.
    int idArmazemOrigem;    // Armazém onde o pacote se encontra ou de onde partiu.
    int idArmazemDestino;   // Destino final do pacote.
    EstadoPacote estado;    // O estado atual do pacote no ciclo de vida da simulação.
    List<int> rota;         // Lista de IDs de armazéns que o pacote deve visitar até o destino.
    
    // Variáveis para estatísticas e lógica de prioridade
    double tempoPostagem;          // Momento em que o pacote foi postado. Usado para priorizar o transporte.
    double tempoTotalArmazenado;   // (Não utilizado nesta implementação) Para estatísticas.
    double tempoEntradaArmazem;    // (Não utilizado nesta implementação) Para estatísticas.

    /**
     * @brief Construtor do Pacote.
     * @param id_original ID lido do arquivo de entrada.
     * @param id_impr ID sequencial para impressão.
     * @param origem Armazém de origem do pacote.
     * @param destino Armazém de destino final do pacote.
     * @param tempo Momento da postagem.
     */
    Pacote(int id_original = -1, int id_impr = -1, int origem = -1, int destino = -1, double tempo = 0.0)
        : id(id_original),
          idImpressao(id_impr),
          idArmazemOrigem(origem),
          idArmazemDestino(destino),
          estado(NAO_POSTADO),
          tempoPostagem(tempo),
          tempoTotalArmazenado(0.0),
          tempoEntradaArmazem(0.0)
          {}
    
    /**
     * @brief Retorna o ID do próximo armazém na rota do pacote.
     * @return O ID do próximo destino, ou -1 se a rota estiver vazia.
     */
    int getProximoDestino() {
        if (!rota.is_empty()) {
            return rota.front();
        }
        return -1; 
    }
   
    /**
     * @brief Remove o próximo destino da rota, avançando o percurso do pacote.
     */
    void avancarRota() {
        if (!rota.is_empty()) {
            rota.pop_front();
        }
    }

    // A cópia de pacotes foi proibida para evitar problemas de gerenciamento de memória
    // e garantir que cada pacote seja uma entidade única na simulação.
    Pacote(const Pacote&) = delete;
    Pacote& operator=(const Pacote&) = delete;
};

#endif