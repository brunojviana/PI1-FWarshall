#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#define MAXNODES 1000
#define MAXINT INT_MAX

typedef struct node
{
    int id;
    int grau_in;
    int grau_out;
    int grau_total;
    int soma_das_distancias;
    float media_das_distancias;
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
    struct arco *matriz_adj[MAXNODES][MAXNODES];
} grafo;

typedef struct matriz_dist
{
    int distancia[MAXNODES][MAXNODES];
    struct node *predecessor[MAXNODES][MAXNODES];
} matriz_dist;

grafo *cria_grafo();
node *cria_node(int i);
arco *cria_arco(grafo *g, node *x, node *y);
matriz_dist *floyd_warshall(grafo *g);
void consultar_distancia(matriz_dist *m);
void calcula_centralidade_proximidade(grafo *g, matriz_dist *m);
void imprime_matriz_adj(grafo *g);
void imprime_matriz_pesos(grafo *g);
void imprime_matriz_distancias(matriz_dist *m);
void preenche_adjacencias(grafo *g);

int main()
{
    grafo *grafo_g;
    matriz_dist *matriz_distancias;
    int opcao=0;

    grafo_g = cria_grafo();
    preenche_adjacencias(grafo_g);
    matriz_distancias = floyd_warshall(grafo_g);
    
    do{

        printf("\nOpção 1 - Imprimir Matriz de Adjacências");
        printf("\nOpção 2 - Imprimir Matriz de Pesos");
        printf("\nOpção 3 - Imprimir Matriz de Distâncias");
        printf("\nOpção 4 - Calcular Centralidade");
        printf("\nOpção 5 - Consultar Distâncias");
        printf("\nOpção -1 - Sair\n");
        printf("\nOpção Desejada: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            imprime_matriz_adj(grafo_g);
            break;
        case 2:
            imprime_matriz_pesos(grafo_g);
            break;
        case 3:
            imprime_matriz_distancias(matriz_distancias);
            break;
        case 4:
            calcula_centralidade_proximidade(grafo_g, matriz_distancias);
            break;
        case 5:
            consultar_distancia(matriz_distancias);
            break;
        case -1:
            break;
        default:
            printf("\nOpção Invalida.\n");
            break;
        }
    } while (opcao != -1);

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
    }
    for (i=0; i<MAXNODES; i++)
    {
        for (j=0; j<MAXNODES; j++)
        {
            novo_grafo->matriz_adj[i][j] = cria_arco(novo_grafo, novo_grafo->nodes[i], novo_grafo->nodes[j]);
        }
    }        
    return novo_grafo;
}

node *cria_node(int i)
{
    node *novo_node;

    novo_node = (node*)malloc(sizeof(node));
    novo_node->id = i;
    novo_node->grau_in = 0;
    novo_node->grau_out = 0;
    novo_node->grau_total = 0;
    novo_node->soma_das_distancias = 0;
    novo_node->media_das_distancias = 0;
    return novo_node;
}

arco *cria_arco(grafo *g, node *x, node *y)
{
    arco *novo_arco;

    novo_arco = (arco*)malloc(sizeof(arco));
    novo_arco->origem = x;
    novo_arco->destino = y;
    novo_arco->adj = 0;
    novo_arco->peso = MAXINT/2;
    return novo_arco;
}

matriz_dist *floyd_warshall(grafo *g)
{
    int i, j, k;
    matriz_dist *nova_dist;

    nova_dist = (matriz_dist*)malloc(sizeof(matriz_dist));

    for (i=0; i<MAXNODES; i++)
    {
        for(j=0; j<MAXNODES; j++)
        {
            if (i != j && g->matriz_adj[i][j]->adj == 1)
            {
                nova_dist->distancia[i][j] = g->matriz_adj[i][j]->peso;
                nova_dist->predecessor[i][j] = g->matriz_adj[i][j]->origem;
            }
            else
            {
                nova_dist->distancia[i][j] = MAXINT/2;
                nova_dist->predecessor[i][j] = NULL;
            }
            if (i != j && nova_dist->distancia[i][j] < MAXINT/2)
            {
                for(k=0; k<MAXNODES; k++)
                {
                    if (nova_dist->distancia[i][j] > g->matriz_adj[i][k]->peso + g->matriz_adj[k][j]->peso)
                    {
                        nova_dist->distancia[i][j] = g->matriz_adj[i][k]->peso + g->matriz_adj[k][j]->peso;
                        nova_dist->predecessor[i][j] = g->matriz_adj[k][j]->origem;
                    }
                }
            }
        }
    }  
    return nova_dist;
}

