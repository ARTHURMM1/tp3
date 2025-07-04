#ifndef ARVORE_AVL_HPP
#define ARVORE_AVL_HPP

#include <algorithm>
#include <stdexcept>

template <typename Chave, typename Valor>
class ArvoreAVL {
private:
    struct No {
        Chave chave;
        Valor valor;
        No *esquerda;
        No *direita;
        int altura;

        No(const Chave& c, const Valor& v) : chave(c), valor(v), esquerda(nullptr), direita(nullptr), altura(1) {}
    };

    No* raiz;

    int altura(No* n) {
        return n ? n->altura : 0;
    }

    int fatorBalanceamento(No* n) {
        return n ? altura(n->esquerda) - altura(n->direita) : 0;
    }

    void atualizarAltura(No* n) {
        if (n) {
            n->altura = 1 + std::max(altura(n->esquerda), altura(n->direita));
        }
    }

    No* rotacaoDireita(No* y) {
        No* x = y->esquerda;
        No* T2 = x->direita;
        x->direita = y;
        y->esquerda = T2;
        atualizarAltura(y);
        atualizarAltura(x);
        return x;
    }

    No* rotacaoEsquerda(No* x) {
        No* y = x->direita;
        No* T2 = y->esquerda;
        y->esquerda = x;
        x->direita = T2;
        atualizarAltura(x);
        atualizarAltura(y);
        return y;
    }

    No* inserir(No* no, const Chave& chave, const Valor& valor) {
        if (!no) return new No(chave, valor);

        if (chave < no->chave) {
            no->esquerda = inserir(no->esquerda, chave, valor);
        } else if (chave > no->chave) {
            no->direita = inserir(no->direita, chave, valor);
        } else {
            // Chave já existe, não faz nada ou atualiza valor se necessário
            return no;
        }

        atualizarAltura(no);
        int fb = fatorBalanceamento(no);

        if (fb > 1 && chave < no->esquerda->chave) // Esquerda-Esquerda
            return rotacaoDireita(no);

        if (fb < -1 && chave > no->direita->chave) // Direita-Direita
            return rotacaoEsquerda(no);

        if (fb > 1 && chave > no->esquerda->chave) { // Esquerda-Direita
            no->esquerda = rotacaoEsquerda(no->esquerda);
            return rotacaoDireita(no);
        }

        if (fb < -1 && chave < no->direita->chave) { // Direita-Esquerda
            no->direita = rotacaoDireita(no->direita);
            return rotacaoEsquerda(no);
        }

        return no;
    }

    No* buscar(No* no, const Chave& chave) const {
        if (!no || no->chave == chave) {
            return no;
        }
        if (chave < no->chave) {
            return buscar(no->esquerda, chave);
        }
        return buscar(no->direita, chave);
    }
    
    void deletarNos(No* no) {
        if (no) {
            deletarNos(no->esquerda);
            deletarNos(no->direita);
            delete no->valor; // Assume que Valor é um ponteiro que precisa ser deletado
            delete no;
        }
    }


public:
    ArvoreAVL() : raiz(nullptr) {}
    ~ArvoreAVL() {
        // A lógica de limpeza profunda (deletar valores) depende da aplicação
    }
    
    void destruir() {
        deletarNos(raiz);
        raiz = nullptr;
    }

    void inserir(const Chave& chave, const Valor& valor) {
        raiz = inserir(raiz, chave, valor);
    }

    Valor buscar(const Chave& chave) const {
        No* no = buscar(raiz, chave);
        if (no) {
            return no->valor;
        }
        throw std::runtime_error("Chave nao encontrada");
    }

    bool contem(const Chave& chave) const {
        return buscar(raiz, chave) != nullptr;
    }
};

#endif