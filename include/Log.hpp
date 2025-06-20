#ifndef LOG_HPP
#define LOG_HPP

#include "List.hpp"
#include "VerticeArmazem.hpp"

class Log {
private:
    List<VerticeArmazem*> armazens;
    int numArmazens; 

public:
    Log() : numArmazens(0) {}
    ~Log() {
        while(!armazens.is_empty()) {
            delete armazens.front();
            armazens.pop_front();
        }
    }

    void setNumArmazens(int n) {
        numArmazens = n;
    }
    
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

    void add_storage(Armazem* novo) {
        VerticeArmazem* vertice = new VerticeArmazem(novo);
        armazens.push_back(vertice);
    }

    void add_path(int idOrigem, int idDestino) {
        VerticeArmazem* origem = encontrarVertice(idOrigem);
        VerticeArmazem* destino = encontrarVertice(idDestino);
        if (origem && destino) {
            origem->vizinhos.push_back(destino);
        }
    }
    
    Armazem* encontrarArmazem(int id) {
        VerticeArmazem* v = encontrarVertice(id);
        return v ? v->storage : nullptr;
    }

    void calcularRota(int origem, int destino, List<int>& rota) {
        if (origem == destino) {
            rota.push_back(origem);
            return;
        }

        // --- ALTERAÇÃO: Alocação dinâmica dos vetores auxiliares ---
        bool* visitado = new bool[numArmazens];
        int* anterior = new int[numArmazens];
        List<int> fila; // Usando a lista como fila para a BFS

        for (int i = 0; i < numArmazens; ++i) {
            visitado[i] = false;
            anterior[i] = -1;
        }

        fila.push_back(origem);
        visitado[origem] = true;
        bool achou = false;

        while (!fila.is_empty()) {
            int atual = fila.front();
            fila.pop_front();

            VerticeArmazem* verticeAtual = encontrarVertice(atual);
            if (!verticeAtual) continue;

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

        if (achou) {
            List<int> caminhoInvertido;
            int atual = destino;
            while (atual != -1) {
                caminhoInvertido.push_front(atual);
                atual = anterior[atual];
            }
            
            auto curr = caminhoInvertido.get_head();
            while(curr) {
                rota.push_back(curr->data);
                curr = curr->next;
            }
        }
        
        // --- ALTERAÇÃO: Libera memória alocada ---
        delete[] visitado;
        delete[] anterior;
    }
};

#endif