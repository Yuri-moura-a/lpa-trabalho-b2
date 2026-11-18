# Sistema de Gestão de Estacionamento

- **Disciplina:** Lógica de Programação e Algoritmos — 2026/2
- **Integrantes:** Yuri Jesus Damasceno de moura (@Yuri/moura/a), Pedro Emanuel Santos Ribeiro (@Emanuel7274)

## Cenário
Estacionamento fictício 4×5 (20 vagas). Dados vivem só em memória durante a sessão.

## Funcionalidades
Menu 1..8 + 0: cadastro, consulta, atualização, movimentação, remoção,
listagem, mapa e relatório de ocupação.

## Estruturas
- Matriz `int mapa[4][5]` — `0 = livre`, `1 = ocupada`.
- `struct Veiculo` — `id[8]`, `descricao[61]`, `categoria` (1/2), `fileira`, `coluna`.
- Vetor dinâmico `Veiculo *lista` — capacidade inicial **2**, dobra com `realloc`,
  teto em **20**. `qtd` separado de `capacidade`.

## Modularização
| Responsabilidade | Funções |
|---|---|
| Entrada validada | `lerLinha`, `lerInteiro`, `lerId`, `lerDescricao` |
| CRUD | `cadastrarVeiculo`, `consultarVeiculo`, `atualizarVeiculo`, `moverVeiculo`, `removerVeiculo` |
| Saída | `listarVeiculos`, `exibirMapa`, `exibirRelatorio`, `exibirMenu` |
| Busca | `buscarPorId` (comparação normalizada em maiúsculo) |

## Compilação e execução
```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c -o main
./main

## Declaração individual

Yuri

Atuei na realização dos commits 1,3,5,7
IA generativa: Usurfrui para apoio educacional para criação do Trabalho Bimestral, a IA utilziada foi o Deepseek

Pedro
Atuei na realização dos commits 2,4,6,8
IA generativa: Usurfrui para apoio educacional para criação do Trabalho Bimestral, a IA utilziada foi o Deepseek