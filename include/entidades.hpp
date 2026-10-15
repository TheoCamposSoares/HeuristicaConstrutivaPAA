#ifndef ENTIDADES_HPP
#define ENTIDADES_HPP
#include <string>
#include <vector>

struct Coordenada {
    float x;
    float y;
};

struct Cliente {
    int idCliente;
    Coordenada posicao;
    bool entregue;
};

struct EstacaoDrone {
    int idEstacao;
    Coordenada posicao;
    int alcance;
    bool ativado;
};

struct Deposito {
    Coordenada posicao;
    int qnt_caminhoes;
};

struct RotaCaminhao {
    int id;
    std::vector<int> sequencia_nos;
    double tempo_viagem;
};

// Estrutura que armazena a solução completa para o problema
struct Solução {
    std::vector<RotaCaminhao> rotas_caminhoes;
    std::vector<std::pair<int, int>> atendimento_drones; // <idEstacao, idCliente>
    double makespan;
};

#endif