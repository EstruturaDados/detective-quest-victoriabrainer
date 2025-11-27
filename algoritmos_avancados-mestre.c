#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

// 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
//
// - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
// - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
// - A chave pode ser o nome do suspeito ou derivada das pistas.
// - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
// - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
// - Adicione um contador para saber qual suspeito foi mais citado.
// - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
// - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
// - Em caso de colisão, use lista encadeada para tratar.
// - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().


// ============================================================
//  Struct da Sala (Árvore da Mansão)
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
//  Struct da Tabela Hash (Pista -> Suspeito)
// ============================================================
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    int contador;
    struct HashNode* prox;
} HashNode;

HashNode* tabelaHash[TAM_HASH];

// ============================================================
//  Protótipos de funções
// ============================================================
Sala* criarSala(const char* nome, const char* pista);
void conectarSalas(Sala* salaPai, Sala* salaEsquerda, Sala* salaDireita);
void explorarSalasComPistas(Sala* salaAtual, PistaNode** arvorePistas);

PistaNode* inserirPista(PistaNode* raiz, const char* texto);
void exibirPistas(PistaNode* raiz);

void inicializarHash();
int funcaoHash(const char* chave);
void inserirNaHash(const char* pista, const char* suspeito);
void listarAssociacoes();
char* encontrarSuspeito(const char* pista);
void verificarSuspeitoFinal();
void mostrarSuspeitoMaisCitado();

void limparBuffer();
void freePistaTree(PistaNode* raiz);
// ============================================================
//  FUNÇÃO PRINCIPAL
// ============================================================
int main() {
    PistaNode* arvorePistas = NULL;
    inicializarHash();

    // Criando as salas
    Sala* hallEntrada = criarSala("Hall de Entrada", "");
    Sala* biblioteca  = criarSala("Biblioteca", "Livro rasgado com sangue");
    Sala* cozinha     = criarSala("Cozinha", "Faca desaparecida");
    Sala* estudio     = criarSala("Estúdio", "Carta suspeita");
    Sala* jardim      = criarSala("Jardim", "");
    Sala* deposito    = criarSala("Depósito", "Pegadas estranhas");
    Sala* jantar      = criarSala("Sala de Jantar", "");

    conectarSalas(hallEntrada, biblioteca, cozinha);
    conectarSalas(biblioteca, estudio, jardim);
    conectarSalas(cozinha, deposito, jantar);

    printf("Bem-vindo(a) à mansão Detective Quest!\n");
    printf("Começando a exploração...\n");
    explorarSalasComPistas(hallEntrada, &arvorePistas);

    printf("\n📜 PISTAS COLETADAS:\n");
    if (arvorePistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(arvorePistas);
    }

    printf("\nASSOCIAÇÕES PISTA → SUSPEITO:\n");
    listarAssociacoes();

    mostrarSuspeitoMaisCitado();
    verificarSuspeitoFinal();

    freePistaTree(arvorePistas);
    freeTabelaHash();
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
    salaPai->esquerda = salaEsquerda;
    salaPai->direita = salaDireita;
}

// ============================================================
//  EXPLORAÇÃO
// ============================================================

/**
 * @brief Permite ao jogador explorar as salas e coletar pistas.
 * @param salaAtual Ponteiro para a sala atual.
 * @param arvorePistas Ponteiro para a árvore de pistas coletadas.
 */
