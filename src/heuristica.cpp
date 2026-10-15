#include "../include/heuristica.hpp"
#include <cmath>

double calcularDistancia(const Coordenada& a, const Coordenada& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

std::vector<int> clientesNoAlcance(
    const EstacaoDrone& estacao,
    const std::vector<Cliente>& clientes
) {
    std::vector<int> clientesAlcancaveis;
    for (const Cliente& cliente : clientes) {
        double distancia = calcularDistancia(estacao.posicao, cliente.posicao);
        if (distancia <= estacao.alcance) {
            clientesAlcancaveis.push_back(cliente.idCliente);
        }
    }
    return clientesAlcancaveis;
}

std::vector<int> selecionarEstacoes(const Instancia& instancia) {
    std::vector<int> estacoesSelecionadas;

    for (const EstacaoDrone& estacao : instancia.estacoes) {
        estacoesSelecionadas.push_back(estacao.idEstacao);
    }

    return estacoesSelecionadas;
}