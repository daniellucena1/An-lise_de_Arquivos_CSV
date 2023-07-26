/*
  Daniel Torres
  João Francisco
  Giovanna Andrade
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


typedef struct novoNO {

  int CodCidade, CodEstado, TotalCasos, UltimaAtualizacao;
  struct novoNO* proximo;

} novoNO;

int MenuInicial();
char *EpochParaData(int epochTime);
int VerificarCarga(int cod_cidade, int cod_estado, novoNO** cabeca);
void AdicionarElemento(novoNO** cabeca, novoNO** calda, int data, int cod_cidade, int cod_estado, int novoscasos);
void lerArquivoConsolidadoExistente(novoNO** cabeca, novoNO** calda);
void LerArquivoCSV(char nomeArquivo[], novoNO** cabeca, novoNO** calda);
void HistoricoDeCargas(int data, int cod_cidade, int cod_estado, int novoscasos);
void DadosConsolidados(char nomeArquivo[], novoNO** cabeca);
void PrintarCasosPorEstado(int carga, novoNO** cabeca);

// Função que retorna a escolha do usuário
int MenuInicial(){
  int escolha;
  printf("----- SISTEMA DE COSOLIDACAO DE DADOS -----\n");
  printf("1- Para fazer a carga de dados\n");
  printf("2- Para imprimir relatorio estadual\n");
  printf("0- Sair\n");
  printf(">> Digite sua opcao: ");
  scanf("%d", &escolha);
  fflush(stdin);

  return escolha;
}

// retorna uma string que representa a data 
char *EpochParaData(int epochTime){
  time_t epoch_time = epochTime;
  struct tm *tm_info = localtime(&epoch_time);

  // aloca uma memória para uma string [dd/mm/aaaa]
  // será liberada no momento em que for usada na função PrintarCasosPorEstado
  char *data = calloc(20, sizeof(char));

  strftime(data, 20, "%d/%m/%Y", tm_info);
  return data;
}

/* verifica se aquele estado ou cidade existem na lista dinâmica, se sim, retorna 1, 
   se não, retorna 0 */
int VerificarCarga(int cod_cidade, int cod_estado, novoNO** cabeca){
  novoNO* percorrer = *cabeca;
  while(percorrer != NULL){
    if(percorrer->CodEstado == cod_estado){
      if(percorrer->CodCidade == cod_cidade){
        return 1;
      }

      // serve para verificar na função PrintarCasosPorEstado se o estado existe na lista dinâmica
      if(cod_cidade == 0){
        return 1;
      }
    }
    percorrer = percorrer->proximo;
  }
  return 0;
}

// Adiciona elementos no final da lista dinâmica
void AdicionarElemento(novoNO** cabeca, novoNO** calda, int data, int cod_cidade, int cod_estado, int novoscasos){

  // aloca uma memória para criação de elementos na lista dinâmica
  novoNO* newnode = (novoNO*) malloc(sizeof(novoNO));

  newnode->UltimaAtualizacao = data;
  newnode->CodCidade = cod_cidade;
  newnode->CodEstado = cod_estado;
  newnode->TotalCasos = novoscasos;
  newnode->proximo = NULL;

  if(*cabeca == NULL){
    *cabeca = newnode;
    *calda = newnode;
  }else{
    (*calda)->proximo = newnode;
    *calda = newnode;
  }
}

// Função que verifica se o arquivo csv dos dados consolidados está vazio ou não
// Se não estiver, ele salva os dados do arquivo na lista dinâmica
void lerArquivoConsolidadoExistente(novoNO** cabeca, novoNO** calda){
  FILE* arquivo = fopen("dados_consolidados.csv", "r");
  int data, cod_cidade, cod_estado, novoscasos;
  if(arquivo != NULL){
    while(!feof(arquivo)){

      fscanf(arquivo, "%d;%d;%d;%d\n", &cod_cidade, &cod_estado, &novoscasos, &data);

      AdicionarElemento(&*cabeca, &*calda, data, cod_cidade, cod_estado, novoscasos);

    }
    fclose(arquivo);
  }
}

// Função que ler os arquivos de cargas
/* Recebe como parâmetro o nome do arquivo para consolidar, a cabeça que aponta para o primeiro nó
   da lista dinâmica, e a calda que aponta para o último */
