#include <stdio.h>
#include <stdlib.h> 

typedef struct NoAresta{
    int idVizinho;
    struct NoAresta* proximo;
} NoAresta;

typedef struct NoVertice{
    int id;
    NoAresta* listaArestas;
    struct NoVertice* proximo;
} NoVertice;

typedef struct{
    NoVertice* inicio;
    int qtdVertice;
    int qtdAresta;
} Grafo;

Grafo* criarGrafo();
NoVertice* buscarVertice(Grafo* g, int id);
int existeAresta(NoVertice* v, int idVizinho);
int inserirVertice(Grafo* g, int id);
int inserirAresta(Grafo* g, int v1, int v2);
void removerArestaUnidirecional(NoVertice* vert, int idRemover);
int removerAresta(Grafo* g, int v1, int v2);
int removerVertice(Grafo* g, int id);
void visualizarGrafo(Grafo* g);
void liberarGrafo(Grafo* g);
void carregarGrafoExemplo(Grafo* g);
void menu(Grafo* g);

Grafo* criarGrafo(){
    Grafo* g = (Grafo*) malloc(sizeof(Grafo));
    if (g == NULL)
        return NULL;
    
        g->inicio = NULL;
        g->qtdVertice = 0;
        g->qtdAresta = 0;

        return g;
}

