// Sistema de Gestao de Estacionamento
// Disciplina: Logica de Programacao e Algoritmos - 2026/2
// Integrantes: <Yuri Jesus Damasceno de moura>, <Pedro Emanuel Santos Ribeiro>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILEIRAS            4
#define COLUNAS             5
#define TOTAL_VAGAS         (FILEIRAS * COLUNAS)
#define TAM_ID              7
#define TAM_DESC            60
#define CAPACIDADE_INICIAL  2

typedef struct {
    char id[TAM_ID + 1];
    char descricao[TAM_DESC + 1];
    int  categoria;  
    int  fileira;     
    int  coluna;    
} Veiculo;

/* ---------- Prototipos ---------- */
int  lerLinha(char *buffer, int tamanho);
int  lerInteiro(const char *msg, int min, int max, int *saida);
int  lerId(const char *msg, char *saida);
int  lerDescricao(const char *msg, char *saida);
int  buscarPorId(Veiculo *lista, int qtd, const char *id);
void cadastrarVeiculo(Veiculo **lista, int *qtd, int *capacidade,int mapa[FILEIRAS][COLUNAS]);
void consultarVeiculo(Veiculo *lista, int qtd);
void atualizarVeiculo(Veiculo *lista, int qtd); 
void moverVeiculo(Veiculo *lista, int qtd, int mapa[FILEIRAS][COLUNAS]);
void removerVeiculo(Veiculo *lista, int *qtd, int mapa[FILEIRAS][COLUNAS]);
void listarVeiculos(Veiculo *lista, int qtd);
void exibirMapa(int mapa[FILEIRAS][COLUNAS]);
void exibirRelatorio(Veiculo *lista, int qtd, int mapa[FILEIRAS][COLUNAS]);
void exibirMenu(void);

/* ---------- Leitura segura ---------- */
int lerLinha(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) == NULL) return 0;
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { /* descarta */ }
    }
    return 1;
}

int lerInteiro(const char *msg, int min, int max, int *saida) {
    char entrada[100];
    char extra;
    while (1) {
        printf("%s", msg);
        if (fgets(entrada, sizeof(entrada), stdin) == NULL) return 0;
        int n = sscanf(entrada, " %d %c", saida, &extra);
        if (n == 1 && *saida >= min && *saida <= max) return 1;
        printf("Valor invalido. Digite um inteiro entre %d e %d.\n", min, max);
    }
}

int lerId(const char *msg, char *saida) {
    char entrada[100];
    while (1) {
        printf("%s", msg);
        if (!lerLinha(entrada, sizeof(entrada))) return 0;
        if (strlen(entrada) != TAM_ID) {
            printf("Identificador deve ter exatamente %d caracteres.\n", TAM_ID);
            continue;
        }
        int ok = 1;
        for (int i = 0; i < TAM_ID; i++) {
            char c = (char)toupper((unsigned char)entrada[i]);
            if (!((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))) {
                ok = 0; break;
            }
            saida[i] = c;
        }
        if (!ok) {
            printf("Use apenas letras A-Z e digitos 0-9.\n");
            continue;
        }
        saida[TAM_ID] = '\0';
        return 1;
    }
}

int lerDescricao(const char *msg, char *saida) {
    char entrada[200];
    while (1) {
        printf("%s", msg);
        if (!lerLinha(entrada, sizeof(entrada))) return 0;
        size_t len = strlen(entrada);
        if (len == 0) {
            printf("Descricao nao pode ser vazia.\n");
            continue;
        }
        if (len > TAM_DESC) {
            printf("Descricao deve ter no maximo %d caracteres.\n", TAM_DESC);
            continue;
        }
        strcpy(saida, entrada);
        return 1;
    }
}

int buscarPorId(Veiculo *lista, int qtd, const char *id) {
    for (int i = 0; i < qtd; i++)
        if (strcmp(lista[i].id, id) == 0) return i;
    return -1;
}

