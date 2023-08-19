#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "lib.h"

int criar(TLista *L){
	int i=0;

	for(i=0;i<MAX;i++)
		L->elemento[i] = NULL;
	L->n = 0;
	return 1;
}

int inserir(TLista *L, int hash, Cliente cliente){
	Tno novo;
	novo = (Tno)malloc(sizeof(struct no));
	if(novo == NULL){
        return 0;
	}

	novo->cliente = cliente;
	novo->proximo = NULL;

	if(L->elemento[hash] == NULL)
		L->elemento[hash] = novo;
	else{
		novo->proximo = L->elemento[hash];
		L->elemento[hash] = novo;
	}
	L->n+=1;
	printf("Cliente inserido");
	return 1;
}

int hashing(int chave){
	return chave %MAX;
}


void exibir(TLista L){
	int i;
	Tno aux;
	if(L.n == 0){
		printf("Tabela vazia\n");
		return;
	}
	else{
		for(i=0; i<MAX; i++){
			if(L.elemento[i] != NULL){
				printf("--------- indice %d -------\n", i);
				aux = L.elemento[i];
				while(aux != NULL){
				printf("Mesa = %d \n", aux->cliente.mesa);
				printf("Nome = %s \n\n", aux->cliente.nome);
				aux = aux->proximo;
				}
			}
		}
	}
}

Cliente* buscarCliente(TLista* L, int hash, int chave) {
    Tno no = L->elemento[hash];
    while (no != NULL) {
        if (no->cliente.mesa == chave) {
            return &(no->cliente);
        }
        no = no->proximo;
    }
    return NULL;
}

// Função auxiliar para criar um novo nó binomial
BinomialNode* criarNoBinomial(Pedido pedido) {
    BinomialNode* novoNo = (BinomialNode*)malloc(sizeof(BinomialNode));
    novoNo->pedido = pedido;
    novoNo->grau = 0;

    novoNo->pai = NULL;
    novoNo->filho = NULL;
    novoNo->irmao = NULL;
    return novoNo;
}

// Função auxiliar para fundir dois heaps binomiais
BinomialNode* fundirBinomialHeaps(BinomialNode* heap1, BinomialNode* heap2) {
    if (heap1 == NULL) return heap2;
    if (heap2 == NULL) return heap1;

    BinomialNode* head = NULL;
    BinomialNode* tail = NULL;
    BinomialNode* temp1 = heap1;
    BinomialNode* temp2 = heap2;

    while (temp1 != NULL && temp2 != NULL) {
        if (temp1->grau <= temp2->grau) {
            if (tail == NULL) {
                tail = temp1;
                head = temp1;
            } else {
                tail->irmao = temp1;
                tail = tail->irmao;
            }
            temp1 = temp1->irmao;
        } else {
            if (tail == NULL) {
                tail = temp2;
                head = temp2;
            } else {
                tail->irmao = temp2;
                tail = tail->irmao;
            }
            temp2 = temp2->irmao;
        }
    }

    if (temp1 != NULL) {
        tail->irmao = temp1;
    } else {
        tail->irmao = temp2;
    }

    return head;
}

// Função auxiliar para inverter a ordem dos irmãos de um nó binomial
BinomialNode* inverterIrmaos(BinomialNode* no) {
    if (no == NULL || no->irmao == NULL) {
        return no;
    }

    BinomialNode* prev = NULL;
    BinomialNode* current = no;
    BinomialNode* next = current->irmao;

    while (next != NULL) {
        current->irmao = prev;
        prev = current;
        current = next;
        next = current->irmao;
    }

    current->irmao = prev;
    return current;
}

// Função auxiliar para unir dois heaps binomiais após a operação de remoção mínima
BinomialNode* unirBinomialTrees(BinomialNode* tree1, BinomialNode* tree2) {
    if (tree1->pedido.id < tree2->pedido.id) {
        tree1->irmao = tree2->filho;
        tree2->filho = tree1;
        tree1->pai = tree2;
        tree2->grau++;
        return tree2;
    } else {
        tree2->irmao = tree1->filho;
        tree1->filho = tree2;
        tree2->pai = tree1;
        tree1->grau++;
        return tree1;
    }
}

