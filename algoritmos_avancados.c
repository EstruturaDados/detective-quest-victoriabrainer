#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

// 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
//
// - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
// - Use funções como criarSala(), conectarSalas() e explorarSalas().
// - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
// - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
// - Finalize a exploração com uma opção de saída (s).
// - Exiba o nome da sala a cada movimento.
// - Use recursão ou laços para caminhar pela árvore.
// - Nenhuma inserção dinâmica é necessária neste nível.

// ============================================================
//  Struct da Sala (nó da árvore binária)
// ============================================================
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;


// ============================================================
//  Protótipos de funções
// ============================================================
Sala* criarSala(const char* nome);
void conectarSalas(Sala* salaPai, Sala* salaEsquerda, Sala* salaDireita);
void explorarSalas(Sala* salaAtual);

void limparBuffer();

// ============================================================
//  FUNÇÃO PRINCIPAL
// ============================================================

int main() {
    // Criando as salas da mansão
    Sala* hallEntrada = criarSala("Hall de Entrada");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* cozinha = criarSala("Cozinha");

    Sala* estudio = criarSala("Estúdio");
    Sala* jardim = criarSala("Jardim");

    Sala* deposito = criarSala("Depósito");
    Sala* jantar = criarSala("Sala de Jantar");


    // Conectando as salas (árvore binária fixa)
    conectarSalas(hallEntrada, biblioteca, cozinha);
    conectarSalas(biblioteca, estudio, jardim);
    conectarSalas(cozinha, deposito, jantar);

    // Iniciando a exploração da mansão
    printf("Bem-vindo(a) à mansão de Detective Quest!\n");
    printf("Começando a exploração...\n");

    explorarSalas(hallEntrada);

    return 0;
}


// ============================================================
//  Implementações das funções
// ============================================================

/**
 * @brief Cria uma nova sala com o nome fornecido.
 * @param nome Nome da sala.
 * @return Ponteiro para a nova sala criada.
 */
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para sala!\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}


/**
 * @brief Conecta duas salas à esquerda e direita de uma sala pai.
 * @param salaPai Ponteiro para a sala pai.
 * @param salaEsquerda Ponteiro para a sala à esquerda.
 * @param salaDireita Ponteiro para a sala à direita.
 */
void conectarSalas(Sala* salaPai, Sala* salaEsquerda, Sala* salaDireita) {
    if (salaPai != NULL) {
        salaPai->esquerda = salaEsquerda;
        salaPai->direita = salaDireita;
    }
}


/**
 * @brief Explora a mansão a partir da sala atual.
 * @param salaAtual Ponteiro para a sala atual.
 */
void explorarSalas(Sala* salaAtual) {
    char opcao;

    while (salaAtual != NULL) {
        printf("\nVocê está em: **%s**\n", salaAtual->nome);

        // Se for folha → fim do caminho
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Você chegou ao fim deste caminho! (nó-folha)\n");
            return;
        }

        printf("Escolha seu caminho:\n");
        if (salaAtual->esquerda != NULL) printf("  (e) Ir para a esquerda → %s\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL)  printf("  (d) Ir para a direita  → %s\n", salaAtual->direita->nome);
        printf("  (s) Sair da exploração\n");

        printf("Opção: ");
        scanf(" %c", &opcao);
        limparBuffer();

        if (opcao == 'e' && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda;
        }
        else if (opcao == 'd' && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita;
        }
        else if (opcao == 's') {
            printf("Exploração encerrada.\n");
            return;
        }
        else {
            printf("Opção inválida, tente novamente.\n");
        }
    }
}


// ============================================================
//  Funções Auxiliares
// ============================================================

/**
 * @brief Limpa o buffer de entrada para evitar problemas com scanf.
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