/* ---------- Operacoes ---------- */
void cadastrarVeiculo(Veiculo **lista, int *qtd, int *capacidade,
                      int mapa[FILEIRAS][COLUNAS]) {
    if (*qtd >= TOTAL_VAGAS) {
        printf("Estacionamento lotado (20/20). Cadastro recusado.\n");
        return;
    }

    char id[TAM_ID + 1];
    if (!lerId("Identificador (7 caracteres): ", id)) return;
    if (buscarPorId(*lista, *qtd, id) != -1) {
        printf("Identificador ja cadastrado.\n");
        return;
    }

    char descricao[TAM_DESC + 1];
    if (!lerDescricao("Descricao (ate 60 chars): ", descricao)) return;

    int categoria;
    if (!lerInteiro("Categoria (1=carro, 2=moto): ", 1, 2, &categoria)) return;

    int fileira, coluna;
    if (!lerInteiro("Fileira (1-4): ", 1, FILEIRAS, &fileira)) return;
    if (!lerInteiro("Coluna (1-5): ", 1, COLUNAS, &coluna)) return;

    if (mapa[fileira - 1][coluna - 1] != 0) {
        printf("Vaga F%d/C%d ja esta ocupada.\n", fileira, coluna);
        return;
    }

    if (*qtd == *capacidade) {
        int novaCap = *capacidade * 2;
        if (novaCap > TOTAL_VAGAS) novaCap = TOTAL_VAGAS;
        Veiculo *temp = realloc(*lista, novaCap * sizeof(Veiculo));
        if (temp == NULL) {
            printf("Falha ao ampliar memoria. Cadastro cancelado.\n");
            return;
        }
        *lista = temp;
        *capacidade = novaCap;
        printf("[info] Colecao ampliada para %d registros.\n", novaCap);
    }

    Veiculo *v = &(*lista)[*qtd];
    strcpy(v->id, id);
    strcpy(v->descricao, descricao);
    v->categoria = categoria;
    v->fileira   = fileira;
    v->coluna    = coluna;
    (*qtd)++;
    mapa[fileira - 1][coluna - 1] = 1;

    printf("Veiculo cadastrado: %s | F%d/C%d\n", id, fileira, coluna);
}

void consultarVeiculo(Veiculo *lista, int qtd) {
    if (qtd == 0) { printf("Nenhum veiculo cadastrado.\n"); return; }
    char id[TAM_ID + 1];
    if (!lerId("Identificador: ", id)) return;
    int idx = buscarPorId(lista, qtd, id);
    if (idx == -1) { printf("Veiculo nao localizado.\n"); return; }
    Veiculo *v = &lista[idx];
    printf("\n--- Dados do veiculo ---\n");
    printf("ID:        %s\n", v->id);
    printf("Descricao: %s\n", v->descricao);
    printf("Categoria: %s\n", v->categoria == 1 ? "Carro" : "Moto");
    printf("Posicao:   F%d/C%d\n\n", v->fileira, v->coluna);
}

void atualizarVeiculo(Veiculo *lista, int qtd) {
    if (qtd == 0) { printf("Nenhum veiculo cadastrado.\n"); return; }
    char id[TAM_ID + 1];
    if (!lerId("Identificador: ", id)) return;
    int idx = buscarPorId(lista, qtd, id);
    if (idx == -1) { printf("Veiculo nao localizado.\n"); return; }

    int opcao;
    printf("1 - Atualizar descricao\n");
    printf("2 - Atualizar categoria\n");
    printf("3 - Atualizar ambos\n");
    if (!lerInteiro("O que deseja atualizar: ", 1, 3, &opcao)) return;

    char novaDesc[TAM_DESC + 1];
    int  novaCat;

    if (opcao == 1 || opcao == 3)
        if (!lerDescricao("Nova descricao: ", novaDesc)) return;
    if (opcao == 2 || opcao == 3)
        if (!lerInteiro("Nova categoria (1=carro, 2=moto): ", 1, 2, &novaCat)) return;

    if (opcao == 1 || opcao == 3) strcpy(lista[idx].descricao, novaDesc);
    if (opcao == 2 || opcao == 3) lista[idx].categoria = novaCat;

    printf("Dados atualizados com sucesso.\n");
}

void moverVeiculo(Veiculo *lista, int qtd, int mapa[FILEIRAS][COLUNAS]) {
    if (qtd == 0) { printf("Nenhum veiculo cadastrado.\n"); return; }
    char id[TAM_ID + 1];
    if (!lerId("Identificador: ", id)) return;
    int idx = buscarPorId(lista, qtd, id);
    if (idx == -1) { printf("Veiculo nao localizado.\n"); return; }

    int fileira, coluna;
    if (!lerInteiro("Nova fileira (1-4): ", 1, FILEIRAS, &fileira)) return;
    if (!lerInteiro("Nova coluna (1-5): ", 1, COLUNAS, &coluna)) return;

    Veiculo *v = &lista[idx];
    if (v->fileira == fileira && v->coluna == coluna) {
        printf("Veiculo ja esta em F%d/C%d.\n", fileira, coluna);
        return;
    }
    if (mapa[fileira - 1][coluna - 1] != 0) {
        printf("Vaga F%d/C%d esta ocupada.\n", fileira, coluna);
        return;
    }

    mapa[v->fileira - 1][v->coluna - 1] = 0;
    mapa[fileira - 1][coluna - 1] = 1;
    v->fileira = fileira;
    v->coluna  = coluna;
    printf("Veiculo %s movido para F%d/C%d.\n", id, fileira, coluna);
}

