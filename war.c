
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR 10
#define MAX_MISSAO 150

/* Estrutura dos territórios */
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

/* ==== PROTÓTIPOS ==== */
Territorio* cadastrarTerritorios(int qtd);
void exibirTerritorios(Territorio* mapa, int qtd);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa, char** missoesJogadores, int qtdJogadores);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
void exibirMissao(char* missao);
int verificarMissao(char* missao, Territorio* mapa, int tamanho);

/* ===========================
       FUNÇÃO PRINCIPAL
   =========================== */
int main() {
    srand(time(NULL));

    int qtdTerritorios;
    int qtdJogadores = 2; // simples para demonstração

    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &qtdTerritorios);
    getchar();

    /* ALOCAR TERRITÓRIOS */
    Territorio* mapa = cadastrarTerritorios(qtdTerritorios);

    /* CONJUNTO DE MISSÕES DISPONÍVEIS */
    char* missoesDisponiveis[] = {
        "Conquistar 2 territorios.",
        "Eliminar um territorio inimigo.",
        "Conquistar qualquer territorio azul.",
        "Reduzir qualquer inimigo para menos de 2 tropas.",
        "Conquistar um territorio consecutivamente."
    };
    int totalMissoes = 5;

    /* ALOCAR MISSÕES DOS JOGADORES */
    char** missoesJogadores = malloc(qtdJogadores * sizeof(char*));
    for (int i = 0; i < qtdJogadores; i++) {
        missoesJogadores[i] = malloc(MAX_MISSAO);
        atribuirMissao(missoesJogadores[i], missoesDisponiveis, totalMissoes);
    }

    printf("\n=== MISSOES DOS JOGADORES ===\n");
    for (int i = 0; i < qtdJogadores; i++) {
        printf("Jogador %d -> ", i + 1);
        exibirMissao(missoesJogadores[i]);
    }

    /* ==== MENU DE JOGO ==== */
    int opcao;
    do {
        printf("\n===== MENU =====\n");
        printf("1 - Exibir territorios\n");
        printf("2 - Realizar ataque\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            exibirTerritorios(mapa, qtdTerritorios);
        }
        else if (opcao == 2) {
            int a, d;

            exibirTerritorios(mapa, qtdTerritorios);
            printf("Escolha o numero do territorio atacante: ");
            scanf("%d", &a);
            printf("Escolha o numero do territorio defensor: ");
            scanf("%d", &d);

            if (a < 0 || a >= qtdTerritorios || d < 0 || d >= qtdTerritorios) {
                printf("Indice invalido!\n");
                continue;
            }

            if (strcmp(mapa[a].cor, mapa[d].cor) == 0) {
                printf("Um territorio nao pode atacar outro do mesmo exercito!\n");
                continue;
            }

            atacar(&mapa[a], &mapa[d]);

            /* === VERIFICAR MISSÕES === */
            for (int j = 0; j < qtdJogadores; j++) {
                if (verificarMissao(missoesJogadores[j], mapa, qtdTerritorios)) {
                    printf("\n\n===== VITÓRIA! =====\n");
                    printf("Jogador %d cumpriu sua missão!\n", j + 1);
                    liberarMemoria(mapa, missoesJogadores, qtdJogadores);
                    return 0;
                }
            }
        }

    } while (opcao != 0);

    liberarMemoria(mapa, missoesJogadores, qtdJogadores);
    return 0;
}

/* ================================
   FUNÇÃO DE CADASTRO DINÂMICO
   ================================ */
Territorio* cadastrarTerritorios(int qtd) {
    Territorio* mapa = malloc(qtd * sizeof(Territorio));

    if (!mapa) {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }

    for (int i = 0; i < qtd; i++) {
        printf("\nCadastro do Territorio %d\n", i);

        getchar();

        printf("Nome: ");
        fgets(mapa[i].nome, MAX_NOME, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;

        printf("Cor do exercito: ");
        fgets(mapa[i].cor, MAX_COR, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = 0;

        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }

    return mapa;
}

/* ================================
   EXIBIÇÃO DO MAPA
   ================================ */
void exibirTerritorios(Territorio* mapa, int qtd) {
    printf("\n===== TERRITORIOS =====\n");
    for (int i = 0; i < qtd; i++) {
        printf("[%d] Nome: %s | Cor: %s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/* ================================
   FUNÇÃO DE ATAQUE
   ================================ */
void atacar(Territorio* atacante, Territorio* defensor) {
    int da = (rand() % 6) + 1;
    int dd = (rand() % 6) + 1;

    printf("\n=== ATAQUE ===\n");
    printf("%s (Atacante) rolou: %d\n", atacante->nome, da);
    printf("%s (Defensor) rolou: %d\n", defensor->nome, dd);

    if (da > dd) {
        printf("Atacante venceu!\n");
        defensor->tropas /= 2;
        strcpy(defensor->cor, atacante->cor);
    }
    else {
        printf("Defensor venceu! Atacante perde 1 tropa.\n");
        if (atacante->tropas > 0)
            atacante->tropas--;
    }
}

/* ================================
   ATRIBUIR MISSÃO AO JOGADOR
   ================================ */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strcpy(destino, missoes[sorteio]);
}

/* ================================
   EXIBIR MISSÃO UMA VEZ
   ================================ */
void exibirMissao(char* missao) {
    printf("Missao: %s\n", missao);
}

/* ================================
   VERIFICAR MISSÃO
   (Lógica inicial e simples)
   ================================ */

int verificarMissao(char* missao, Territorio* mapa, int tamanho) {

    /* Exemplo 1:
       Se missão contém "Conquistar 2 territorios"
       verificar se há 2 territorios com mesma cor
    */
    if (strstr(missao, "Conquistar 2 territorios") != NULL) {
        int cont = 0;
        for (int i = 0; i < tamanho - 1; i++) {
            if (strcmp(mapa[i].cor, mapa[i + 1].cor) == 0)
                cont++;
        }
        if (cont >= 2) return 1;
    }

    /* Exemplo 2:
       Se missão contém "Eliminar um territorio inimigo"
       verifica se algum territorio está com 0 tropas
    */
    if (strstr(missao, "Eliminar um territorio inimigo") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas == 0)
                return 1;
        }
    }

    /* Exemplo simples: verificar cor azul */
    if (strstr(missao, "territorio azul") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "azul") == 0)
                return 1;
        }
    }

    return 0; // missão ainda não cumprida
}

/* ================================
   LIBERAR MEMÓRIA
   ================================ */
void liberarMemoria(Territorio* mapa, char** missoesJogadores, int qtdJogadores) {
    free(mapa);

    for (int i = 0; i < qtdJogadores; i++) {
        free(missoesJogadores[i]);
    }
    free(missoesJogadores);
}
