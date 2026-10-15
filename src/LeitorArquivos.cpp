#include "../include/leitorArquivos.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

bool lerInstancia(const std::string& caminhoArquivo, Instancia& instancia) {
    
    instancia.nomeArquivo = caminhoArquivo;
    instancia.clientes.clear();
    instancia.estacoes.clear();
    
    // Abre o arquivo
    std::ifstream arquivo(caminhoArquivo);

    // Verifica se o arquivo foi aberto com sucesso
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return false;
    }

    // Pega informacoes do nome do arquivo
    int n = 0, k = 0, r = 0;
    int extraidos = std::sscanf(caminhoArquivo.c_str(), "B-n%d-k%d-r%d.mtspds", &n, &k, &r);

    if (extraidos != 3) {
        std::cerr << "\nErro: O padrão ou a extensao nao coincidem!" << std::endl;
        return false;
    };

    instancia.deposito.qnt_caminhoes = k;

    int coordDeposito = 0;
    int idCliente = 1;
    int idEstacao = 1;

    std::string linha;
    char virgula;
    
    
    // Pega informacoes do texto armazenado no arquivo
    while (std::getline(arquivo, linha)) {
        if (linha.empty()) continue;
        char primeiroCarac = linha[0];
        
        if(!isdigit(primeiroCarac)) continue;
            std::stringstream ss(linha);
        
        if(coordDeposito == 0) {
            ss >> instancia.deposito.posicao.x >> virgula >> instancia.deposito.posicao.y;
            coordDeposito == 1;

        } else if(idCliente <= n){
            Cliente cliente;
            cliente.entregue = false;
            cliente.idCliente = idCliente;
            idCliente++;
            ss >> cliente.posicao.x >> virgula >> cliente.posicao.y;
            instancia.clientes.push_back(cliente);

        } else if (idEstacao <= 4) {
            EstacaoDrone estacao;
            estacao.ativado = false;
            estacao.alcance = r;
            estacao.idEstacao = idEstacao;
            idEstacao++;
            ss >> estacao.posicao.x >> virgula >> estacao.posicao.y;
            instancia.estacoes.push_back(estacao);
        };

    }

    // Fecha o arquivo
    arquivo.close();

    if(instancia.clientes.empty() ||instancia.estacoes.empty()) {
        std::cerr << "Erro: Não foi possível ler a instância" << caminhoArquivo << std::endl;
        return false;
    }
    
    return true;
}