void LerArquivoCSV(char nomeArquivo[], novoNO** cabeca, novoNO** calda){
  
  FILE* arquivodeCargas = fopen(nomeArquivo, "r");
  int data, cod_cidade, cod_estado, novoscasos;

  if(arquivodeCargas != NULL){

    while(!feof(arquivodeCargas)){

      // ler linha por linha do arquivo de carga
      fscanf(arquivodeCargas, "%d;%d;%d;%d\n", &data, &cod_cidade, &cod_estado, &novoscasos);

      if(VerificarCarga(cod_cidade, cod_estado, &*cabeca) == 0){
        
        AdicionarElemento(&*cabeca, &*calda, data, cod_cidade, cod_estado, novoscasos);

      }else{
        
        // percorre a lista dinâmica e verifica se a cidade já existe
        // se sim, ele soma os casos
        novoNO* percorrer = *cabeca;
        while(percorrer != NULL){
          if(percorrer->CodEstado == cod_estado){
              
            if(percorrer->CodCidade == cod_cidade){
              
              percorrer->TotalCasos += novoscasos;
              
              /* Se a nova data for mais recente que a anterior
                 ele altera a variável para a nova data */
              if(data > percorrer->UltimaAtualizacao){
                  
                percorrer->UltimaAtualizacao = data;
              }
            }
          }
          percorrer = percorrer->proximo;
        }
      }
      HistoricoDeCargas(data, cod_cidade, cod_estado, novoscasos);
    }
    printf("\n>> Arquivo consolidado com sucesso!\n\n");
    fclose(arquivodeCargas);
  }else{
    printf("\n>> Nao foi possivel abrir o arquivo\n\n");
  }
}


// Função que cria o historico de cargas
/* Recebe como parâmetro os dados retirados da leitura do arquivo de carga e não os dados
  de uma lista dinâmica */
void HistoricoDeCargas(int data, int cod_cidade, int cod_estado, int novoscasos){
  FILE *historico = fopen("historico_de_cargas.csv", "a");

  // variável que representa a data atual em epoch
  long int t = time(NULL);

  fprintf(historico, "%d;%d;%d;%d;%d\n", t, cod_cidade, cod_estado, novoscasos, data);
  fclose(historico);
}

// Função para consolidar os dados
// Percorre a lista dinâmica e escreve no arquivo "dados_consolidados.csv" todos os dados retirados das cargas
void DadosConsolidados(char nomeArquivo[], novoNO** cabeca){

  FILE* arquivodeDados = fopen("dados_consolidados.csv", "w");
  novoNO* percorrer = *cabeca;
  while(percorrer != NULL){
    fprintf(arquivodeDados, "%d;%d;%d;%d\n", percorrer->CodCidade, percorrer->CodEstado, percorrer->TotalCasos, percorrer->UltimaAtualizacao);
    percorrer = percorrer->proximo;
  }
  fclose(arquivodeDados);

}

// Função que printa os casos por estado
// Recebe como parâmetro o código do estado e a cabeça para percorrer a lista dinâmica
void PrintarCasosPorEstado(int carga, novoNO** cabeca){
  novoNO* percorrer = *cabeca;

  // Verifica se o estado já foi consolidado
  if(VerificarCarga(0, carga, &*cabeca) == 1){

    // Percorre a lista dinâmica e soma todos os casos do estado selecionado
    int numerodeCasos = 0;
    while(percorrer != NULL){
      if(percorrer->CodEstado == carga){
        numerodeCasos += percorrer->TotalCasos;
      }
      percorrer = percorrer->proximo;
    }

    // Reinicia o nó para percorrer a lista novamente
    percorrer = *cabeca;

    printf("\n********* RELATORIO ESTADUAL **********\n");
    printf("Codigo do Estado: %d\n", carga);
    printf("Total de casos no Estado: %d\n", numerodeCasos);
    printf("Dados por cidade:\n\n");
    printf("CIDADE \t QTD CASOS \t ULTIMA ATUALIZACAO\n");
    printf("------------------------------------------------------\n\n");
    while(percorrer != NULL){

      // Printa todos casos do estado separado por cidade e sua data de registro mais recente
      if(percorrer->CodEstado == carga){
        char *data = EpochParaData(percorrer->UltimaAtualizacao);
        printf("%5d %12d  %20s\n", percorrer->CodCidade, percorrer->TotalCasos, data);
        free(data);
        percorrer = percorrer->proximo;
      }else{
        percorrer = percorrer->proximo;
      }
    }
    printf("\n-----------------------------------------------------\n\n");

  }else{

    // Caso as cargas do estado não tenham sido consolidadas
    printf("\n>> Codigo do estado informado nao existe na base\n\n");
  }
}

int main() {
  
    char nome_arquivo_entrada[100];

    // Define os primeiros nós da lista
    novoNO *cabeca = NULL, *calda = NULL;

    lerArquivoConsolidadoExistente(&cabeca, &calda);

    int escolha = 1;
    while(escolha != 0){

      escolha = MenuInicial();

      if(escolha == 0){

        printf("\n>> Encerrando...\n");

      }else if(escolha == 1){
        
        printf("\n>> Digite o nome do arquivo de entrada: ");
        scanf("%s", nome_arquivo_entrada);
        fflush(stdin);

        LerArquivoCSV(nome_arquivo_entrada, &cabeca, &calda);
        DadosConsolidados(nome_arquivo_entrada, &cabeca);
        
      }else if(escolha == 2){

        int carga;
        printf("\n>> Codigo do Estado: ");
        scanf("%d", &carga);
        fflush(stdin);
        PrintarCasosPorEstado(carga, &cabeca);

      }

    }

    // libera a memória alocada para a lista dinâmica
    calda = cabeca;
    while(calda != NULL){
        novoNO* prox = calda ->proximo;
        free(calda);
        calda = prox;
    }

  return 0;
}