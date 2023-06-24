#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cliente {
    char nome[50];
    char cpf[15];
    char endereco[100];
    struct Cliente *esquerda;
    struct Cliente *direita;
} Cliente;

Cliente *criarCliente(const char *nome, const char *cpf, const char *endereco) {
    Cliente *novoCliente = (Cliente *)malloc(sizeof(Cliente));
    strcpy(novoCliente->nome, nome);
    strcpy(novoCliente->cpf, cpf);
    strcpy(novoCliente->endereco, endereco);
    novoCliente->esquerda = NULL;
    novoCliente->direita = NULL;
    return novoCliente;
}

Cliente *inserirCliente(Cliente *raiz, Cliente *novoCliente) {
    if (raiz == NULL) {
        return novoCliente;
    }

    if (strcmp(novoCliente->nome, raiz->nome) < 0) {
        raiz->esquerda = inserirCliente(raiz->esquerda, novoCliente);
    } else {
        raiz->direita = inserirCliente(raiz->direita, novoCliente);
    }

    return raiz;
}

Cliente *buscarCliente(Cliente *raiz, const char *nome) {
    if (raiz == NULL || strcmp(nome, raiz->nome) == 0) {
        return raiz;
    }

    if (strcmp(nome, raiz->nome) < 0) {
        return buscarCliente(raiz->esquerda, nome);
    } else {
        return buscarCliente(raiz->direita, nome);
    }
}

void imprimirCliente(Cliente *cliente) {
    printf("Nome: %s\n", cliente->nome);
    printf("CPF: %s\n", cliente->cpf);
    printf("Endereço: %s\n", cliente->endereco);
}

void imprimirArvore(Cliente *raiz, int nivel) {
    if (raiz == NULL) {
        return;
    }

    imprimirArvore(raiz->direita, nivel + 1);

    for (int i = 0; i < nivel; i++) {
        printf("    ");
    }
    printf("%s\n", raiz->nome);

    imprimirArvore(raiz->esquerda, nivel + 1);
}

void liberarArvore(Cliente *raiz) {
    if (raiz == NULL) {
        return;
    }

    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

int main() {
    Cliente *arvore = NULL;
    int opcao;
    char nomeBusca[50];

    do {
        printf("==== Menu ====\n");
        printf("1. Cadastrar novo cliente\n");
        printf("2. Buscar cliente\n");
        printf("3. Ver árvore\n");
        printf("0. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        getchar();  // Limpar o buffer do teclado

        switch (opcao) {
            case 1:
                {
                    char nome[50];
                    char cpf[15];
                    char endereco[100];

                    printf("Digite o nome do cliente: ");
                    fgets(nome, 50, stdin);
                    nome[strlen(nome) - 1] = '\0';

                    printf("Digite o CPF do cliente: ");
                    fgets(cpf, 15, stdin);
                    cpf[strlen(cpf) - 1] = '\0';

                    printf("Digite o endereço do cliente: ");
                    fgets(endereco, 100, stdin);
                    endereco[strlen(endereco) - 1] = '\0';

                    Cliente *novoCliente = criarCliente(nome, cpf, endereco);
                    arvore = inserirCliente(arvore, novoCliente);

                    printf("Cliente cadastrado com sucesso!\n");
                    printf("=================\n");
                }
                break;
            case 2:
                printf("Digite o nome do cliente a ser buscado: ");
                fgets(nomeBusca, 50, stdin);
                nomeBusca[strlen(nomeBusca) - 1] = '\0';

                Cliente *clienteEncontrado = buscarCliente(arvore, nomeBusca);

                if (clienteEncontrado == NULL) {
                    printf("Cliente não encontrado.\n");
                } else {
                    printf("Cliente encontrado:\n");
                    imprimirCliente(clienteEncontrado);
                }

                printf("=================\n");
                break;
            case 3:
                printf("Árvore de clientes:\n");
                imprimirArvore(arvore, 0);
                printf("=================\n");
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                printf("=================\n");
                break;
        }
    } while (opcao != 0);

    liberarArvore(arvore);

    return 0;
}
