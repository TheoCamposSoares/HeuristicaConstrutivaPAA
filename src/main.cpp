#include "../include/execCompleta.hpp"
#include "../include/execUnica.hpp"

#include <iostream>

int main() {
    std::cout << "=== Heuristica construtiva ===\n"
              << "1 - Execucao unica\n"
              << "2 - Execucao completa\n";

    int tipoExecucao = 0;
    if (!lerOpcaoInteira("Escolha o tipo de execucao: ", {1, 2}, tipoExecucao)) {
        std::cerr << "\nEntrada encerrada antes da escolha do tipo de execucao.\n";
        return 1;
    }

    if (tipoExecucao == 2) {
        return executarCompleta() ? 0 : 1;
    }

    if (!executarUnica()) {
        if (std::cin.eof()) {
            std::cerr << "\nEntrada encerrada antes de concluir a execucao.\n";
        }
        return 1;
    }

    return 0;
}
