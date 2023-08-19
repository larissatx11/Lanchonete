#include <stdio.h>
#include <time.h>
#include "lib.h"

int main() {
    int op, hash, p, opcao, m, id = 0, numCaixas = 0, tipoAtendimento, fechar, indice, flag = 0;
    float soma = 0;
    TLista L;
    Cliente cliente;
    Pedido pedido;
    Caixa* caixas[MAX_CAIXAS];
    criar(&L);
    struct tm *timeinicio, *timefim;
    char str_inicio[100], str_fim[100];
    time_t aux;
    HeapS* heap;

   do{
    //Exibir menu
        puts("\n\t\t\t\tEDITOR DE LISTAS\n");
        puts("\t1  - ABRIR LANCHONETE\n\t2  - EXIBIR LISTA\n\t3  - REGISTRAR CLIENTE\n\t4  - FAZER PEDIDO\n\t"
             "5  - CRIAR CAIXA PRIORITARIO\n\t6  - CRIAR CAIXA NAO PRIORITARIO\n\t7  - FECHAR CAIXA"
             "\n\t8  - REALIZAR PAGAMENTO\n\t9  - PEDIDO PRONTO\n\t10 - FECHAR LANCHONETE\n\t0  - SAIR\n");

        for(int i = 0; i < numCaixas; i++){
            printf("\tCaixa %d: %s\n", i+1, caixas[i]->tipo == 1 ? "prioritario" : "nao prioritario");
            if (caixas[i]->estado == 1) {
                imprimirBinomialHeap(caixas[i]->heap);
            } else {
                printf("\tCaixa fechado.\n");
            }
            printf("\n");
        }

        if(heap->estado == 1){
            imprimirHeap(heap);
        } else if(heap->estado == 2){
            printf("\tCozinha fechada.\n");
        }

        printf("\nINFORME SUA OPCAO:\n");
        scanf("%d", &op);//Escolha da Opção

    	switch(op)
        	{
        	    case 1:
                    if (numCaixas >= MAX_CAIXAS) {
                        printf("Limite maximo de caixas atingido.\n");
                        return;
                    }

                    Caixa* caixaPrioritario = (Caixa*)malloc(sizeof(Caixa));
                    Caixa* caixaNaoPrioritario = (Caixa*)malloc(sizeof(Caixa));

                    if (caixaPrioritario == NULL || caixaNaoPrioritario == NULL) {
                        printf("Erro ao alocar memoria para os caixas.\n");
                        return;
                    }

                    caixaPrioritario->heap = NULL;
                    caixaPrioritario->tipo = 1; // Prioritário
                    caixaPrioritario->estado = 1; // Aberto

                    caixaNaoPrioritario->heap = NULL;
                    caixaNaoPrioritario->tipo = 2; // Não prioritário
                    caixaNaoPrioritario->estado = 1; // Aberto

                    caixas[numCaixas] = caixaPrioritario;
                    (numCaixas)++;
                    caixas[numCaixas] = caixaNaoPrioritario;
                    (numCaixas)++;

                    heap = criarHeap(20);

                    printf("Lanchonete aberta com sucesso.\n");
                    break;

        		case 2:
        			exibir(L);
					break;

				case 3:
				    // Capturar o horário atual
                    aux = time(NULL);
                    timeinicio = localtime(&aux);
                    strftime(str_inicio, sizeof(str_inicio), "%H:%M:%S", timeinicio);
                    strcpy(cliente.horarioInicio, str_inicio);

                    printf("Insira seu nome: ");
                    fflush(stdin);
	                scanf("%[^\n]", cliente.nome);

					printf("Informe o numero da mesa: ");
	                scanf("%d", &cliente.mesa);

	                hash = hashing(cliente.mesa);
	                inserir(&L, hash, cliente);
                    printf("\nMesa: %d", cliente.mesa);
                    printf("\nNome: %s", cliente.nome);
                    printf("\nHorario de chegada: %s", str_inicio);
                    break;
				case 4:
				    soma = 0;
                    printf("Informe sua mesa: ");
                    scanf("%d", &m);
                    hash = hashing(m);
                    Cliente* clienteEncontrado = buscarCliente(&L, hash, m);
                    if(clienteEncontrado == NULL){
                        printf("Mesa nao cadastrada.");
                        break;
                    } else {
                        pedido.cliente = clienteEncontrado;
                    }

                    printf("Informe o tipo de atendimento: \n 1- Prioritario \n 2- Nao Prioritario \n");
	                scanf("%d", &pedido.tipo_atendimento);
	                // Verificar se o tipo de atendimento é válido
                    if (pedido.tipo_atendimento != 1 && pedido.tipo_atendimento != 2) {
                        printf("Tipo de atendimento invalido.\n");
                        break;
                    }
                    tipoAtendimento = pedido.tipo_atendimento;

                    do{
                    puts("--------- Cardapio ----------");
                    printf(" 1- Misto-Quente    R$2,50\n 2- X-Burger    R$4,00\n 3- X-Salada    R$6,00\n 4- X-Egg    R$6,50\n 5- X-Tudo    R$15,00\n\n---------- Sucos ----------"
                           "\n 6- Laranja    R$3,00\n 7- Maracuja    R$3,00\n 8- Goiaba    R$3,00\n\n");
                            printf("Insira seu pedido: ");
                            scanf("%d", &p);
                            if(p == 1){
                                soma = soma + 2.50;
                            } else if(p == 2){
                                soma = soma + 4;
                            } else if(p == 3){
                                soma = soma + 6;
                            } else if(p == 4){
                                soma = soma + 6.50;
                            } else if(p == 5){
                                soma = soma + 15;
                            } else if(p == 6 || p == 7 || p == 8 ){
                                soma = soma + 3;
                            }
                            printf("\nDeseja mais alguma coisa?\n 1- Sim\n 0- Nao \n");
                            scanf("%d", &opcao);
                    }while(opcao != 0);
                    id++;
                    pedido.id = id;
                    pedido.total = soma;

                    int i = 0;
                    if (tipoAtendimento == 1) {
                        printf("\nCaixas disponiveis: \n");
                        for (int j = 0; j < numCaixas; j++) {
                            if (caixas[j]->tipo == 1) {
                                printf(" Caixa %d: prioritario\n", j + 1);
                            }
                        }
                        printf("\n Em qual caixa prioritario deseja inserir? ");
                        scanf("%d", &i);
                        i--;
                        if (i >= 0 && i < numCaixas && caixas[i]->tipo == 1) {
                            caixas[i]->heap = inserirBinomialHeap(caixas[i]->heap, pedido);
                            printf("\nPedido inserido no caixa prioritario.\n");
                        } else {
                            printf("\nCaixa prioritario selecionado invalido.\n");
                        }
                    } else if (tipoAtendimento == 2) {
                        printf("\nCaixas disponiveis: \n");
                        for (int j = 0; j < numCaixas; j++) {
                            if (caixas[j]->tipo == 2) {
                                printf(" Caixa %d: nao prioritario\n", j + 1);
                            }
                        }
                        printf("\n Em qual caixa nao prioritario deseja inserir?");
                        scanf("%d", &i);
                        i--;
                        if (i >= 0 && i < numCaixas && caixas[i]->tipo == 2) {
                            caixas[i]->heap = inserirBinomialHeap(caixas[i]->heap, pedido);
                            printf("\nPedido inserido no caixa nao prioritario.\n");
                        } else {
                            printf("\nCaixa nao prioritario selecionado invalido.\n");
                        }
                    } else {
                        printf("\nTipo de atendimento invalido.\n");
                    }

                    aux = time(NULL);
                    timefim = localtime(&aux);
                    strftime(str_fim, sizeof(str_fim), "%H:%M:%S", timefim);
                    strcpy(pedido.horarioConclusao, str_fim);
                    printf("\n Id do pedido: %d", pedido.id);
                    printf("\nNome do cliente: %s", pedido.cliente->nome);
                    printf("\n Total: %.2f", pedido.total);
                    printf("\n Tipo: %d",tipoAtendimento);
                    printf("\n Horario do termino do pedido: %s", str_fim);
                    break;
            case 5:
                    // Criar um novo caixa prioritário
                if (numCaixas < MAX_CAIXAS) {
                    Caixa* novoCaixa = (Caixa*)malloc(sizeof(Caixa));
                    novoCaixa->heap = NULL;
                    novoCaixa->tipo = 1; // Prioritário
                    novoCaixa->estado = 1; // Aberto

                    caixas[numCaixas] = novoCaixa;
                    numCaixas++;

                    printf("Caixa prioritario criado.\n");
                } else {
                    printf("Limite maximo de caixas atingido.\n");
                }

                break;
            case 6:
                // Criar um novo caixa não prioritário
                if (numCaixas < MAX_CAIXAS) {
                    Caixa* novoCaixa = (Caixa*)malloc(sizeof(Caixa));
                    novoCaixa->heap = NULL;
                    novoCaixa->tipo = 2; // Não prioritário
                    novoCaixa->estado = 1; // Aberto

                    caixas[numCaixas] = novoCaixa;
                    numCaixas++;

                    printf("Caixa nao prioritario criado.\n");
                } else {
                    printf("Limite maximo de caixas atingido.\n");
                }
                break;
            case 7:
                // Fechar um caixa e transferir pedidos
                printf("\nCaixas disponiveis: \n");
                for(int i = 0; i < numCaixas; i++){
                    printf("Caixa %d: %s\n", i+1, caixas[i]->tipo == 1 ? "prioritario" : "nao prioritario");
                }
                printf("Informe qual caixa deseja fechar: \n");
                scanf("%d", &fechar);
                fechar--;
                Caixa* caixaFechar = caixas[fechar];
                BinomialNode* pedidosTransferidos = fecharCaixa(caixaFechar, caixas, numCaixas);
                if (pedidosTransferidos != NULL) {
                    printf("Pedidos transferidos para outro caixa com a mesma prioridade.\n");
                }
                break;
            case 8:
                //Realiza pagamento
                printf("Em que caixa seu pedido esta? ");
                scanf("%d", &indice);
                indice--;
                printf("Dados do pedido %d: \n", caixas[indice]->heap->pedido.id);
                printf("Cliente: %s, Mesa: %d, Total: %.2f", caixas[indice]->heap->pedido.cliente->nome,
                       caixas[indice]->heap->pedido.cliente->mesa, caixas[indice]->heap->pedido.total);
                printf("\nQual a forma de pagamento? 1 - Cartao, 2 - Dinheiro. ");
                scanf("%d");
                printf("Processando...");
                sleep(2);
                printf("\nPagamento concluido com sucesso!");

                Pedido pedido = caixas[indice]->heap->pedido;

                BinomialNode* No = caixas[indice]->heap;
                caixas[indice]->heap = extrairMinBinomialHeap(No);

                inserirHeap(heap, pedido);
                break;
            case 9:
                printf("Pedido pronto!\n");
                printf("Nome: %s", heap->v[0].cliente->nome);
                heap = removerHeap(heap);
                break;

            case 10:

                flag = 0;
                for(int i = 0; i < numCaixas; i++){
                    if(caixas[i]->heap != NULL){
                        flag = 1;
                        break;
                    }
                }

                if(heap->tamanho == 0 && flag == 0){
                    heap->estado = 2;
                } else {
                    flag = 1;
                }

                if(flag == 0){
                    for(int i = 0; i < numCaixas; i++){
                        if(caixas[i]->heap == NULL){
                            caixas[i]->estado = 0;
                        }
                    }
                    printf("\n Lanchonete fechada com sucesso!");
                } else {
                    printf("\nVoce nao pode fechar a lanchonete pois ha caixas com clientes ou ha pedidos na cozinha.");
                }
                break;
    		default:
    			puts("OPCAO INCORRETA,TENTAR NOVAMENTE.");
    			break;
        }
    }while(op!=0);

  system("PAUSE");
  return 0;
}
