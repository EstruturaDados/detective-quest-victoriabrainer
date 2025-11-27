#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

// 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
//
// - Crie uma struct Pista com campo texto (string).
// - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
// - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
// - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
// - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
// - Não precisa remover ou balancear a árvore.
// - Use funções para modularizar: inserirPista(), listarPistas().
// - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

// ============================================================
//  Struct da Sala (nó da árvore binária da mansão)
// ============================================================
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;


// ============================================================
//  Struct da Árvore BST de Pistas
// ============================================================
typedef struct PistaNode {
    char texto[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;


// ============================================================
//  Protótipos de funções
// ============================================================
Sala* criarSala(const char* nome, const char* pista);
void conectarSalas(Sala* salaPai, Sala* salaEsquerda, Sala* salaDireita);

void explorarSalasComPistas(Sala* salaAtual, PistaNode** arvorePistas);

PistaNode* inserirPista(PistaNode* raiz, const char* texto);
void exibirPistas(PistaNode* raiz);

void limparBuffer();
void freePistaTree(PistaNode* raiz);

// ============================================================
//  FUNÇÃO PRINCIPAL
// ============================================================
int main() {
    // Árvore de pistas (BST)
    PistaNode* arvorePistas = NULL;

    // Criando as salas da mansão (algumas com pistas)
    Sala* hallEntrada = criarSala("Hall de Entrada", "");
    Sala* biblioteca  = criarSala("Biblioteca", "Livro rasgado com sangue");
    Sala* cozinha     = criarSala("Cozinha", "Faca desaparecida");
    Sala* estudio     = criarSala("Estúdio", "Carta suspeita");
    Sala* jardim      = criarSala("Jardim", "");
    Sala* deposito    = criarSala("Depósito", "Pegadas estranhas");
    Sala* jantar      = criarSala("Sala de Jantar", "");

    // Conectando as salas (árvore binária fixa)
    conectarSalas(hallEntrada, biblioteca, cozinha);
    conectarSalas(biblioteca, estudio, jardim);
    conectarSalas(cozinha, deposito, jantar);

    // Início da exploração
    printf("Bem-vindo(a) à mansão de Detective Quest!\n");
    printf("Começando a exploração...\n");

    explorarSalasComPistas(hallEntrada, &arvorePistas);

    // Exibindo pistas ao final
    printf("\nPISTAS COLETADAS (ORDEM ALFABÉTICA):\n");
    if (arvorePistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(arvorePistas);
    }

    freePistaTree(arvorePistas);
    return 0;
}

// ============================================================
//  Implementações das funções
// ============================================================

/**
 * @brief Cria uma nova sala com o nome e pista fornecidos.
 * @param nome Nome da sala.
 * @param pista Pista presente na sala.
 * @return Ponteiro para a nova sala criada.
 */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
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

// ============================================================
//  Exploração + Coleta de Pistas
// ============================================================

/**
 * @brief Explora a mansão a partir da sala atual, coletando pistas.
 * @param salaAtual Ponteiro para a sala atual.
 * @param arvorePistas Ponteiro duplo para a árvore de pistas.
 */
void explorarSalasComPistas(Sala* salaAtual, PistaNode** arvorePistas) {
    char opcao;

    while (salaAtual != NULL) {
        printf("\nVocê está em: **%s**\n", salaAtual->nome);

        // Se houver pista, coleta automaticamente
        if (strlen(salaAtual->pista) > 0) {
            printf("🕵️ Você encontrou uma pista: %s\n", salaAtual->pista);
            *arvorePistas = inserirPista(*arvorePistas, salaAtual->pista);

            // Evita coletar a mesma pista novamente
            strcpy(salaAtual->pista, "");
        }

        // Se for folha → fim do caminho
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Você chegou ao fim deste caminho!\n");
            return;
        }

        printf("Escolha o caminho:\n");
        if (salaAtual->esquerda != NULL)
            printf("  (e) Ir para a esquerda → %s\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL)
            printf("  (d) Ir para a direita  → %s\n", salaAtual->direita->nome);
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
//  BST DE PISTAS
// ============================================================


/**
 * @brief Insere uma nova pista na árvore de busca binária.
 * @param raiz Raiz da árvore de pistas.
 * @param texto Texto da pista a ser inserida.
 * @return Ponteiro para a raiz atualizada da árvore.
 */
PistaNode* inserirPista(PistaNode* raiz, const char* texto) {
    if (raiz == NULL) {
        PistaNode* nova = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(nova->texto, texto);
        nova->esquerda = NULL;
        nova->direita = NULL;
        return nova;
    }

    if (strcmp(texto, raiz->texto) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    } else if (strcmp(texto, raiz->texto) > 0) {
        raiz->direita = inserirPista(raiz->direita, texto);
    }

    return raiz;
}


/**
 * @brief Exibe as pistas em ordem alfabética (em ordem).
 * @param raiz Raiz da árvore de pistas.
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("🔎 %s\n", raiz->texto);
        exibirPistas(raiz->direita);
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


/**
 * @brief Libera a memória alocada para a árvore de pistas.
 * @param raiz Raiz da árvore de pistas.
 */
void freePistaTree(PistaNode* raiz) {
    if (raiz != NULL) {
        freePistaTree(raiz->esquerda);
        freePistaTree(raiz->direita);
        free(raiz);
    }
}