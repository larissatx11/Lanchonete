#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_CAIXAS 10
#define MAX 11

// Struct para representar um cliente
typedef struct {
    char nome[50];
    int mesa;
    char horarioInicio[20];
} Cliente;

typedef struct {
    Cliente* cliente;
    int id;
    int tipo_atendimento; // Prioritário ou não prioritário
    float total;
    char horarioConclusao[20];
} Pedido;

struct no{
        Cliente cliente;
        struct no *proximo;
};
typedef struct no* Tno;

typedef struct{
        Tno elemento[MAX];
        int n;
} TLista;

typedef struct noArv{
    int grau;
    Pedido pedido;

    struct noArv* irmao;
    struct noArv* pai;
    struct noArv* filho;
}BinomialNode;

// Struct para representar um caixa
typedef struct {
    BinomialNode* heap;
    int tipo; // 1 - Prioritário ou 2 - não prioritário
    int estado; // 1 - Aberto ou 0 - fechado
} Caixa;

typedef struct {
    Pedido* v;
    int tamanho;
    int capacidade;
    int estado;
} HeapS;

int criar(TLista *L);
int inserir(TLista *L, int hash, Cliente cliente);
int hashing(int chave);
void exibir(TLista L);
Cliente* buscarCliente(TLista* L, int hash, int chave);
BinomialNode* criarNoBinomial(Pedido pedido);
BinomialNode* fundirBinomialHeaps(BinomialNode* heap1, BinomialNode* heap2);
BinomialNode* inverterIrmaos(BinomialNode* no);
BinomialNode* unirBinomialTrees(BinomialNode* tree1, BinomialNode* tree2);
BinomialNode* linkBinomialTrees(BinomialNode* tree1, BinomialNode* tree2);
BinomialNode* ajustarHeap(BinomialNode* heap);
BinomialNode* inserirBinomialHeap(BinomialNode* heap, Pedido pedido);
BinomialNode* encontrarMinBinomialHeap(BinomialNode* heap);
BinomialNode* extrairMinBinomialHeap(BinomialNode* heap);
BinomialNode* unionHeaps(BinomialNode* heap1, BinomialNode* heap2);
void imprimirBinomialNode(BinomialNode* node);
void imprimirBinomialHeap(BinomialNode* heap);
BinomialNode* fecharCaixa(Caixa* caixaFechar, Caixa** caixas, int numCaixas);
void trocar(Pedido* a, Pedido* b);
int comparar(Pedido* a, Pedido* b);
HeapS* criarHeap(int capacidade);
int indicePai(int i);
int indiceFilhoEsquerdo(int i);
int indiceFilhoDireito(int i);
void inserirHeap(HeapS* heap, Pedido pedido);
HeapS* removerHeap(HeapS* heap);
int estaVazio(HeapS* heap);
void liberarHeap(HeapS* heap);
void imprimirHeap(HeapS* heap);
