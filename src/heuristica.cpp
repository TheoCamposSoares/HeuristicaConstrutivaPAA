#include "../include/heuristica.hpp"
#include <cmath>
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <unordered_set>
#include <vector>
#include <unordered_map>

double calcularDistancia(const Coordenada& a, const Coordenada& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

std::vector<int> clientesNoAlcance(const EstacaoDrone& estacao, const std::vector<Cliente>& clientes) {
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

std::vector<std::pair<int, int>> atribuirClientes(const Instancia& instancia,
    const std::vector<int>& estacoesSelecionadas) {
    std::vector<std::pair<int, int>> atribuicoes;

    for (const Cliente& cliente : instancia.clientes) {
        double menorDistancia = std::numeric_limits<double>::max();
        int melhorEstacao = -1;
        for (int idEstacao : estacoesSelecionadas) {
            for (const EstacaoDrone& estacao : instancia.estacoes) {
                if (estacao.idEstacao != idEstacao) {
                    continue;
                }
                double distancia = calcularDistancia(
                    cliente.posicao,
                    estacao.posicao
                );

                if (distancia <= estacao.alcance &&
                    distancia < menorDistancia) {
                    menorDistancia = distancia;
                    melhorEstacao = estacao.idEstacao;
                }
                break;
            }
        }

        if (melhorEstacao != -1) {
            atribuicoes.push_back({cliente.idCliente, melhorEstacao});
        }
    }
    return atribuicoes;
}

std::vector<RotaCaminhao> construirRotas(const Instancia& instancia, const std::vector<int>& estacoesSelecionadas,
    const std::vector<std::pair<int, int>>& atribuicoes) {
    const int numCaminhoes = instancia.deposito.qntCaminhoes;

    if (numCaminhoes <= 0) {
        throw std::invalid_argument("A instancia deve ter ao menos um caminhao.");
    }
    const Coordenada& deposito = instancia.deposito.posicao;

    // Guarda os clientes atendidos por drones e as estacoes utilizadas.
    std::unordered_set<int> clientesPorDrone;
    std::unordered_set<int> estacoesUtilizadas;

    for (const auto& atribuicao : atribuicoes) {
        int idCliente = atribuicao.first;
        int idEstacao = atribuicao.second;
        if (std::find(
                estacoesSelecionadas.begin(),
                estacoesSelecionadas.end(),
                idEstacao
            ) != estacoesSelecionadas.end()) {
            clientesPorDrone.insert(idCliente);
            estacoesUtilizadas.insert(idEstacao);
        }
    }

    // Converte o ID do no em sua coordenada.
    // 0 = deposito, positivo = cliente, negativo = estacao.
    auto obterPosicao = [&](int idNo) -> Coordenada {
        if (idNo == 0) {
            return deposito;
        }

        if (idNo > 0) {
            for (const Cliente& cliente : instancia.clientes) {
                if (cliente.idCliente == idNo) {
                    return cliente.posicao;
                }
            }
        } else {
            int idEstacao = -idNo;

            for (const EstacaoDrone& estacao : instancia.estacoes) {
                if (estacao.idEstacao == idEstacao) {
                    return estacao.posicao;
                }
            }
        }
        throw std::invalid_argument("ID de no inexistente.");
    };

    // Monta a lista de nos que precisam ser visitados por caminhoes.
    std::vector<int> nos;

    for (const Cliente& cliente : instancia.clientes) {
        if (clientesPorDrone.find(cliente.idCliente) ==
            clientesPorDrone.end()) {
            nos.push_back(cliente.idCliente);
        }
    }

    for (int idEstacao : estacoesUtilizadas) {
        nos.push_back(-idEstacao);
    }

    // sweep: ordena os nos pelo angulo em relacao ao deposito.
    std::sort(nos.begin(), nos.end(), [&](int a, int b) {
        Coordenada posA = obterPosicao(a);
        Coordenada posB = obterPosicao(b);

        double anguloA = std::atan2(
            posA.y - deposito.y,
            posA.x - deposito.x
        );

        double anguloB = std::atan2(
            posB.y - deposito.y,
            posB.x - deposito.x
        );
        return anguloA < anguloB;
    });

    // Divide os nos em grupos contiguos segundo a ordem Sweep.
    std::vector<std::vector<int>> grupos(numCaminhoes);
    const int totalNos = static_cast<int>(nos.size());
    const int base = totalNos / numCaminhoes;
    const int resto = totalNos % numCaminhoes;
    int indice = 0;

    for (int i = 0; i < numCaminhoes; ++i) {
        int quantidade = base + (i < resto ? 1 : 0);
        for (int j = 0; j < quantidade; ++j) {
            grupos[i].push_back(nos[indice++]);
        }
    }

    // Ordena cada grupo pelo vizinho mais proximo.
    std::vector<RotaCaminhao> rotas;
    rotas.reserve(numCaminhoes);

    for (int i = 0; i < numCaminhoes; ++i) {
        RotaCaminhao rota;
        rota.id = i + 1;
        rota.tempoViagem = 0.0;

        rota.sequenciaNos.push_back(0);
        Coordenada posAtual = deposito;
        std::vector<int> pendentes = grupos[i];

        while (!pendentes.empty()) {
            auto maisProximo = pendentes.begin();
            double menorDistancia = calcularDistancia(
                posAtual,
                obterPosicao(*maisProximo)
            );

            for (auto it = pendentes.begin() + 1;
                 it != pendentes.end(); ++it) {
                double distancia = calcularDistancia(
                    posAtual,
                    obterPosicao(*it)
                );
                if (distancia < menorDistancia) {
                    menorDistancia = distancia;
                    maisProximo = it;
                }
            }

            int proximoNo = *maisProximo;
            rota.tempoViagem += menorDistancia;
            rota.sequenciaNos.push_back(proximoNo);
            posAtual = obterPosicao(proximoNo);

            pendentes.erase(maisProximo);
        }

        // Retorna ao deposito.
        rota.tempoViagem += calcularDistancia(posAtual, deposito);
        rota.sequenciaNos.push_back(0);

        rotas.push_back(rota);
    }

    return rotas;
}