void consultar_distancia(matriz_dist *m)
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

        if (m->distancia[node1][node2] != MAXINT/2)
        {
            printf("\nArco: (%d,%d)", node1, node2);
            printf("\nDistância: %d", m->distancia[node1][node2]);
            printf("\nPredecessor: Node %d\n\n", m->predecessor[node1][node2]->id);
        }
        else
        {
            printf("\nNão há uma sequência de arcos que ligam o Node %d ao Node %d.\n\n", node1, node2);
        }
    } while (node1 != -1);
}

void calcula_centralidade_proximidade(grafo *g, matriz_dist *m)
{
    int i, j;

    node *node_central;

    for (i=0; i<MAXNODES; i++)
    {
        for (j=0; j<MAXNODES; j++)
        {
            if (i != j && m->distancia[i][j] < MAXINT/2)
            {
                g->nodes[i]->soma_das_distancias = g->nodes[i]->soma_das_distancias + m->distancia[i][j];
            }
        }
        g->nodes[i]->media_das_distancias = g->nodes[i]->soma_das_distancias / g->nodes[i]->grau_total;
    }

    node_central = g->nodes[0];

    for (i=0; i<MAXNODES; i++)
    {
        if (g->nodes[i]->media_das_distancias < node_central->media_das_distancias)
        {
            node_central = g->nodes[i];
        }
    }
    printf("\nO Node %d é o de maior centralidade.\n", node_central->id);
    printf("Número de Arestas: %d\n", node_central->grau_total);
    printf("Média das Distâncias: %.2f\n\n", node_central->media_das_distancias);
}

void imprime_matriz_adj(grafo *g)
{
    int i, j;
    FILE *arq;

    arq = fopen("Matriz_Adjacencias.txt", "w+");

    for (i=0; i<MAXNODES; i++)
    {
        for (j=0; j<MAXNODES; j++)
        {
            fprintf(arq,"%d,", g->matriz_adj[i][j]->adj);
        }
        fprintf(arq,"\n");
    }
    fclose(arq);
}

void imprime_matriz_pesos(grafo *g)
{
    int i, j;
    FILE *arq;

    arq = fopen("Matriz_Pesos.txt", "w+");

    for (i=0; i<MAXNODES; i++)
    {
        for (j=0; j<MAXNODES; j++)
        {
            if (g->matriz_adj[i][j]->peso == MAXINT/2)
            {
                fprintf(arq, "INF,");
            }
            else
            {
                fprintf(arq,"%d,", g->matriz_adj[i][j]->peso);
            }
        }
        fprintf(arq,"\n");
    }
    fclose(arq);
}

void imprime_matriz_distancias(matriz_dist *m)
{
    int i, j;
    FILE *arq;

    arq = fopen("Matriz_Distancias.txt", "w+");
    
    for (i=0; i<MAXNODES; i++)
    {
        for (j=0; j<MAXNODES; j++)
        {
            if (m->distancia[i][j] == MAXINT/2)
            {
                fprintf(arq, "INF,");
            }
            else
            {
                fprintf(arq,"%d,", m->distancia[i][j]);    
            }
        }
        fprintf(arq,"\n");
    }
    fclose(arq);
}

