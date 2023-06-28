#include <stdio.h>
#include <stdlib.h>


/*
    inserir elementos na arvore binaria..

*/
typedef struct no{
    int valor;
    struct no *direita;
    struct no *esquerda;

}NoArv;

NoArv* inserir_varsao_1(NoArv *raiz, int num){
    //verificar se a raiz é nula
    if(raiz == NULL){
        NoArv *aux = malloc(sizeof(NoArv));
        aux->valor = num;
        aux->esquerda = NULL;
        aux->direita = NULL;
        return aux;

    }else{
        if(num < raiz->valor){
            raiz->esquerda = inserir_varsao_1(raiz->esquerda,num);
        }else{
            raiz->direita = inserir_varsao_1(raiz->direita,num);
        }
        return raiz;
    }

}

void imprimir_versao_1(NoArv *raiz){   
    
    if(raiz){
        printf("%d ",raiz->valor);
        imprimir_versao_1(raiz->esquerda);
        imprimir_versao_1(raiz->direita);
    }

}

void imprimir_versao_2(NoArv *raiz){   
    
    if(raiz){
        imprimir_versao_2(raiz->esquerda);
        printf("%d ",raiz->valor);
        imprimir_versao_2(raiz->direita);
    }

}



int main(){

    NoArv *raiz = NULL; 

    raiz = inserir_varsao_1(raiz,50);
    raiz = inserir_varsao_1(raiz,25);
    raiz = inserir_varsao_1(raiz,100);
    raiz = inserir_varsao_1(raiz,30);
    printf("---- ---primeiro caso  -------------\n");
    imprimir_versao_1(raiz);
    printf("\n------------------------------------");
     printf("\n---- ---segundo caso ordenado  -------------\n");
    imprimir_versao_2(raiz);
    printf("\n------------------------------------");
    int opcao,valor;
    do{
        printf("\n0 - sair\n1 - inserir\n2 - imprimir\n");
        scanf("%d",&opcao);
        switch (opcao)
        {
        case 1:
            printf("digite um valor: \n");
            scanf("%d",&valor);
            raiz = inserir_varsao_1(raiz, valor);
            break;
        case 2:
            printf("\n-----versão 1 ---------- \n");
            imprimir_versao_1(raiz);
            printf("\n-------- versão 2 -----------\n");
            imprimir_versao_2(raiz);
            break;
        default:
            printf("opção invalida!!!");
            break;
        }


    }while(opcao != 0);


    return 0;
}