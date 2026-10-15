#ifndef LEITOR_ARQUIVOS_HPP
#define LEITOR_ARQUIVOS_HPP
#include "entidades.hpp"
#include <string>
#include <vector>

// Estrutura para filtragem dos dados importantes de cada instancia
struct Instancia {
    std::string nomeArquivo;
    int num_clientes;
    int num_estacoes;
    int raio;

    std::vector<Cliente> clientes;
    std::vector<EstacaoDrone> estacoes;
    Deposito deposito;
};

// Funcao responsavel por ler cada arquivo do banco de instancias
bool lerInstancia(const std::string& caminhoArquivo, Instancia& instancia);

#endif