#ifndef ENTIDADES_HPP
#define ENTIDADES_HPP
#include <string>
#include <vector>
#include "entidades.hpp"

struct Coordenada{
    float x;
    float y;
};

struct Cliente{
    int id;
    Coordenada posicao;
    bool entregue;
};

struct EstacaoDrone{
    int id;
    Coordenada posicao;
    int qnt_drones;
    int alcance;
    bool ativado;
};

struct Deposito{
    Coordenada posicao;
    int qnt_caminhoes;
};

#endif