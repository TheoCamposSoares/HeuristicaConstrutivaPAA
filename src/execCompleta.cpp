#include "../include/execCompleta.hpp"

#include "../include/heuristica.hpp"

#include <algorithm>
#include <array>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <string>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <dirent.h>
#endif

namespace {

const std::array<double, 3> VELOCIDADES = {{0.5, 1.0, 2.0}};
const std::array<const char*, 3> ARQUIVOS_RESULTADO = {{
    "resultado05.csv",
    "resultado1.csv",
    "resultado2.csv"
}};

bool listarInstanciasDiretorio(const std::string& diretorio,
                              std::vector<std::string>& instancias) {
#ifdef _WIN32
    const std::string padrao = diretorio + "\\*.mtspds";
    WIN32_FIND_DATAA arquivoEncontrado;
    HANDLE busca = FindFirstFileA(padrao.c_str(), &arquivoEncontrado);
    if (busca == INVALID_HANDLE_VALUE) {
        const DWORD erro = GetLastError();
        if (erro == ERROR_FILE_NOT_FOUND) {
            return true;
        }
        std::cerr << "Erro ao listar o diretorio de instancias: "
                  << diretorio << " (codigo " << erro << ").\n";
        return false;
    }

    do {
        if ((arquivoEncontrado.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
            instancias.push_back(
                diretorio + "\\" + arquivoEncontrado.cFileName);
        }
    } while (FindNextFileA(busca, &arquivoEncontrado));

    const DWORD erroBusca = GetLastError();
    FindClose(busca);
    if (erroBusca != ERROR_NO_MORE_FILES) {
        std::cerr << "Erro ao percorrer o diretorio de instancias: "
                  << diretorio << " (codigo " << erroBusca << ").\n";
        return false;
    }
#else
    DIR* diretorioAberto = opendir(diretorio.c_str());
    if (diretorioAberto == nullptr) {
        std::cerr << "Erro ao abrir o diretorio de instancias: "
                  << diretorio << '\n';
        return false;
    }

    struct dirent* entrada;
    while ((entrada = readdir(diretorioAberto)) != nullptr) {
        const std::string nome = entrada->d_name;
        if (nome.size() >= 7 &&
            nome.compare(nome.size() - 7, 7, ".mtspds") == 0) {
            instancias.push_back(diretorio + "/" + nome);
        }
    }
    closedir(diretorioAberto);
#endif
    return true;
}

bool escreverResultados(const std::string& caminho,
                        const std::vector<std::array<double, 3>>& resultados) {
    std::ofstream arquivo(caminho.c_str(), std::ios::out | std::ios::trunc);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao criar o arquivo de resultados: "
                  << caminho << '\n';
        return false;
    }

    arquivo.imbue(std::locale::classic());
    arquivo << "1 drone,2 drones,3 drones\n";
    arquivo << std::fixed << std::setprecision(6);
    for (const std::array<double, 3>& linha : resultados) {
        arquivo << linha[0] << ',' << linha[1] << ',' << linha[2] << '\n';
    }

    if (!arquivo) {
        std::cerr << "Erro ao gravar o arquivo de resultados: "
                  << caminho << '\n';
        return false;
    }
    return true;
}

}  // namespace

bool executarCompleta() {
    std::vector<std::string> instancias;
#ifdef _WIN32
    const std::string diretorioClustered = "instancias\\clustered";
    const std::string diretorioScattered = "instancias\\scattered";
    const std::string separador = "\\";
#else
    const std::string diretorioClustered = "instancias/clustered";
    const std::string diretorioScattered = "instancias/scattered";
    const std::string separador = "/";
#endif
    if (!listarInstanciasDiretorio(diretorioClustered, instancias) ||
        !listarInstanciasDiretorio(diretorioScattered, instancias)) {
        return false;
    }

    std::sort(instancias.begin(), instancias.end());
    if (instancias.empty()) {
        std::cerr << "Nenhuma instancia .mtspds foi encontrada nas pastas "
                     "instancias\\clustered e instancias\\scattered.\n";
        return false;
    }

    std::array<std::vector<std::array<double, 3>>, 3> resultados;
    for (std::size_t i = 0; i < instancias.size(); ++i) {
        Instancia instancia{};
        if (!lerInstancia(instancias[i], instancia)) {
            std::cerr << "Execucao completa interrompida na instancia: "
                      << instancias[i] << '\n';
            return false;
        }

        std::array<std::array<double, 3>, 3> resultadosInstancia;
        try {
            for (std::size_t indiceVelocidade = 0;
                 indiceVelocidade < VELOCIDADES.size();
                 ++indiceVelocidade) {
                for (int dronesPorEstacao = 1;
                     dronesPorEstacao <= 3;
                     ++dronesPorEstacao) {
                    const Solucao solucao = executarHeuristica(
                        instancia, dronesPorEstacao,
                        VELOCIDADES[indiceVelocidade]);
                    resultadosInstancia[indiceVelocidade]
                                       [dronesPorEstacao - 1] =
                        solucao.makespan;
                }
            }
        } catch (const std::exception& erro) {
            std::cerr << "Erro ao processar a instancia " << instancias[i]
                      << ": " << erro.what() << '\n';
            return false;
        }

        for (std::size_t indiceVelocidade = 0;
             indiceVelocidade < VELOCIDADES.size();
             ++indiceVelocidade) {
            resultados[indiceVelocidade].push_back(
                resultadosInstancia[indiceVelocidade]);
        }
    }

    for (std::size_t indiceVelocidade = 0;
         indiceVelocidade < ARQUIVOS_RESULTADO.size();
         ++indiceVelocidade) {
        const std::string caminho =
            std::string("resultados") + separador +
            ARQUIVOS_RESULTADO[indiceVelocidade];
        if (!escreverResultados(caminho, resultados[indiceVelocidade])) {
            return false;
        }
    }

    std::cout << "Execucao completa concluida: " << instancias.size()
              << " instancias processadas, 9 combinacoes por instancia.\n"
              << "Resultados salvos em resultados" << separador
              << "resultado05.csv, resultados" << separador
              << "resultado1.csv e resultados" << separador
              << "resultado2.csv.\n";
    return true;
}
