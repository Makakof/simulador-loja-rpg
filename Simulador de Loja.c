#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>


typedef struct item
{
    char nome[45];
    int quantidade;
    int preco;
    struct item *next;
}item;

int lancamento(int cap)
{
    int resultado;
    resultado = rand();
    resultado = resultado % cap;

    return resultado;
}

void insertAtBegin(item **list,item *nova)
{
    nova->next = NULL;

    if(*list == NULL)
    {
        *list = nova;
    }
    else
    {
        nova->next = *list;
        *list = nova;
    }
}


void loadFileTxt(item **list)
{
    FILE *file;
    item *new;
    file = fopen("disponivel.txt","r");
    if(file == NULL)
    {
        printf("Erro ao abrir aquivo disponível.txt!\n");
        return 1;
    }
    while(!feof(file))
    {
        new = (item *) malloc(sizeof(item));
        fscanf(file,"%[^\n]\n",new->nome);
        new->next = NULL;
        insertAtBegin(list,new);
    }
    fclose(file);
}

void defineQuantidade(item **list)
{
    item *q;
    q = *list;

    while(q != NULL)
    {
        q->quantidade = lancamento(6) + 1;
        q = q->next;
    }
}

void definePreco(item **list)
{
    item *q;
    q = *list;

    while(q != NULL)
    {
        q->preco = lancamento(10) + 6;
        q = q->next;
    }
}

void showList(item *list)
{
    item *q;
    printf("\n");
    for(q = list;q != NULL;q = q->next)
    {
        printf("%d x %s sendo %d moedas de cobre p/u \n",q->quantidade, q->nome,q->preco);
    }
    printf("\n");
}



int menuPrincipal()
{
    printf("Ola Aventureiro, Bem-vindo ao Leilao\n");
    printf("1)Comprar item\n");
    printf("2)Vender item\n");
    printf("3)Loja Misteriosa\n");
    printf("4)Sair\n");
    printf("O que deseja? ");
}

void realizaCompra(item **list, char opcaoDeItem[])
{
    item *q;
    int quantidadeDesejada;
    q = *list;

    while(q != NULL && strcmp(opcaoDeItem, q->nome) != 0)
    {
        q = q->next;
    }

    if(q != NULL)
    {
        printf("Quantos(as) %s deseja comprar? ",q->nome);
        scanf("%d",&quantidadeDesejada);
        if(quantidadeDesejada <= q->quantidade)
        {
            q->quantidade = q->quantidade - quantidadeDesejada;
            printf("Item(ns) comprado(s) num total de: %d moedas de cobre! \n",quantidadeDesejada*q->preco);
        }
        else
        {
            printf("Você digitou uma quantidade alem da que esta disponivel! \n");
        }
    }
    else
    {
        printf("O item desejado nao existe ou você digitou errado! \n");
    }
}

void realizaVenda(item **list, char opcaoDeItem[])
{
    item *q;
    int quantidadeDesejada;
    q = *list;

    while(q != NULL && strcmp(opcaoDeItem, q->nome) != 0)
    {
        q = q->next;
    }

    if(q != NULL)
    {
        printf("Quantos(as) %s deseja vender? ",q->nome);
        scanf("%d",&quantidadeDesejada);
        if(quantidadeDesejada <= q->quantidade)
        {
            q->quantidade = q->quantidade - quantidadeDesejada;
            printf("Item(ns) vendido(s) num total de: %d moedas de cobre \n",quantidadeDesejada*q->preco);
        }
        else
        {
            printf("Voce digitou uma quantidade alem da desejada! \n");
        }
    }
    else
    {
        printf("O item a ser vendido nao existe ou voce digitou errado! \n");
    }
}

int contaItens(item *list)
{
    item *q;
    int contador = 0;
    for(q = list;q != NULL;q = q->next)
    {
        contador++;
    }
    return contador;
}


void copiaLista(item *list, item **novaLista)
{
    item *q, *nova;
    for(q = list; q != NULL; q = q->next)
    {
        nova = (item *) malloc(sizeof(item));
        nova->next = NULL;
        *nova = *q;
        insertAtBegin(novaLista,nova);
    }
}

void selecionaItens(item *list, item **novaLista)
{
    item *q, *nova;
    int contador = 0;
    q = list;
    while(q != NULL && contador <= 10)
    {
        // Gera um número aleatório entre 0 e 1
        double randomValue = (double)rand() / RAND_MAX;

        // Calcula a probabilidade de selecionar o nó atual
        double probability = (double)lancamento(10) / (contador + 1);
        if (randomValue < probability) 
        {
            nova = (item *) malloc(sizeof(item));
            nova->next = NULL;
            *nova = *q;
            insertAtBegin(novaLista,nova);
            contador++; 
        }
        q = q->next;
    }
}

int main(void)
{

    setlocale(LC_ALL, "Portuguese");
    int sair, opcao, segundos;
    sair = 0;
    char opcaoDeItem[30];
    item *listaComTodosItens = NULL;
    item *ItensParaCompra = NULL;
    item *itensAVenda = NULL;

    segundos = time(0);
    srand(segundos);

    loadFileTxt(&listaComTodosItens);
    defineQuantidade(&listaComTodosItens);
    definePreco(&listaComTodosItens);
    selecionaItens(listaComTodosItens,&itensAVenda);
    selecionaItens(listaComTodosItens,&ItensParaCompra);

    do
    {
        printf("\n");
        menuPrincipal();
        scanf("%d",&opcao);

        switch(opcao)
        {
            case 1:
                printf("Os itens a venda sao: \n");
                showList(itensAVenda);
                printf("O que deseja comprar? ");
                scanf(" %[^\n]",opcaoDeItem);
                realizaCompra(&itensAVenda,opcaoDeItem);
                printf("\n");
            break;
            case 2:
                printf("Os itens desejados sao: \n");
                showList(ItensParaCompra);
                printf("O que deseja vender? ");
                scanf(" %[^\n]",opcaoDeItem);
                realizaVenda(&ItensParaCompra,opcaoDeItem);
                printf("\n");
            break;
            case 3:
                printf("O viajante ainda não chegou, volte mais tarde!");
                printf("\n");
            break;
            case 4:
                sair = 1;
            break;
            default:
                printf("Você digitou uma opcao invalida, tente de novo! \n");
            break;
        }
    }while(!sair);
    system("pause");    
    return 0;
}