void removerVeiculo(Veiculo *lista, int *qtd, int mapa[FILEIRAS][COLUNAS]) {
    if (*qtd == 0) { printf("Nenhum veiculo cadastrado.\n"); return; }
    char id[TAM_ID + 1];
    if (!lerId("Identificador: ", id)) return;
    int idx = buscarPorId(lista, *qtd, id);
    if (idx == -1) { printf("Veiculo nao localizado.\n"); return; }

    mapa[lista[idx].fileira - 1][lista[idx].coluna - 1] = 0;
    for (int i = idx; i < *qtd - 1; i++)
        lista[i] = lista[i + 1];
    (*qtd)--;

    printf("Veiculo %s removido. Vaga liberada.\n", id);
}

void listarVeiculos(Veiculo *lista, int qtd) {
    if (qtd == 0) { printf("Nenhum veiculo cadastrado.\n"); return; }
    printf("\n%-8s %-62s %-6s %-7s\n", "ID", "Descricao", "Categ.", "Posicao");
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < qtd; i++) {
        printf("%-8s %-62s %-6s F%d/C%d\n",
               lista[i].id,
               lista[i].descricao,
               lista[i].categoria == 1 ? "Carro" : "Moto",
               lista[i].fileira, lista[i].coluna);
    }
    printf("\n");
}

void exibirMapa(int mapa[FILEIRAS][COLUNAS]) {
    printf("\n     C1 C2 C3 C4 C5\n");
    for (int i = 0; i < FILEIRAS; i++) {
        printf("F%d | ", i + 1);
        for (int j = 0; j < COLUNAS; j++)
            printf("%c  ", mapa[i][j] == 0 ? '.' : 'X');
        printf("\n");
    }
    printf("Legenda: X = ocupada, . = livre\n\n");
}

void exibirRelatorio(Veiculo *lista, int qtd, int mapa[FILEIRAS][COLUNAS]) {
    int ocupadas = 0, carros = 0, motos = 0;
    for (int i = 0; i < FILEIRAS; i++)
        for (int j = 0; j < COLUNAS; j++)
            if (mapa[i][j] == 1) ocupadas++;
    for (int i = 0; i < qtd; i++) {
        if (lista[i].categoria == 1) carros++;
        else                         motos++;
    }
    int livres = TOTAL_VAGAS - ocupadas;
    float pct = (ocupadas / (float)TOTAL_VAGAS) * 100.0f;

    printf("\n===== RELATORIO DE OCUPACAO =====\n");
    printf("Total de vagas:      %d\n", TOTAL_VAGAS);
    printf("Vagas ocupadas:      %d\n", ocupadas);
    printf("Vagas livres:        %d\n", livres);
    printf("Percentual ocupacao: %.2f%%\n", pct);
    printf("Carros cadastrados:  %d\n", carros);
    printf("Motos cadastradas:   %d\n", motos);
    printf("Registros ativos:    %d\n", qtd);
    printf("=================================\n\n");
}

/* ---------- Menu ---------- */
void exibirMenu(void) {
    printf("\n===== ESTACIONAMENTO =====\n");
    printf("1 - Cadastrar veiculo\n");
    printf("2 - Consultar veiculo por identificador\n");
    printf("3 - Atualizar descricao/categoria\n");
    printf("4 - Mover veiculo\n");
    printf("5 - Remover veiculo\n");
    printf("6 - Listar veiculos cadastrados\n");
    printf("7 - Exibir mapa de vagas\n");
    printf("8 - Exibir relatorio de ocupacao\n");
    printf("0 - Encerrar\n");
}

/* ---------- main (esqueleto) ---------- */
int main(void) {
    int mapa[FILEIRAS][COLUNAS] = {0};
    int capacidade = CAPACIDADE_INICIAL;
    int qtd = 0;

    Veiculo *lista = malloc(capacidade * sizeof(Veiculo));
    if (lista == NULL) {
        printf("Erro ao alocar memoria inicial.\n");
        return 1;
    }

    int opcao;
    do {
        exibirMenu();
        if (!lerInteiro("Opcao: ", 0, 8, &opcao)) break;

        switch (opcao) {
            case 1: cadastrarVeiculo(&lista, &qtd, &capacidade, mapa); break;
            case 2: consultarVeiculo(lista, qtd);                      break;   
            case 3: atualizarVeiculo(lista, qtd);                      break;
            case 4: moverVeiculo(lista, qtd, mapa);                    break;
            case 5: removerVeiculo(lista, &qtd, mapa);                 break;
            case 6: listarVeiculos(lista, qtd);                        break;
            case 7: exibirMapa(mapa);                                  break;
            case 8: exibirRelatorio(lista, qtd, mapa);                 break;
            case 0: printf("Encerrando e liberando memoria...\n");     break;   
            default: printf("Funcionalidade em desenvolvimento.\n");   break;
        }

    } while (opcao != 0);

    free(lista);
    lista = NULL;
    return 0;
}