void explorarSalasComPistas(Sala* salaAtual, PistaNode** arvorePistas) {
    char opcao;

    while (salaAtual != NULL) {
        printf("\nVocê está em: %s\n", salaAtual->nome);

        if (strlen(salaAtual->pista) > 0) {
            printf("🕵️ PISTA ENCONTRADA: %s\n", salaAtual->pista);

            *arvorePistas = inserirPista(*arvorePistas, salaAtual->pista);

            // ASSOCIAÇÃO AUTOMÁTICA COM SUSPEITOS
            if (strstr(salaAtual->pista, "Livro"))
                inserirNaHash(salaAtual->pista, "Mordomo");
            else if (strstr(salaAtual->pista, "Faca"))
                inserirNaHash(salaAtual->pista, "Cozinheiro");
            else if (strstr(salaAtual->pista, "Carta"))
                inserirNaHash(salaAtual->pista, "Herdeira");
            else if (strstr(salaAtual->pista, "Pegadas"))
                inserirNaHash(salaAtual->pista, "Jardineiro");

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
//  TABELA HASH
// ============================================================

/**
 * @brief Inicializa a tabela hash.
 */
void inicializarHash() {
    for (int i = 0; i < TAM_HASH; i++)
        tabelaHash[i] = NULL;
}


/**
 * @brief Função de hash simples baseada na soma dos valores ASCII.
 * @param chave Chave para calcular o índice.
 * @return Índice na tabela hash.
 */
int funcaoHash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i]; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}

/**
 * @brief Insere uma associação pista → suspeito na tabela hash.
 * @param pista Texto da pista.
 * @param suspeito Nome do suspeito.
 */
void inserirNaHash(const char* pista, const char* suspeito) {
    int indice = funcaoHash(pista);

    HashNode* atual = tabelaHash[indice];
    while (atual) {
        // Evita duplicar a MESMA pista
        if (strcmp(atual->pista, pista) == 0) {
            return; // Pista já cadastrada
        }

        // Se o suspeito já existe, incrementa o contador
        if (strcmp(atual->suspeito, suspeito) == 0) {
            atual->contador++;
            return;
        }
        atual = atual->prox;
    }

    HashNode* novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->contador = 1;
    novo->prox = tabelaHash[indice];
    tabelaHash[indice] = novo;
}


/**
 * @brief Encontra o suspeito associado a uma pista.
 * @param pista Texto da pista.
 * @return Nome do suspeito ou NULL se não encontrado.
 */
char* encontrarSuspeito(const char* pista) {
    int indice = funcaoHash(pista);
    HashNode* atual = tabelaHash[indice];

    while (atual) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }

    return NULL;
}


/**
 * @brief Lista todas as associações pista → suspeito na tabela hash.
 */
void listarAssociacoes() {
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode* atual = tabelaHash[i];
        while (atual) {
            printf("%s → %s (%d pistas)\n",
                   atual->pista, atual->suspeito, atual->contador);
            atual = atual->prox;
        }
    }
}


/**
 * @brief Exibe o suspeito mais citado baseado nas pistas coletadas.
 */
void mostrarSuspeitoMaisCitado() {
    int maior = 0;
    char nome[50] = "";

    for (int i = 0; i < TAM_HASH; i++) {
        HashNode* atual = tabelaHash[i];
        while (atual) {
            if (atual->contador > maior) {
                maior = atual->contador;
                strcpy(nome, atual->suspeito);
            }
            atual = atual->prox;
        }
    }

    if (maior > 0)
        printf("\n🏆 SUSPEITO MAIS CITADO: %s (%d pistas)\n", nome, maior);
}


/**
 * @brief Verifica e exibe o suspeito mais provável baseado nas pistas coletadas.
 */
void verificarSuspeitoFinal() {
    char acusado[50];
    printf("\n⚖️ Quem você acusa? ");
    fgets(acusado, 50, stdin);
    acusado[strcspn(acusado, "\n")] = '\0';

    for (int i = 0; i < TAM_HASH; i++) {
        HashNode* atual = tabelaHash[i];
        while (atual) {
            if (strcmp(atual->suspeito, acusado) == 0) {
                if (atual->contador >= 2)
                    printf("CULPADO CONFIRMADO!\n");
                else
                    printf("Provas insuficientes.\n");
                return;
            }
            atual = atual->prox;
        }
    }

    printf("Nenhuma pista contra esse suspeito.\n");
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


/**
 * @brief Libera a memória alocada para a tabela hash.
 */
void freeTabelaHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode* atual = tabelaHash[i];
        while (atual) {
            HashNode* temp = atual;
            atual = atual->prox;
            free(temp);
        }
        tabelaHash[i] = NULL;
    }
}