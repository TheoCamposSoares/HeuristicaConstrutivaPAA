#include "../include/leitorArquivos.hpp"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

bool lerInstancia(const std::string& caminhoArquivo, Instancia& instancia) {
    instancia.nomeArquivo = caminhoArquivo;
    instancia.clientes.clear();
    instancia.estacoes.clear();

    const std::size_t separador = caminhoArquivo.find_last_of("/\\");
    const std::string nomeArquivo = caminhoArquivo.substr(
        separador == std::string::npos ? 0 : separador + 1);
    int n = 0;
    int k = 0;
    int r = 0;
    int caracteresLidos = 0;
    const int extraidos = std::sscanf(
        nomeArquivo.c_str(),
        "%*[^-]-n%d-k%d-r%d.mtspds%n",
        &n,
        &k,
        &r,
        &caracteresLidos);

    if (extraidos != 3 || caracteresLidos != static_cast<int>(nomeArquivo.size())
        || n < 2 || k <= 0 || r <= 0) {
        std::cerr << "Erro: nome de arquivo invalido: " << nomeArquivo << '\n';
        return false;
    }

    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << caminhoArquivo << '\n';
        return false;
    }

    instancia.num_clientes = n - 1;
    instancia.num_estacoes = 0;
    instancia.raio = r;
    instancia.deposito.qnt_caminhoes = k;

    enum class Secao { nenhuma, deposito, clientes, estacoes };
    Secao secao = Secao::nenhuma;
    bool depositoLido = false;

    std::string linha;
    while (std::getline(arquivo, linha)) {
        if (linha.find("The Depot") != std::string::npos) {
            secao = Secao::deposito;
            continue;
        }
        if (linha.find("The Demand Locations") != std::string::npos) {
            secao = Secao::clientes;
            continue;
        }
        if (linha.find("The Drone Stations") != std::string::npos) {
            secao = Secao::estacoes;
            continue;
        }

        std::stringstream ss(linha);
        float x = 0.0f;
        float y = 0.0f;
        char virgula = '\0';
        if (!(ss >> x >> virgula >> y) || virgula != ',') {
            continue;
        }

        if (secao == Secao::deposito && !depositoLido) {
            instancia.deposito.posicao.x = x;
            instancia.deposito.posicao.y = y;
            depositoLido = true;
        } else if (secao == Secao::clientes) {
            if (static_cast<int>(instancia.clientes.size()) >= instancia.num_clientes) {
                std::cerr << "Erro: quantidade de clientes excede N-1 no arquivo: "
                          << caminhoArquivo << '\n';
                return false;
            }
            Cliente cliente;
            cliente.entregue = false;
            cliente.idCliente = static_cast<int>(instancia.clientes.size()) + 1;
            cliente.posicao.x = x;
            cliente.posicao.y = y;
            instancia.clientes.push_back(cliente);
        } else if (secao == Secao::estacoes) {
            EstacaoDrone estacao;
            estacao.ativado = false;
            estacao.alcance = r;
            estacao.idEstacao = static_cast<int>(instancia.estacoes.size()) + 1;
            estacao.posicao.x = x;
            estacao.posicao.y = y;
            instancia.estacoes.push_back(estacao);
        }
    }

    instancia.num_estacoes = static_cast<int>(instancia.estacoes.size());
    if (!depositoLido
        || static_cast<int>(instancia.clientes.size()) != instancia.num_clientes
        || instancia.estacoes.empty()) {
        std::cerr << "Erro: estrutura da instancia invalida: " << caminhoArquivo
                  << " (esperados " << instancia.num_clientes << " clientes; lidos "
                  << instancia.clientes.size() << ")\n";
        return false;
    }

    return true;
}