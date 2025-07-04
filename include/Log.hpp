#ifndef LOG_HPP
#define LOG_HPP

#include "List.hpp"
#include "VerticeArmazem.hpp"

/**
 * @class Log
 * @brief Representa a topologia da rede logística como um grafo.
 * * Utiliza uma lista de adjacências para armazenar os armazéns (vértices)
 * e suas conexões (arestas). É responsável por calcular as rotas dos pacotes.
 */
class Log {
private:
    List<VerticeArmazem*> armazens; // Lista de vértices do grafo
    int numArmazens;                // Número total de armazéns, usado para alocar vetores auxiliares

public:
    Log() : numArmazens(0) {}
    ~Log() {
        // Libera a memória de todos os vértices alocados
        while(!armazens.is_empty()) {
            delete armazens.front();
            armazens.pop_front();
        }
    }

    void setNumArmazens(int n) {
        numArmazens = n;
    }
    
    /**
     * @brief Encontra um vértice no grafo pelo ID do seu armazém.
     * @param id O ID do armazém a ser encontrado.
     * @return Ponteiro para o VerticeArmazem se encontrado, nullptr caso contrário.
     */
    VerticeArmazem* encontrarVertice(int id) {
        List<VerticeArmazem*>::Node* current = armazens.get_head();
        while (current) {
            if (current->data->storage->id == id) {
                return current->data;
            }
            current = current->next;
        }
        return nullptr;
    }

    /**
     * @brief Adiciona um novo armazém (vértice) ao grafo.
     * @param novo Ponteiro para o Armazem a ser adicionado.
     */
    void add_storage(Armazem* novo) {
        VerticeArmazem* vertice = new VerticeArmazem(novo);
        armazens.push_back(vertice);
    }

    /**
     * @brief Adiciona uma conexão (aresta) entre dois armazéns.
     * @param idOrigem ID do armazém de origem.
     * @param idDestino ID do armazém de destino.
     */
    void add_path(int idOrigem, int idDestino) {
        VerticeArmazem* origem = encontrarVertice(idOrigem);
        VerticeArmazem* destino = encontrarVertice(idDestino);
        if (origem && destino) {
            origem->vizinhos.push_back(destino); // Adiciona na lista de adjacências
        }
    }
    
    /**
     * @brief Encontra um armazém pelo seu ID.
     * @param id O ID do armazém.
     * @return Ponteiro para o Armazem se encontrado, nullptr caso contrário.
     */
    Armazem* encontrarArmazem(int id) {
        VerticeArmazem* v = encontrarVertice(id);
        return v ? v->storage : nullptr;
    }

    /**
     * @brief Calcula a rota de menor caminho entre dois armazéns usando Busca em Largura (BFS).
     * Ideal para grafos não ponderados.
     * @param origem ID do armazém de origem.
     * @param destino ID do armazém de destino.
     * @param rota A lista que será preenchida com a sequência de IDs de armazéns da rota.
     */
    void calcularRota(int origem, int destino, List<int>& rota) {
        if (origem == destino) {
            rota.push_back(origem);
            return;
        }

        // Vetores auxiliares para o BFS
        bool* visitado = new bool[numArmazens];
        int* anterior = new int[numArmazens];
        List<int> fila; // Usando a lista como fila

        for (int i = 0; i < numArmazens; ++i) {
            visitado[i] = false;
            anterior[i] = -1;
        }

        fila.push_back(origem);
        visitado[origem] = true;
        bool achou = false;

        // Loop principal do BFS
        while (!fila.is_empty()) {
            int atual = fila.front();
            fila.pop_front();

            VerticeArmazem* verticeAtual = encontrarVertice(atual);
            if (!verticeAtual) continue;

            // Itera sobre os vizinhos do vértice atual
            List<VerticeArmazem*>::Node* vizinhoNode = verticeAtual->vizinhos.get_head();
            while (vizinhoNode) {
                int idVizinho = vizinhoNode->data->storage->id;
                if (!visitado[idVizinho]) {
                    visitado[idVizinho] = true;
                    anterior[idVizinho] = atual;
                    fila.push_back(idVizinho);
                    if (idVizinho == destino) {
                        achou = true;
                        break;
                    }
                }
                vizinhoNode = vizinhoNode->next;
            }
            if (achou) break;
        }

        // Se encontrou um caminho, reconstrói a rota a partir do vetor 'anterior'
        if (achou) {
            List<int> caminhoInvertido;
            int atual = destino;
            while (atual != -1) {
                caminhoInvertido.push_front(atual);
                atual = anterior[atual];
            }
            
            // Copia o caminho para a lista de rota final
            auto curr = caminhoInvertido.get_head();
            while(curr) {
                rota.push_back(curr->data);
                curr = curr->next;
            }
        }
        
        // Libera a memória alocada para os vetores auxiliares
        delete[] visitado;
        delete[] anterior;
    }
};

#endif