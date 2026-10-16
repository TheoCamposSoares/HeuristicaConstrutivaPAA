#ifndef LEITOR_ARQUIVOS_HPP
#define LEITOR_ARQUIVOS_HPP
#include "entidades.hpp"
#include <string>
#include <vector>
#include <fstream>

// Estrutura para filtragem dos dados importantes de cada instancia
struct Instancia {
    std::string nomeArquivo;
    int numClientes;
    int numEstacoes;
    int raio;

    std::vector<Cliente> clientes;
    std::vector<EstacaoDrone> estacoes;
    Deposito deposito;
};

// Extrai as informacoes necessarias do nome do arquivo
bool extrairParametrosDoNome(const std::string& caminhoArquivo, int& qntClientes, int& qntCaminhoes, int& raio);

// Extrai as informacoes necessarias do corpo do texto
bool extrairConteudo(std::ifstream& arquivo, Instancia& instancia, int qntClientes, int raio);

// Funcao responsavel por ler cada arquivo do banco de instancias
bool lerInstancia(const std::string& caminhoArquivo, Instancia& instancia);

#endif