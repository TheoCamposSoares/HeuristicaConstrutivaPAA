#ifndef HEURISTICA_HPP
#define HEURISTICA_HPP
#include <string>
#include <vector>
#include "entidades.hpp"
#include "leitorArquivos.hpp"

double calcularDistancia(const Coordenada& a, const Coordenada& b);

std::vector<int> clientesNoAlcance(
    const EstacaoDrone& estacao,
    const std::vector<Cliente>& clientes
);

std::vector<int> selecionarEstacoes(const Instancia& instancia);

#endif