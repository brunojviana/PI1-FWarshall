#include <stdio.h>
#include <stdlib.h>
#define MAXNODES 1000

typedef struct node
{
    int id;
} node;

typedef struct arco
{
    struct node *origem;
    struct node *destino;
    int adj;
    int peso;
} arco;

typedef struct grafo
{
    struct node *nodes[MAXNODES];
    struct arco *matriz_arcos[MAXNODES][MAXNODES];
} grafo;

grafo *cria_grafo();
node *cria_node(int i);
void preenche_matriz(grafo *g);
arco *cria_arco(grafo *g, node *x, node *y, int modo);
void consultar_adjacencia(grafo *g);

int main()
{
    grafo *grafo_g;
    int sair=0;

    grafo_g = cria_grafo();
    preenche_matriz(grafo_g);
    consultar_adjacencia(grafo_g); 

    return 0;
}

grafo *cria_grafo()
{
    grafo *novo_grafo;
    int i, j;

    novo_grafo = (grafo*)malloc(sizeof(grafo));
    
    for (i=0; i<MAXNODES; i++)
    {
        novo_grafo->nodes[i] = cria_node(i);

        for (j=0; j<MAXNODES; j++)
        {
            novo_grafo->matriz_arcos[i][j] = cria_arco(novo_grafo, novo_grafo->nodes[i], novo_grafo->nodes[j], 1);
        }
    }
    return novo_grafo;
}

node *cria_node(int i)
{
    node *novo_node;

    novo_node = (node*)malloc(sizeof(node));
    novo_node->id = i;
    return novo_node;
}

void preenche_matriz(grafo *g)
{
    int qtd_arcos = 0;
    int id_origem, id_destino;

    while (qtd_arcos < MAXNODES * MAXNODES)
    {
        id_origem = rand() % MAXNODES;
        id_destino = rand() % MAXNODES;   
        g->matriz_arcos[id_origem][id_destino] = cria_arco(g, g->nodes[id_origem], g->nodes[id_destino], 2);
        qtd_arcos++;
    }
}

arco *cria_arco(grafo *g, node *x, node *y, int modo)
{
    arco *novo_arco;

    novo_arco = (arco*)malloc(sizeof(arco));
    novo_arco->origem = x;
    novo_arco->destino = y;
    if (modo == 1)
    {
        novo_arco->adj = 0;
        novo_arco->peso = 0;
    }
    else
    {
        novo_arco->adj = 1;
        novo_arco->peso = rand() % 100;
    }
    return novo_arco;
}

void consultar_adjacencia(grafo *g)
{
    int node1, node2;

    do{
        printf("Node Origem: ");
        scanf("%d",&node1);
        if (node1 == -1)
        {
            break;
        }
        printf("Node Destino: ");
        scanf("%d",&node2);

        if (g->matriz_arcos[node1][node2]->adj)
        {
            printf("\nAdjacente!\n");
            printf("\nArco (%d,%d) - Peso: %d\n\n", node1, node2, g->matriz_arcos[node1][node2]->peso);
        }
        else
        {
            printf("\nNode %d não é adjacente ao Node %d!\n\n", node2, node1);
        }
    } while (node1 != -1);
}