// Função auxiliar para unir dois heaps binomiais após a operação de união
BinomialNode* linkBinomialTrees(BinomialNode* tree1, BinomialNode* tree2) {
    tree1->irmao = tree2->filho;
    tree2->filho = tree1;
    tree1->pai = tree2;
    tree2->grau++;
    return tree2;
}

// Função auxiliar para ajustar a estrutura do heap binomial após a remoção mínima
BinomialNode* ajustarHeap(BinomialNode* heap) {
    if (heap == NULL || heap->irmao == NULL) {
        return heap;
    }

    BinomialNode* prev = NULL;
    BinomialNode* current = heap;
    BinomialNode* next = current->irmao;

    while (next != NULL) {
        if (current->grau != next->grau ||
            (next->irmao != NULL && next->irmao->grau == current->grau)) {
            prev = current;
            current = next;
        } else {
            if (current->pedido.id <= next->pedido.id) {
                current->irmao = next->irmao;
                linkBinomialTrees(next, current);
            } else {
                if (prev == NULL) {
                    heap = next;
                } else {
                    prev->irmao = next;
                }
                linkBinomialTrees(current, next);
                current = next;
            }
        }
        next = current->irmao;
    }

    return heap;
}

// Função para inserir um elemento no heap binomial
BinomialNode* inserirBinomialHeap(BinomialNode* heap, Pedido pedido) {
    BinomialNode* novoNo = criarNoBinomial(pedido);
    return fundirBinomialHeaps(heap, novoNo);
}

// Função auxiliar para encontrar o nó com o valor mínimo no heap binomial
BinomialNode* encontrarMinBinomialHeap(BinomialNode* heap) {
    BinomialNode* minNo = heap;
    BinomialNode* temp = heap;

    while (temp != NULL) {
        if (temp->pedido.id < minNo->pedido.id) {
            minNo = temp;
        }
        temp = temp->irmao;
    }
    return minNo;
}

BinomialNode* extrairMinBinomialHeap(BinomialNode* heap) {
    if (heap == NULL) {
        return NULL;
    }

    // Encontrar o nó com o valor mínimo
    BinomialNode* minNo = heap;
    BinomialNode* prevMin = NULL;
    BinomialNode* temp = heap;

    while (temp->irmao != NULL) {
        if (temp->irmao->pedido.id < minNo->pedido.id) {
            minNo = temp->irmao;
            prevMin = temp;
        }
        temp = temp->irmao;
    }

    // Remover o nó com o valor mínimo do heap binomial
    if (prevMin == NULL) {
        heap = minNo->irmao;
    } else {
        prevMin->irmao = minNo->irmao;
    }

    // Inverter a ordem dos filhos do nó removido
    BinomialNode* filho = minNo->filho;
    BinomialNode* filhoPrev = NULL;

    while (filho != NULL) {
        BinomialNode* temp = filho;
        filho = filho->irmao;
        temp->irmao = filhoPrev;
        filhoPrev = temp;
        temp->pai = NULL;
    }

    // Fundir o heap binomial original com os filhos do nó removido
    heap = fundirBinomialHeaps(heap, inverterIrmaos(filhoPrev));

    // Ajustar a estrutura do heap binomial
    heap = ajustarHeap(heap);

    // Liberar a memória do nó removido
    free(minNo);
    return heap;
}
// Função auxiliar para imprimir um nó binomial e seus filhos recursivamente
void imprimirBinomialNode(BinomialNode* node) {
    if (node == NULL) return;

    printf("%d ", node->pedido.id);
    imprimirBinomialNode(node->filho);
    imprimirBinomialNode(node->irmao);
}

// Função para imprimir o heap binomial
void imprimirBinomialHeap(BinomialNode* heap) {
    if (heap == NULL) {
        printf("\tCaixa vazio.\n");
        return;
    }

    printf("\tFila de pedidos: ");
    imprimirBinomialNode(heap);
    printf("\n");
}

