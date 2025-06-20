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
    // Limpa a rota antes de preencher (se você tiver um método clear, use-o)
    // Se não tiver, pode ignorar, pois você sempre preenche do zero.

    if (origem == destino) {
        rota.push_back(origem);
        return;
    }

    int visitado[100];
    int anterior[100];
    int fila[100];
    int ini = 0, fim = 0;

    for (int i = 0; i < numArmazens; ++i) {
        visitado[i] = 0;
        anterior[i] = -1;
    }

    fila[fim++] = origem;
    visitado[origem] = 1;

    bool achou = false;
    while (ini < fim) {
        int atual = fila[ini++];
        VerticeArmazem* verticeAtual = encontrarVertice(atual);
        if (!verticeAtual) continue;

        List<VerticeArmazem*>::Node* vizinhoNode = verticeAtual->vizinhos.get_head();
        while (vizinhoNode) {
            int idVizinho = vizinhoNode->data->storage->id;
            if (!visitado[idVizinho]) {
                visitado[idVizinho] = 1;
                anterior[idVizinho] = atual;
                fila[fim++] = idVizinho;
                if (idVizinho == destino) {
                    achou = true;
                    break;
                }
            }
            vizinhoNode = vizinhoNode->next;
        }
        if (achou) break;
    }

    // Reconstrói o caminho
    if (!visitado[destino]) {
        // Não existe caminho
        return;
    }
    int caminho[100];
    int tam = 0;
    int atual = destino;
    while (atual != -1) {
        caminho[tam++] = atual;
        atual = anterior[atual];
    }
    for (int i = tam - 1; i >= 0; --i) {
        rota.push_back(caminho[i]);
    }
}
    
};

#endif