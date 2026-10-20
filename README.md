# Problema do Caixeiro Viajante Múltiplo com Estações de Drone (mTSP-DS)

Este repositório contém uma implementação modular e eficiente em C++ desenvolvida para resolver o Problema do Caixeiro Viajante Múltiplo com Estações de Drones (mTSP-DS). O objetivo principal é a otimização e minimização do makespan em diferentes configurações de instâncias.

## 📁 Estrutura do Repositório

```
mTSP-DS/
│
├── include/                   # Declarações e cabeçalhos (.hpp)
│   ├── entidades.hpp          # Definição das estruturas do problema (Vértices, Drones, Caixeiros, etc.)
│   ├── execCompleta.hpp       # Interface para execução completa de instâncias
│   ├── execUnica.hpp          # Interface para execução de uma instância individual
│   ├── heuristica.hpp         # Declaração da Heurística Construtiva Gulosa
│   ├── salvarExecUnica.hpp    # Declaração da função de salvar resultados únicos
│   └── leitorArquivos.hpp     # Declaração da leitura dos arquivos .mtspds
│
├── instancias/                # Conjunto de dados e instâncias de teste
│   ├── clustered/             # Instâncias com topologia espacial agrupada
│   └── scattered/             # Instâncias com topologia espacial dispersa
│
├── resultados/                # Relatórios e métricas de saída gerados
│   ├── detalhes_execucao.txt  # Resultados para as execuções únicas do usuário
│   ├── resultado05.csv        # Resultados para o fator de velocidade alpha = 0.5
│   ├── resultado1.csv         # Resultados para o fator de velocidade alpha = 1.0
│   ├── resultado2.csv         # Resultados para o fator de velocidade alpha = 2.0
│   └── tempo_medio.csv        # Tabela com os tempos médios de execução
│
├── src/                       # Implementação das lógicas em C++ (.cpp)
│   ├── LeitorArquivos.cpp     # Leitura dos dados das instâncias
│   ├── execCompleta.cpp       # Lógica da bateria de testes completa
│   ├── execUnica.cpp          # Lógica para teste e depuração individual
│   ├── heuristica.cpp         # Lógica da Heurística Construtiva Gulosa
│   ├── salvarExecUnica.cpp    # Salva os resultados da execução única
│   └── main.cpp               # Ponto de entrada da aplicação
│
├── .gitignore
└── Makefile                   # Automação de compilação                   
```


## ▶️ Como Compilar e Executar

### Requisitos Mínimos
Compilador g++ com suporte ao padrão C++11 ou superior.

### Compilando
Na pasta raiz do repositório, execute:

```
g++ -std=c++11 -O2 src/main.cpp src/LeitorArquivos.cpp src/heuristica.cpp src/execUnica.cpp src/execCompleta.cpp src/salvarExecUnica.cpp -o programa
```

Também é possível compilar com `make` no linux e `mingw32-make` no windows, usando o `Makefile` incluído (gera `programa.exe`).

### Executando
O programa deve ser executado a partir da pasta raiz do repositório, pois usa os caminhos relativos `instancias/` e `resultados/`.

- **Linux, macOS:** `./programa`
- **Windows:** `.\programa.exe`

Ao iniciar, o programa exibe um menu com dois modos.

**1 - Execução única.** Processa uma instância com uma configuração específica. São solicitados:
- o arquivo da instância, informado com a pasta e o nome, por exemplo `clustered\B-n34-k5-r8.mtspds` ou `scattered/A-n36-k5-r12.mtspds` (as duas barras são aceitas em qualquer sistema);
- a velocidade dos drones: 0.5, 1 ou 2;
- a quantidade de drones por estação, entre 1, 2 ou 3.

Em seguida, são exibidos os dados da instância, as atribuições dos drones, as rotas dos caminhões e o *makespan*. As mesmas informações são salvas no arquivo resultados/detalhes_execucao.txt. No caso de mais de uma execução única, os resultados são salvos no mesmo arquivo, um abaixo do outro.

**2 - Execução completa.** Processa todas as instâncias de `instancias/clustered` e `instancias/scattered`, com as 9 combinações de velocidade (0.5, 1 e 2) e de drones por estação (1 a 3). Os resultados são gravados na pasta `resultados/`, que precisa existir:

| Arquivo | Conteúdo |
|---|---|
| `resultado05.csv`, `resultado1.csv`, `resultado2.csv` | *Makespan* de cada instância para 1, 2 e 3 drones, um arquivo por velocidade |
| `tempo_medio.csv` | Média aritmética, em segundos, do tempo computacional das 9 execuções de cada instância |