NoVertice* buscarVertice(Grafo* g, int id){
    NoVertice* atual = g->inicio;
    while(atual != NULL){
        if(atual->id == id){
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

int inserirVertice(Grafo* g, int id){
    if(buscarVertice(g,id) != NULL){
        printf("Vertice %d ja inserido no grafo\n", id);
        return 0;
    }

    NoVertice* novo = (NoVertice*) malloc (sizeof(NoVertice));
    if(novo == NULL){
        printf("Erro de alocacao de memoria\n");
        return 0;
    }

    novo->id = id;
    novo->listaArestas = NULL;
    novo->proximo = g->inicio;

    g->inicio = novo;
    g->qtdVertice ++;

    return 1;
}

int existeAresta(NoVertice* v, int idVizinho){
    NoAresta* atual = v->listaArestas;
    while (atual != NULL){
        if(atual->idVizinho == idVizinho){
            return 1;
        }
        atual = atual->proximo;
    }
    return 0;
}

int inserirAresta(Grafo* g, int v1, int v2){
    NoVertice* vert1 = buscarVertice(g, v1);
    NoVertice* vert2 = buscarVertice(g, v2);

    if(vert1 == NULL){
        printf("O primeiro vertice nao existe\n");
        return 0;
    }

    if(vert2 == NULL){
        printf("O segundo vertice nao existe\n");
        return 0;
    }

    if(existeAresta(vert1, v2)){
        printf("Aresta (%d,%d) ja existe", v1, v2);
        return 0;
    }
    
    NoAresta* nova1 = (NoAresta*) malloc(sizeof(NoAresta));
    if(nova1==NULL)
        return 0;
    nova1->idVizinho = v2;
    nova1->proximo = vert1->listaArestas;
    vert1->listaArestas = nova1;

    NoAresta* nova2 = (NoAresta*) malloc(sizeof(NoAresta));
    if(nova2==NULL)
        return 0;
    nova2->idVizinho = v1;
    nova2->proximo = vert2->listaArestas;
    vert2->listaArestas = nova2;

    g->qtdAresta++;

    return 1;
}

void visualizarGrafo(Grafo* g){
    if(g== NULL || g->inicio == NULL){
        printf("\nGrafo vazio\n");
        return;
    }

    printf("\nGrafo (V: %d, A: %d):\n", g->qtdVertice, g->qtdAresta);
    NoVertice* VAtual = g->inicio;

    while(VAtual != NULL){
        printf("Vertice %d: ", VAtual->id);

        NoAresta* AAtual = VAtual->listaArestas;
        if(AAtual == NULL){
            printf("Sem vizinhos");
        } else {
            while(AAtual !=NULL){
                printf("-> %d", AAtual->idVizinho);
                AAtual = AAtual->proximo;
            }
        }
        printf("-> NULL\n\n");

        VAtual = VAtual->proximo;
    }
}

void removerArestaUnidirecional(NoVertice* vert, int idRemover){
    if(vert == NULL || vert->listaArestas == NULL)
        return;

    NoAresta* atual = vert->listaArestas;
    NoAresta* anterior = NULL;

    while (atual != NULL){
        if(atual->idVizinho == idRemover){
            if(anterior == NULL){
                vert->listaArestas = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
}

int removerVertice(Grafo* g, int id){
    if(g==NULL || g->inicio == NULL){
        printf("Grafo vazio");
        return 0;
    }

    NoVertice* vertRemover = buscarVertice(g, id);
    if(vertRemover == NULL){
        printf("Vertice %d nao encontrado no grafo.\n", id);
        return 0;
    }

    int arestasRemovidas = 0;
    NoAresta* auxCont = vertRemover->listaArestas;
    while (auxCont != NULL) {
        arestasRemovidas++;
        auxCont = auxCont->proximo;
    }

    NoAresta * arestaAtual = vertRemover->listaArestas;
    while(arestaAtual != NULL){
        NoVertice* vizinho = buscarVertice(g, arestaAtual->idVizinho);
        if (vizinho != NULL){
            removerArestaUnidirecional(vizinho, id);
        }
        arestaAtual = arestaAtual->proximo;
    }

    arestaAtual = vertRemover->listaArestas;
    while(arestaAtual != NULL){
        NoAresta* aux = arestaAtual;
        arestaAtual = arestaAtual->proximo;
        free(aux);
    }

    NoVertice* verticeAtual = g->inicio;
    NoVertice* verticeAnterior = NULL;

    while (verticeAtual != NULL && verticeAtual-> id != id) {
        verticeAnterior = verticeAtual;
        verticeAtual = verticeAtual->proximo;
    }

    if(verticeAnterior == NULL){
        g->inicio = verticeAtual->proximo;
    } else {
        verticeAnterior->proximo = verticeAtual->proximo;
    }

    free(verticeAtual);
    g->qtdVertice--;
    g->qtdAresta -= arestasRemovidas;

    return 1;
}

int removerAresta(Grafo* g, int v1, int v2){
    if(g==NULL||g->inicio == NULL){
        printf("Grafo vazio");
        return 0;
    }

    NoVertice* vert1 = buscarVertice(g, v1);
    NoVertice* vert2 = buscarVertice(g, v2);

    if(vert1 == NULL){
        printf("O primeiro vertice nao existe\n");
        return 0;
    }

    if(vert2 == NULL){
        printf("O segundo vertice nao existe\n");
        return 0;
    }

    if(!existeAresta(vert1, v2)){
        printf("Aresta (%d,%d) nao existe", v1, v2);
        return 0;
    }

    removerArestaUnidirecional(vert1, v2);
    removerArestaUnidirecional(vert2, v1);

    g->qtdAresta--;

    return 1;
}

void liberarGrafo(Grafo* g){
    if(g == NULL)
        return;

    NoVertice* verticeAtual = g->inicio;

    while(verticeAtual != NULL){
        NoAresta* arestaAtual = verticeAtual->listaArestas;
        while(arestaAtual!= NULL){
            NoAresta* auxA = arestaAtual;
            arestaAtual = arestaAtual->proximo;
            free(auxA);
        }
        NoVertice* auxV = verticeAtual;
        verticeAtual = verticeAtual->proximo;
        free(auxV);
    }    
    free(g);
}

void carregarGrafoExemplo(Grafo* g) {
    for (int i = 1; i <= 9; i++) {
        inserirVertice(g, i);
    }
    inserirAresta(g, 1, 2);
    inserirAresta(g, 1, 6);
    inserirAresta(g, 2, 3);
    inserirAresta(g, 2, 4);
    inserirAresta(g, 3, 5);
    inserirAresta(g, 4, 5);
    inserirAresta(g, 4, 6);
    inserirAresta(g, 4, 7);
    inserirAresta(g, 7, 8);
    inserirAresta(g, 7, 9);
}


void menu(Grafo *g){
    int opt;
    int v1, v2;
    do{
        printf("\nEscolha um das seguintes opcoes:\n");
        printf("1 - Inserir vertices\n");
        printf("2 - Inserir arestas\n");
        printf("3 - Visualizar grafo\n");
        printf("4 - Remover vertices\n");
        printf("5 - remover arestas\n");
        printf("6 - sair\n");
        printf("\n: ");
        if (scanf("%d", &opt) != 1) {
            while (getchar() != '\n');
            printf("Entrada invalida\n");
            continue;
        }
        switch(opt){
            case 1:
                printf("-Inserir vertices:\n\n");
                printf("Insira o id do vertice: ");
                scanf("%d", &v1);
                if(inserirVertice(g, v1)){
                    printf("%d inserido", v1);
                }
                break;
            case 2:
                printf("-Inserir aresta:\n\n");
                printf("Insira o id do vertice de origem: ");
                scanf("%d", &v1);
                printf("Insira o id do vertice de destino: ");
                scanf("%d", &v2);
                inserirAresta(g, v1, v2);
                break;
            case 3:
                visualizarGrafo(g);
                break;
            case 4:
                printf("-Remover vertice:\n\n");
                printf("Insira o id do vertice para remover: ");
                scanf("%d", &v1);
                removerVertice(g, v1);
                break;
            case 5:
                printf("-Remover Aresta:\n\n");
                printf("Digite o ID do primeiro vertice: ");
                scanf("%d", &v1);
                printf("Digite o ID do segundo vertice: ");
                scanf("%d", &v2);
                removerAresta(g, v1, v2);
                break;
            case 6:
                liberarGrafo(g);
                break;
            default:
                printf("Opcao invalida, tente novamente. \n");
                break;
        }

    }while(opt!=6);
}

int main(void){

    Grafo * g = criarGrafo();
    if(g == NULL){
        return 1;
    }
    carregarGrafoExemplo(g);
    menu(g);

    return 0;
}