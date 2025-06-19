#ifndef LOG_HPP
#define LOG_HPP

#include "List.hpp"
#include "VerticeArmazem.hpp"
#include "Pacote.hpp"

#define MAX_ARMAZENS 100 // Assumindo um número máximo de armazéns para arrays auxiliares

class Log {
private:
    List<VerticeArmazem*> armazens;

    

public:
    Log() {}
    ~Log() {
        // Libera a memória dos vértices
        while(!armazens.is_empty()) {
            delete armazens.front();
            armazens.pop_front();
        }
    }
    
    VerticeArmazem* encontrarVertice(int id) {
        List<VerticeArmazem*>::Node* current = armazens.get_head(); // Correção
        while (current) {
            if (current->data->storage.getId() == id) {
                return current->data;
            }
            current = current->next;
        }
        return nullptr;
    }

    void add_storage(Armazem* novo) {
        VerticeArmazem* vertice = new VerticeArmazem(*novo); // Correção: desreferencia o ponteiro
        armazens.push_back(vertice);
    }

    void add_path(int idOrigem, int idDestino) {
        VerticeArmazem* origem = encontrarVertice(idOrigem);
        VerticeArmazem* destino = encontrarVertice(idDestino);
        if (origem && destino) {
            origem->vizinhos.push_back(destino);
            destino->vizinhos.push_back(origem);
        }
    }
    
    Armazem* encontrarArmazem(int id) {
        VerticeArmazem* v = encontrarVertice(id);
        return v ? &(v->storage) : nullptr;
    }

    List<int> calcularRota(int idOrigem, int idDestino) {
        List<int> rota;
        if (idOrigem == idDestino) return rota;

        List<int> fila;
        bool visitado[MAX_ARMAZENS] = {false};
        int predecessor[MAX_ARMAZENS];
        for(int i=0; i<MAX_ARMAZENS; ++i) predecessor[i] = -1;

        fila.push_back(idOrigem);
        visitado[idOrigem] = true;

        bool encontrado = false;
        while (!fila.is_empty()) {
            int atualId = fila.front();
            fila.pop_front();

            if (atualId == idDestino) {
                encontrado = true;
                break;
            }

            VerticeArmazem* verticeAtual = encontrarVertice(atualId);
            if (verticeAtual) {
                List<VerticeArmazem*>::Node* vizinhoNode = verticeAtual->vizinhos.get_head(); // Correção
                while (vizinhoNode) {
                    int vizinhoId = vizinhoNode->data->storage.getId();
                    if (!visitado[vizinhoId]) {
                        visitado[vizinhoId] = true;
                        predecessor[vizinhoId] = atualId;
                        fila.push_back(vizinhoId);
                    }
                    vizinhoNode = vizinhoNode->next;
                }
            }
        }

        if (encontrado) {
            List<int> rotaInvertida;
            int passo = idDestino;
            while (passo != -1) {
                rotaInvertida.push_front(passo);
                passo = predecessor[passo];
            }
            
            rotaInvertida.pop_front();
            
            List<int>::Node* node = rotaInvertida.get_head(); // Correção
            while(node) {
                rota.push_back(node->data);
                node = node->next;
            }
        }
        
        return rota;
    }
};

#endif