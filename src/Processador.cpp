#include "Processador.hpp"
#include <iostream>
#include <sstream>
#include <iomanip> // Adicionado para std::setw e std::setfill

Processador::Processador() {}

Processador::~Processador() {
    // Libera a memória da base de eventos
    auto noEvento = baseDeEventos.get_head();
    while (noEvento) {
        delete noEvento->data;
        noEvento = noEvento->next;
    }
    
    // Libera a memória dos índices
    indicePacotes.destruir();
    indiceClientes.destruir();
}

void Processador::executar(std::istream& inputStream) {
    std::string linha;
    while (std::getline(inputStream, linha)) {
        if (!linha.empty()) {
            processarLinha(linha);
        }
    }
}

void Processador::processarLinha(const std::string& linha) {
    std::istringstream ss(linha);
    int dataHora;
    std::string tipo;

    ss >> dataHora >> tipo;

    if (tipo == "EV") {
        processarEvento(ss, linha);
    } else if (tipo == "PC") {
        processarConsultaPacote(ss, linha);
    } else if (tipo == "CL") {
        processarConsultaCliente(ss, linha);
    }
}

void Processador::adicionarClienteAoIndice(const std::string& cliente, int idPacote) {
    if (!indiceClientes.contem(cliente)) {
        indiceClientes.inserir(cliente, new List<int>());
    }
    List<int>* pacotesDoCliente = indiceClientes.buscar(cliente);
    bool jaExiste = false;
    auto noPacote = pacotesDoCliente->get_head();
    while(noPacote) {
        if (noPacote->data == idPacote) {
            jaExiste = true;
            break;
        }
        noPacote = noPacote->next;
    }
    if (!jaExiste) {
        pacotesDoCliente->push_back(idPacote);
    }
}

void Processador::processarEvento(std::istringstream& ss, const std::string& linha) {
    std::string tipoEvento;
    int idPacote;
    ss >> tipoEvento >> idPacote;
    
    Evento* novoEvento = new Evento(0, idPacote, tipoEvento, linha);
    baseDeEventos.push_back(novoEvento);

    if (!indicePacotes.contem(idPacote)) {
        indicePacotes.inserir(idPacote, new List<Evento*>());
    }
    indicePacotes.buscar(idPacote)->push_back(novoEvento);

    if (tipoEvento == "RG") {
        std::string remetente, destinatario;
        ss >> remetente >> destinatario;
        adicionarClienteAoIndice(remetente, idPacote);
        adicionarClienteAoIndice(destinatario, idPacote);
    }
}

void Processador::processarConsultaPacote(std::istringstream& ss, const std::string& linha) {
    int idPacote;
    ss >> idPacote;
    
    std::istringstream temp_ss(linha);
    int dataHoraConsulta;
    temp_ss >> dataHoraConsulta;

    // CORREÇÃO: Reformata a linha da consulta para a saída
    std::cout << std::setw(6) << std::setfill('0') << dataHoraConsulta << " PC ";
    std::cout << std::setw(3) << std::setfill('0') << idPacote << std::endl;

    if (!indicePacotes.contem(idPacote)) {
        std::cout << 0 << std::endl;
        return;
    }
    
    List<Evento*>* historicoCompleto = indicePacotes.buscar(idPacote);
    
    int contadorEventos = 0;
    auto noEvento = historicoCompleto->get_head();
    std::istringstream evento_ss;
    int dataHoraEvento;

    while(noEvento) {
        evento_ss.str(noEvento->data->linhaOriginal);
        evento_ss.clear();
        evento_ss >> dataHoraEvento;
        if (dataHoraEvento <= dataHoraConsulta) {
            contadorEventos++;
        }
        noEvento = noEvento->next;
    }

    std::cout << contadorEventos << std::endl;
    
    noEvento = historicoCompleto->get_head();
    while(noEvento) {
        evento_ss.str(noEvento->data->linhaOriginal);
        evento_ss.clear();
        evento_ss >> dataHoraEvento;
        if (dataHoraEvento <= dataHoraConsulta) {
            noEvento->data->imprimir();
        }
        noEvento = noEvento->next;
    }
}

void Processador::processarConsultaCliente(std::istringstream& ss, const std::string& linha) {
    std::string nomeCliente;
    ss >> nomeCliente;
    
    std::istringstream temp_ss(linha);
    int dataHoraConsulta;
    temp_ss >> dataHoraConsulta;

    // CORREÇÃO: Reformata a linha da consulta para a saída
    std::cout << std::setw(6) << std::setfill('0') << dataHoraConsulta << " CL " << nomeCliente << std::endl;

    if (!indiceClientes.contem(nomeCliente)) {
        std::cout << 0 << std::endl;
        return;
    }

    List<int>* pacotesDoCliente = indiceClientes.buscar(nomeCliente);
    List<Evento*> eventosColetados;

    auto noIdPacote = pacotesDoCliente->get_head();
    while (noIdPacote) {
        int idPacote = noIdPacote->data;
        List<Evento*>* historicoPacote = indicePacotes.buscar(idPacote);
        
        Evento* primeiroEvento = nullptr;
        Evento* ultimoEvento = nullptr;

        auto noEvento = historicoPacote->get_head();
        while(noEvento) {
            std::istringstream evento_ss(noEvento->data->linhaOriginal);
            int dataHoraEvento;
            evento_ss >> dataHoraEvento;

            if (dataHoraEvento <= dataHoraConsulta) {
                if (primeiroEvento == nullptr) {
                    primeiroEvento = noEvento->data;
                }
                ultimoEvento = noEvento->data;
            } else {
                break;
            }
            noEvento = noEvento->next;
        }

        if (primeiroEvento) {
            eventosColetados.push_back(primeiroEvento);
        }
        if (ultimoEvento && ultimoEvento != primeiroEvento) {
            eventosColetados.push_back(ultimoEvento);
        }

        noIdPacote = noIdPacote->next;
    }

    List<Evento*> eventosOrdenados;
    auto comparadorEventos = [](const Evento* a, const Evento* b) {
        std::istringstream ss_a(a->linhaOriginal);
        std::istringstream ss_b(b->linhaOriginal);
        int time_a, time_b;
        ss_a >> time_a;
        ss_b >> time_b;
        if (time_a != time_b) return time_a < time_b;
        return a->idPacote < b->idPacote;
    };
    
    auto noColetado = eventosColetados.get_head();
    while (noColetado) {
        eventosOrdenados.push_sorted(noColetado->data, comparadorEventos);
        noColetado = noColetado->next;
    }

    List<Evento*> eventosUnicos;
    if (!eventosOrdenados.is_empty()) {
        eventosUnicos.push_back(eventosOrdenados.get_head()->data);
        auto noAtual = eventosOrdenados.get_head()->next;
        auto noUnicoAnterior = eventosUnicos.get_head();

        while (noAtual) {
            if (noAtual->data != noUnicoAnterior->data) {
                eventosUnicos.push_back(noAtual->data);
                noUnicoAnterior = noUnicoAnterior->next;
            }
            noAtual = noAtual->next;
        }
    }
    
    std::cout << eventosUnicos.size() << std::endl;
    auto noFinal = eventosUnicos.get_head();
    while(noFinal) {
        noFinal->data->imprimir();
        noFinal = noFinal->next;
    }
}