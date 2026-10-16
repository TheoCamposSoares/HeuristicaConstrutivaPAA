#include "../include/leitorArquivos.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

bool extrairParametrosDoNome(const std::string& caminhoArquivo, int& qntClientes, int& qntCaminhoes, int& raio) {
    int extraidos = std::sscanf(caminhoArquivo.c_str(), "%*[^-]-n%d-k%d-r%d.mtspds", &qntClientes, &qntCaminhoes, &raio);
    return (extraidos == 3);
}

bool extrairConteudo(std::ifstream& arquivo, Instancia& instancia, int qntClientes, int raio) {
    int coordDeposito = 0;
    int idCliente = 1;
    int idEstacao = 1;

    std::string linha;
    char virgula;

    while (std::getline(arquivo, linha)) {
        if (linha.empty()) continue;
        
        char primeiroCarac = linha[0];
        if (!isdigit(primeiroCarac)) continue;

        std::stringstream ss(linha);

        if (coordDeposito == 0) {
            ss >> instancia.deposito.posicao.x >> virgula >> instancia.deposito.posicao.y;
            coordDeposito = 1;
        } 
        else if (idCliente <= qntClientes - 1) {
            Cliente cliente;
            cliente.entregue = false;
            cliente.idCliente = idCliente++;
            
            ss >> cliente.posicao.x >> virgula >> cliente.posicao.y;
            instancia.clientes.push_back(cliente);
        } 
        else {
            EstacaoDrone estacao;
            estacao.ativado = false;
            estacao.alcance = raio;
            estacao.idEstacao = idEstacao++;
            
            ss >> estacao.posicao.x >> virgula >> estacao.posicao.y;
            instancia.estacoes.push_back(estacao);
        }
    }

    return true;
}

bool lerInstancia(const std::string& caminhoArquivo, Instancia& instancia) {
    instancia.nomeArquivo = caminhoArquivo;
    instancia.clientes.clear();
    instancia.estacoes.clear();

    // 1. Abre o arquivo
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << caminhoArquivo << std::endl;
        return false;
    }

    // 2. Extrai as informações do nome do arquivo
    int qntClientes = 0, qntCaminhoes = 0, raio = 0;
    if (!extrairParametrosDoNome(caminhoArquivo, qntClientes, qntCaminhoes, raio)) {
        std::cerr << "\nErro: O padrão ou a extensão do nome do arquivo não coincidem!" << std::endl;
        arquivo.close();
        return false;
    }

    instancia.deposito.qntCaminhoes = qntCaminhoes;
    instancia.raio = raio;
    instancia.numClientes = qntClientes - 1;

    // 3. Processa o conteúdo do arquivo
    extrairConteudo(arquivo, instancia, qntClientes, raio);

    // 4. Fecha o arquivo
    arquivo.close();

    // 5. Validação final
    if (instancia.clientes.empty() || instancia.estacoes.empty()) {
        std::cerr << "Erro: Não foi possível ler os dados da instância " << caminhoArquivo << std::endl;
        return false;
    }

    return true;
}