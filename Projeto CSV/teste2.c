#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct dados{
    int dataRegistro, codCidade, codEstado, novosCasos;
    struct dados *proximo;
};

char *epochData(int tempo){
    time_t epoch_time = tempo;
    struct tm *tm_info = localtime(&epoch_time);
    char *data = calloc(20,sizeof(char));
    strftime(data, 20, "%d/%m/%Y",tm_info);
    return data;
}

void escreverRegistro(struct dados *inicio){          // Escreve todos os dados mas SEM consolidar;
    struct dados *noAtual = inicio;
    FILE *novoArquivo = fopen("historico_Cargas.csv", "w");

    if(novoArquivo != NULL)
    {
        //printf("Arquivo copiado com sucesso!");
        while(noAtual != NULL)
        {
            if (noAtual->proximo != NULL){
                fprintf(novoArquivo, "%d;%d;%d;%d\n",noAtual->codCidade,noAtual->codEstado,noAtual->novosCasos,noAtual->dataRegistro);
            }else{
                fprintf(novoArquivo, "%d;%d;%d;%d",noAtual->codCidade,noAtual->codEstado,noAtual->novosCasos,noAtual->dataRegistro);
            }
            noAtual = noAtual->proximo;
        }
    }
    else
    {
        //printf("Nao foi possível abrir arquivo");
    }
    fclose(novoArquivo);
}

int verificarIguais(int cidade, int estado, struct dados *inicio){
    struct dados *noAtual = inicio;

    while (noAtual != NULL){
        if (noAtual->codEstado == estado){
            if (noAtual->codCidade == cidade){
                return 1;
            }
        }
            noAtual = noAtual->proximo;
    }
    return 0;
}

int manipularDados(char nomeArquivo[], struct dados **cabeca, struct dados **rabo, struct dados *cabecaLista) {
    FILE *pontArquivos = fopen(nomeArquivo, "r");

    if (pontArquivos != NULL) {
        while (!feof(pontArquivos)) {
            int campo1, campo2, campo3, campo4;
            fscanf(pontArquivos, "%d;%d;%d;%d", &campo1, &campo2, &campo3, &campo4);

            int data = campo1, codigoCidade = campo2, codigoEstado = campo3, novosCasos = campo4;

            if (verificarIguais(codigoCidade,codigoEstado,cabecaLista) == 1){
                struct dados *noAtual = cabecaLista;
                while (noAtual != NULL){
                    if (noAtual->codEstado == campo3){
                        if (noAtual->codCidade == campo2){
                            noAtual->novosCasos += campo4;
                            if (campo1 > noAtual->dataRegistro){
                                noAtual->dataRegistro = campo1;
                            }
                        }
                        noAtual = noAtual->proximo;
                    }else{
                        noAtual = noAtual->proximo;
                    }
                }
            }else{
                struct dados *novoNo = malloc(sizeof(struct dados));
                novoNo->dataRegistro = data;
                novoNo->codCidade = codigoCidade;
                novoNo->codEstado = codigoEstado;
                novoNo->novosCasos = novosCasos;
                novoNo->proximo = NULL;
                if (*cabeca == NULL) {
                    *cabeca = novoNo;
                    *rabo = novoNo;
                } else {
                    (*rabo)->proximo = novoNo;
                    *rabo = novoNo;
                }
            }
        }
    } else {
        return 0;
    }
    fclose(pontArquivos);
    return 1;
}


void imprimir(int entradaUsuario){
    FILE *arquivoPronto = fopen("arquivoConsolidado.csv", "r");
    int somaGeral = 0;

    if (arquivoPronto != NULL) {
        while (!feof(arquivoPronto)) {
            int campo1,campo2,campo3,campo4;
            fscanf(arquivoPronto, "%d;%d;%d;%d", &campo1, &campo2, &campo3, &campo4);
            if (campo2 == entradaUsuario){
                somaGeral += campo3;
            }
        }
        printf("*** RELATORIO ESTADUAL ***\n");
        printf("Codigo Estado %d\n", entradaUsuario);
        printf("Total de casos no estado: %d casos\n",somaGeral);
        printf("Dados por cidade:\n");
        printf("\n");
        printf("CIDADE \t QTD CASOS \t ULTIMA ATUALIZACAO\n");
        printf("--------------------------------------\n");

        rewind(arquivoPronto);
        while (!feof(arquivoPronto)) {
            int campo1,campo2,campo3,campo4;
            fscanf(arquivoPronto, "%d;%d;%d;%d", &campo1, &campo2, &campo3, &campo4);
            if (campo2 == entradaUsuario){
                char *data = epochData(campo4);
                printf("%5d %9d \t %s\n", campo1, campo3, data);
                free(data);
            }
        }

    } else {
        puts("Dados inexistentes !\n");
    }
    printf("\n");
    fclose(arquivoPronto);
}

void imprimirLista(struct dados **inicio){
    struct dados *NoAtual = *inicio;

    //caminha ate o final
    while (NoAtual != NULL){
        printf("%d;%d;%d;%d\n", NoAtual->codCidade, NoAtual->codEstado, NoAtual->novosCasos, NoAtual->dataRegistro);
        NoAtual = NoAtual->proximo;
    }
}

void exibirMenu() {
    int opcao;
    int entrada;
    char nomeArquivo[50];
    struct dados *cabeca = NULL, *rabo = NULL;

    do {
        printf("***** SISTEMA DE CONSOLIDAÇÃO DE DADOS *****\n");
        printf("\n");
        printf("1- FAZER CARGA DE ARQUIVOS.\n");
        printf("2- IMPRIMIR RELÁTORIO POR ESTADO.\n");
        printf("0- SAIR.\n");
        printf("DIGITE SUA OPÇÃO: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Digite o nome do arquivo a ser adicionado: ");
                scanf("%s",nomeArquivo);
                manipularDados(nomeArquivo, &cabeca,&rabo,cabeca);
                //consolidarDados(cabeca);
                escreverRegistro(cabeca);
                if(manipularDados(nomeArquivo,&cabeca, &rabo,cabeca) != 0){
                    printf("Arquivo copiado com sucesso !\n\n");
                }else{
                    printf("Arquivo não copiado !\n\n");
                }

                break;
            case 2:
                printf("DIGTE O CODIGO DO ESTADO :\n");
                scanf("%i",&entrada);
                imprimir(entrada);
                break;
            case 3:
                printf("Você selecionou a opção 3.\n");
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while(opcao != 4);
}

int main(){
    struct dados *cabeca = NULL, *rabo = NULL;

    //exibirMenu();

    //manipularDados("cargaUF2.csv", &cabeca, &rabo, cabeca);
    //manipularDados("cargaUF4.csv", &cabeca, &rabo);
    //manipularDados("cargaUF10.csv", &cabeca, &rabo);
    //manipularDados("cargaUF12.csv", &cabeca, &rabo);
    //manipularDados("cargaUF13.csv", &cabeca, &rabo);
    manipularDados("cargaUF16.csv", &cabeca, &rabo,cabeca);
    //manipularDados("cargaUF16.csv", &cabeca, &rabo,cabeca);
    //manipularDados("cargaUF22.csv", &cabeca, &rabo);
    imprimirLista(&cabeca);

    //consolidarDados(cabeca);
    //imprimir(2);
    return 0;
}