BinomialNode* fecharCaixa(Caixa* caixaFechar, Caixa** caixas, int numCaixas) {
    // Verificar se há outro caixa disponível com a mesma prioridade para transferir os pedidos
    Caixa* caixaTransferir = NULL;
    for (int i = 0; i < numCaixas; i++) {
        if ((caixas[i]->tipo == caixaFechar->tipo) && (caixas[i] != caixaFechar) && (caixas[i]->estado != 0)) {
            caixaTransferir = caixas[i];
            break;
        }
    }

    if (caixaTransferir == NULL) {
        printf("Nenhum caixa disponivel com a mesma prioridade para transferir os pedidos.\n");
        return NULL;
    }

    // Extrair os pedidos do caixa a ser fechado
    BinomialNode* pedidosExtraidos = caixaFechar->heap;

    // Atualizar o estado dos caixas
    caixaFechar->estado = 0; // Fechado
    caixaFechar->heap = NULL;
    caixaTransferir->heap = fundirBinomialHeaps(caixaTransferir->heap, pedidosExtraidos);

    return pedidosExtraidos;
}

// Função auxiliar para trocar dois elementos do heap
void trocar(Pedido* a, Pedido* b) {
    Pedido temp = *a;
    *a = *b;
    *b = temp;
}

// Função para comparar os elementos do heap com base no campo "id"
int comparar(Pedido* a, Pedido* b) {
    if (a->id < b->id)
        return -1;
    else if (a->id > b->id)
        return 1;
    else
        return 0;
}

// Função para criar um heap vazio
HeapS* criarHeap(int capacidade) {
    HeapS* heap = (HeapS*)malloc(sizeof(HeapS));
    heap->v = (Pedido*)malloc(capacidade * sizeof(Pedido));
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    heap->estado = 1;
    return heap;
}

// Função para obter o índice do pai de um elemento no heap
int indicePai(int i) {
    return (i - 1) / 2;
}

// Função para obter o índice do filho esquerdo de um elemento no heap
int indiceFilhoEsquerdo(int i) {
    return (2 * i) + 1;
}

// Função para obter o índice do filho direito de um elemento no heap
int indiceFilhoDireito(int i) {
    return (2 * i) + 2;
}

// Função para inserir um elemento no heap
void inserirHeap(HeapS* heap, Pedido pedido) {

    if (heap->tamanho == heap->capacidade) {
        printf("O heap esta cheio. Nao e possivel inserir mais elementos.\n");
        return;
    }

    int indice = heap->tamanho;
    heap->v[indice] = pedido;
    heap->tamanho++;

    // Corrigir a ordem do heap subindo o elemento
    while (indice != 0 && comparar(&heap->v[indice], &heap->v[indicePai(indice)]) < 0) {
        trocar(&heap->v[indice], &heap->v[indicePai(indice)]);
        indice = indicePai(indice);
    }
}

// Função para remover o elemento de menor prioridade do heap e retornar o heap resultante
HeapS* removerHeap(HeapS* heap) {
    if (heap->tamanho == 0) {
        printf("A cozinha esta vazia. Nao e possivel remover elementos.\n");
        return heap;
    }

    heap->v[0] = heap->v[heap->tamanho - 1];
    heap->tamanho--;

    // Corrigir a ordem do heap descendo o elemento
    int indice = 0;
    while (1) {
        int indiceFilhoEsq = indiceFilhoEsquerdo(indice);
        int indiceFilhoDir = indiceFilhoDireito(indice);
        int indiceMenor = indice;

        if (indiceFilhoEsq < heap->tamanho && comparar(&heap->v[indiceFilhoEsq], &heap->v[indiceMenor]) < 0)
            indiceMenor = indiceFilhoEsq;
        if (indiceFilhoDir < heap->tamanho && comparar(&heap->v[indiceFilhoDir], &heap->v[indiceMenor]) < 0)
            indiceMenor = indiceFilhoDir;

        if (indiceMenor != indice) {
            trocar(&heap->v[indice], &heap->v[indiceMenor]);
            indice = indiceMenor;
        } else {
            break;
        }
    }
    return heap;
}


// Função para verificar se o heap está vazio
int estaVazio(HeapS* heap) {
    return heap->tamanho == 0;
}

// Função para liberar a memória alocada pelo heap
void liberarHeap(HeapS* heap) {
    free(heap->v);
    free(heap);
}

void imprimirHeap(HeapS* heap) {
    printf("\tCozinha:\n");
    if(heap->tamanho == 0){
        printf("\tFila vazia.");
        printf("\n");
        return;
    }

    printf("\tFila: ");
    for (int i = 0; i < heap->tamanho; i++) {
        printf("%d ", heap->v[i].id);
    }

    printf("\n");
}