void preenche_adjacencias(grafo *g)
{
    int qtd_arcos = 0;
    int id_origem, id_destino;

    while (qtd_arcos < MAXNODES * MAXNODES)
    {
        id_origem = rand() % MAXNODES;
        id_destino = rand() % MAXNODES;

        if (id_origem != id_destino)
        {
            if (id_origem <= 100)
            {
                if (id_destino <= 100)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 1 + (rand() % 10);
                }
                else if (id_destino > 100 && id_destino <= 200)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 2 * (10 + (rand() % 10));
                }
                else if (id_destino > 200 && id_destino <= 300)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 3 * (10 + (rand() % 10));
                }
                else if (id_destino > 300 && id_destino <= 400)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 4 * (10 + (rand() % 10));
                }
                else if (id_destino > 400 && id_destino <= 500)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 5 * (10 + (rand() % 10));
                }
                else
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 10 * (10 + (rand() % 10));
                }                                                
            }
            else if (id_origem >= 100 && id_origem < 200)
            {
                if (id_destino <= 100)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 3 * (rand() % 10);
                }
                else if (id_destino > 100 && id_destino <= 200)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 2 * (10 + (rand() % 10));
                }
                else if (id_destino > 200 && id_destino <= 300)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 3 * (10 + (rand() % 10));
                }
                else if (id_destino > 300 && id_destino <= 400)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 4 * (10 + (rand() % 10));
                }
                else if (id_destino > 400 && id_destino <= 500)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 5 * (10 + (rand() % 10));
                }
                else
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 10 * (10 + (rand() % 10));
                } 
            }
            else if (id_origem >= 200 && id_origem < 300)
            {
                if (id_destino <= 200)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 4 * (10 + rand() % 10);
                }
                else if (id_destino > 200 && id_destino <= 300)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 2 * (10 + (rand() % 10));
                }
                else if (id_destino > 300 && id_destino <= 400)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 3 * (10 + (rand() % 10));
                }
                else if (id_destino > 400 && id_destino <= 500)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 4 * (10 + (rand() % 10));
                }
                else if (id_destino > 500 && id_destino <= 600)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 5 * (10 + (rand() % 10));
                }
                else
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 10 * (10 + (rand() % 10));
                } 
            }            
            else if (id_origem >= 300 && id_origem < 400)
            {
                if (id_destino <= 300)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 5 * (10 + rand() % 10);
                }
                else if (id_destino > 300 && id_destino <= 400)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 2 * (10 + (rand() % 10));
                }
                else if (id_destino > 400 && id_destino <= 500)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 3 * (10 + (rand() % 10));
                }
                else if (id_destino > 500 && id_destino <= 600)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 4 * (10 + (rand() % 10));
                }
                else if (id_destino > 600 && id_destino <= 700)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 5 * (10 + (rand() % 10));
                }
                else
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 10 * (10 + (rand() % 10));
                } 
            }
            else if (id_origem >= 400 && id_origem < 500)
            {
                if (id_destino <= 400)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 6 * (10 + rand() % 10); 
                }
                else if (id_destino > 400 && id_destino <= 500)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 2 * (10 + (rand() % 10));
                }
                else if (id_destino > 500 && id_destino <= 600)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 3 * (10 + (rand() % 10));
                }
                else if (id_destino > 600 && id_destino <= 700)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 4 * (10 + (rand() % 10));
                }
                else if (id_destino > 700 && id_destino <= 800)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 5 * (10 + (rand() % 10));
                }
                else
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 10 * (10 + (rand() % 10));
                } 
            }
            else
            {
                if (id_destino <= 500)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 10 * (10 + rand() % 10);
                }
                else if (id_destino > 500 && id_destino <= 600)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 1 * (10 + (rand() % 10));
                }
                else if (id_destino > 600 && id_destino <= 700)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 2 * (10 + (rand() % 10));
                }
                else if (id_destino > 700 && id_destino <= 800)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 3 * (10 + (rand() % 10));
                }
                else if (id_destino > 800 && id_destino <= 900)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 4 * (10 + (rand() % 10));
                }
                else if (id_destino > 900 && id_destino <= 1000)
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 5 * (10 + (rand() % 10));
                }
                else
                {
                    g->matriz_adj[id_origem][id_destino]->peso = 10 * (10 + (rand() % 10));
                } 
            }
            g->matriz_adj[id_origem][id_destino]->adj = 1;
            g->nodes[id_origem]->grau_in++;
            g->nodes[id_origem]->grau_total++;
            g->nodes[id_destino]->grau_out++;
            g->nodes[id_destino]->grau_total++; 
            qtd_arcos++;
        }   
    }
}

