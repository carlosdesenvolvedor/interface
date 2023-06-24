#include <stdio.h>
#include <stdlib.h>

#define tamanho 100
#define E 0
#define D 1
#define R -1

// Definição da estrutura do nó da árvore
struct str_no {
    char dado;
    int esquerda;
    int direita;
    int pai;
};

// Declaração do array de nós da árvore
struct str_no arvore[tamanho];

int lado, indice = 0;
int opt = -1;
char pai, no;

// Declaração de funções
void arvore_insere(int pai, char dado, int lado);
int arvore_procura(char dado);
void menu_mostrar(void);

int main(void) {
    int temp;
    do {
        menu_mostrar();  // Mostra o menu para o usuário
        scanf("%d", &opt);  // Lê a opção escolhida pelo usuário

        switch (opt) {
            case 1:
                printf("\nDigite o valor do PAI: ");
                scanf(" %c", &pai);
                printf("Digite o valor do NO: ");
                scanf(" %c", &no);
                printf("Digite o lado da subarvore (E=%d/D=%d/R=%d): ", E, D, R);
                scanf("%d", &lado);

                temp = arvore_procura(pai);  // Procura o nó pai na árvore
                arvore_insere(temp, no, lado);  // Insere o novo nó na árvore

                break;
            case 2:
                printf("Digite o valor do NO: ");
                scanf(" %c", &no);

                temp = arvore_procura(no);  // Procura o nó na árvore
                printf("No %c\nFilho Esquerda: %c\nFilho Direita: %c\n\n", arvore[temp].dado, arvore[arvore[temp].esquerda].dado, arvore[arvore[temp].direita].dado);

                system("pause");
                break;
        }
    } while (opt != 0);

    system("pause");
    return 0;
}

// Função para inserir um nó na árvore
void arvore_insere(int pai, char dado, int lado) {
    switch (lado) {
        case E:
            arvore[pai].esquerda = indice;  // Define o índice do filho esquerdo do nó pai
            arvore[indice].dado = dado;  // Define o dado do novo nó
            arvore[indice].pai = pai;  // Define o pai do novo nó
            arvore[indice].esquerda = -1;  // Define que o novo nó não possui filho esquerdo
            arvore[indice].direita = -1;  // Define que o novo nó não possui filho direito
            indice++;  // Incrementa o índice para a próxima inserção
            break;
        case D:
            arvore[pai].direita = indice;  // Define o índice do filho direito do nó pai
            arvore[indice].dado = dado;  // Define o dado do novo nó
            arvore[indice].pai = pai;  // Define o pai do novo nó
            arvore[indice].esquerda = -1;  // Define que o novo nó não possui filho esquerdo
            arvore[indice].direita = -1;  // Define que o novo nó não possui filho direito
            indice++;  // Incrementa o índice para a próxima inserção
            break;
        case R:
            arvore[indice].dado = dado;  // Define o dado do novo nó
            arvore[indice].pai = -1;  // Define que o novo nó é a raiz da árvore
            arvore[indice].esquerda = -1;  // Define que o novo nó não possui filho esquerdo
            arvore[indice].direita = -1;  // Define que o novo nó não possui filho direito
            indice++;  // Incrementa o índice para a próxima inserção
            break;
    }
}

// Função para procurar um nó na árvore
int arvore_procura(char dado) {
    if (indice != 0) {
        for (int i = 0; i < indice; i++) {
            if (arvore[i].dado == dado) {
                return i;  // Retorna o índice do nó encontrado
            }
        }
    } else {
        return 0;  // Retorna 0 se a árvore estiver vazia
    }
}

// Função para mostrar o menu na tela
void menu_mostrar(void) {
    system("cls");

    for (int i = 0; i < indice; i++) {
        printf("| %c ", arvore[i].dado);  // Mostra os nós existentes na árvore
    }

    printf("\n1 - Inserir um NO na arvore");
    printf("\n2 - Pesquisar um NO na arvore");
    printf("\n0 - Sair...\n\n");
}