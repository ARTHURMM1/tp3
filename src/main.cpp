#include "Processador.hpp"
#include <iostream>
#include <fstream> // Necessário para std::ifstream

int main(int argc, char *argv[]) {
    // Verifica se o número de argumentos da linha de comando está correto.
    // argc deve ser 2: o nome do programa (argv[0]) e o nome do arquivo (argv[1]).
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_de_entrada>" << std::endl;
        return 1; // Retorna um código de erro
    }

    // Tenta abrir o arquivo de entrada fornecido como argumento
    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo '" << argv[1] << "'" << std::endl;
        return 1; // Retorna um código de erro
    }

    // Opcional: desativa a sincronização com stdio para otimizar a impressão na saída padrão
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(NULL);

    Processador p;
    p.executar(inputFile); // Passa o fluxo do arquivo para o processador

    inputFile.close(); // Fecha o arquivo ao final da execução

    return 0;
}