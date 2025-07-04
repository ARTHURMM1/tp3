#ifndef PROCESSADOR_HPP
#define PROCESSADOR_HPP

#include "Evento.hpp"
#include "List.hpp"
#include "ArvoreAVL.hpp"
#include <string>
#include <istream> // Adicionado para std::istream

class Processador {
public:
    Processador();
    ~Processador();

    // Assinatura alterada para aceitar um fluxo de entrada genérico
    void executar(std::istream& inputStream);

private:
    List<Evento*> baseDeEventos;
    ArvoreAVL<int, List<Evento*>*> indicePacotes;
    ArvoreAVL<std::string, List<int>*> indiceClientes;

    void processarLinha(const std::string& linha);
    void processarEvento(std::istringstream& ss, const std::string& linha);
    void processarConsultaPacote(std::istringstream& ss, const std::string& linha);
    void processarConsultaCliente(std::istringstream& ss, const std::string& linha);
    void adicionarClienteAoIndice(const std::string& cliente, int idPacote);
};

#endif