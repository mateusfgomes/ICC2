#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define DEBUG 0 //para debug

/*
=============================================================================
Mateus Ferreira Gomes
Nº USP: 10734773
Instituto de Ciencias Matematicas e de Computacao - Universidade de Sao Paulo

Trabalho 2 - Introducao a Ciencia da Computacao II

Programa que trabalha com aprendizado de maquina para disticao de Classes de
flores a partir de caracteristicas de exemplos fornecidos por meio de um
arquivo
=============================================================================
*/


//definindo a estrutura de caracteristicas da flor
typedef struct flor{

  char name[11];//nome
  double sl;//sepal lenght
  double sw;//sepal width
  double pl;//petal lenght
  double pw;//petal width

}FLOR;

//funcao que pega os valores do treino
void treinar(FILE* ftrain, FLOR** flor_treino){

  char buffer[68];//armazena a primeira linha do treino
  fgets(buffer, 68, ftrain);//pega a primeira linha do treino
  int i = 0;
  char c;//caracter para ser descartado (virgula e aspa)

  //pegando as dimensoes da flor e guardando num vetor de structs de treino
  while(fscanf(ftrain, "%lf%c%lf%c%lf%c%lf%c%c%s", &(*flor_treino)[i].sl, &c, &(*flor_treino)[i].sw, &c, &(*flor_treino)[i].pl, &c, &(*flor_treino)[i].pw, &c, &c, &*(*flor_treino)[i].name) != EOF ){
    if (DEBUG) printf("%lf\n", (*flor_treino)[i].sl);
    if (DEBUG) printf("%lf\n", (*flor_treino)[i].sw);
    if (DEBUG) printf("%lf\n", (*flor_treino)[i].pl);
    if (DEBUG) printf("%lf\n", (*flor_treino)[i].pw);
    if (DEBUG) printf("%s\n", (*flor_treino)[i].name);
    i++;
  }
}

//funcao que pega os valores do teste
void testar(FILE* ftest, FLOR** flor_teste){

  char buffer[68];//armazena a primeira linha do teste
  fgets(buffer, 68, ftest);//pegando essa primeira linha
  int i = 0;
  char c;//caracter a ser descartado

  //pegando as dimensoes das flores e guardando num vetor de structs de teste
  while(fscanf(ftest, "%lf%c%lf%c%lf%c%lf%c%c%s", &(*flor_teste)[i].sl, &c, &(*flor_teste)[i].sw, &c, &(*flor_teste)[i].pl, &c, &(*flor_teste)[i].pw, &c, &c, &*(*flor_teste)[i].name) != EOF ){
    if (DEBUG) printf("%lf\n", (*flor_teste)[i].sw);
    if (DEBUG) printf("%lf\n", (*flor_teste)[i].pl);
    if (DEBUG) printf("%lf\n", (*flor_teste)[i].sl);
    if (DEBUG) printf("%lf\n", (*flor_teste)[i].pw);
    if (DEBUG) printf("%s\n", (*flor_teste)[i].name);
    i++;
  }
}

//contando as quebras de linha para saber o tamanho do vetor a ser alocado
int n_quebras_linha(FILE* file){

  int contador = 0;//variavel para contar as linhas
  char h;

  //procurando char a char por um \n ate o fim do arquivo
  while((fscanf(file, "%c", &h)) != EOF){
      if(h == '\n')//se for \n
        contador++;//conta
  }
  //retorna contador -1 para tirar o \n da primeira linha
  return contador-1;
}

//funcao que remove a ultima aspa do nome da flor
void remove_aspa(FLOR** flor_treino, FLOR** flor_teste, int conta_quebras_train, int conta_quebras_test){

  //armazena o tamanho do nome da flor
  int tamanho;


  //vai exemplo de treino por exemplo de treino
  for(int i = 0; i < conta_quebras_train; i++){
    tamanho = strlen((*flor_treino)[i].name); //pega o tamanho do nome da flor
    //procura no tamanho inteiro por uma aspa e remove
    for(int j = 0; j < tamanho; j++){
      if((*flor_treino)[i].name[j] == '"'){
        (*flor_treino)[i].name[j] = '\0'; //troca a aspa por \0
      }
    }
  }


  //vai exemplo de teste por exemplo de teste
  for(int i = 0; i < conta_quebras_test; i++){
    tamanho = strlen((*flor_teste)[i].name); //pega novamente o tamanho do nome da flor
    //procura no tamanho inteiro por aspas
    for(int j = 0; j < tamanho; j++){
      if((*flor_teste)[i].name[j] == '"'){
        (*flor_teste)[i].name[j] = '\0'; //substitui por \0
      }
    }
  }
}

//calcula a distancia do exemplo de teste para todos os exemplos de treino
void calcula_vizinhos(FLOR* flor_train, FLOR* flor_teste, int conta_quebras_train, int conta_quebras_test, double distancia[conta_quebras_test][conta_quebras_train]){

  //calculando essa distancia e armazenando numa matriz de distancias
  for(int i = 0; i < conta_quebras_test; i++){
    for(int j = 0; j < conta_quebras_train; j++){
      distancia[i][j] = sqrt(pow((flor_train[j].sl - flor_teste[i].sl), 2) + pow(flor_train[j].sw - flor_teste[i].sw, 2) + pow(flor_train[j].pl - flor_teste[i].pl, 2) + pow(flor_train[j].pw - flor_teste[i].pw, 2));
    }
  }
}

//funcao que verifica qual e o tipo de flor a partir da analise dos dados da matriz de distancias ordenada e da matriz de posicoes ordenada
// recebe o numero de teste e de treino
void checar(int conta_quebras_test, int conta_quebras_train, int matriz_posicoes[conta_quebras_test][conta_quebras_train], double distancia[conta_quebras_test][conta_quebras_train], int* contador, int k, FLOR* flor_teste, FLOR* flor_train){

  int setosa = 0; //numero de vezes que setosa aparece
  int versicolor = 0; //numero de vezes que versicolor aparece
  int virginica = 0; //numero de vezes que virginica aparece
  double acertos = 0; //conta quantos acertos foram obtidos pelo programa

  //verifica o tipo
  for(int i = 0; i < conta_quebras_test; i++){
    for(int j = 0; j < k; j++){
      //se for setosa, conta
      if(strlen(flor_train[matriz_posicoes[i][j]].name) == 6)
        setosa++;

      //se for virginica, conta
      else if(strlen(flor_train[matriz_posicoes[i][j]].name) == 9)
        virginica++;

      //se for versicolor, conta
      else if(strlen(flor_train[matriz_posicoes[i][j]].name) == 10)
        versicolor++;
    }

    //se contou mais setosa, printa setosa e depois printa a resposta do especialista
    if (setosa > versicolor && setosa > virginica){
      printf("setosa ");
        if (strlen(flor_teste[i].name) == 6)
          acertos++;
      printf("%s\n", flor_teste[i].name);
    }
    //se contou mais versicolor, printa versicolor e depois printa a resposta do especialista
    else if (versicolor > setosa && versicolor > virginica){
      printf("versicolor ");
        if (strlen(flor_teste[i].name) == 10)
          acertos++;
      printf("%s\n", flor_teste[i].name);
    }
    //se contou mais virginica, printa virginica e depois printa a resposta do especialista
    else if (virginica > setosa && virginica > versicolor){
      printf("virginica ");
        if(strlen(flor_teste[i].name) == 9)
          acertos++;
      printf("%s\n", flor_teste[i].name);
    }
    // se for igual, printa, entre os iguais, o primeiro que aparecer na linha uma vez que essa esta ordenada da menor distancia para a maior
    // as linhas abaixo realizam a busca do mais proximo dentro do limite do k e incrementam no numero de acertos, em caso de acerto
    else if (setosa == versicolor){
      for (int j = 0; j < k; j++){
        if (strlen(flor_train[matriz_posicoes[i][j]].name) == 6){
          printf("setosa ");
          printf("%s\n", flor_teste[i].name);
          if (strlen(flor_teste[i].name) == 6) //confere se acertou
            acertos++;
          break;
        }
        else if (strlen(flor_train[matriz_posicoes[i][j]].name) == 10){
          printf("versicolor ");
          printf("%s\n", flor_teste[i].name);
          if (strlen(flor_teste[i].name) == 10) //confere se acertou
            acertos++;
          break;
        }
      }
    }
    else if (setosa == virginica){
      for (int j = 0; j < k; j++){
        if (strlen(flor_train[matriz_posicoes[i][j]].name) == 6){
          printf("setosa ");
          printf("%s\n", flor_teste[i].name);
          if (strlen(flor_teste[i].name) == 6) //confere se acertou
            acertos++;
          break;
        }
        else if (strlen(flor_train[matriz_posicoes[i][j]].name) == 9){
          printf("virginica ");
          printf("%s\n", flor_teste[i].name);
          if (strlen(flor_teste[i].name) == 9) //confere se acertou
            acertos++;
          break;
        }
      }
    }
    else if (versicolor == virginica){
      for (int j = 0; j < k; j++){
        if (strlen(flor_train[matriz_posicoes[i][j]].name) == 10){
          printf("versicolor ");
          printf("%s\n", flor_teste[i].name);
          if (strlen(flor_teste[i].name) == 10) //confere se acertou
            acertos++;
          break;
        }
        else if (strlen(flor_train[matriz_posicoes[i][j]].name) == 9){
          printf("virginica ");
          printf("%s\n", flor_teste[i].name);
          if (strlen(flor_teste[i].name) == 9) //confere se acertou
            acertos++;
          break;
        }
      }
    }
    //resetando
    setosa = 0;
    virginica = 0;
    versicolor = 0;
  }

  //printa a taxa de acertos
  printf("%.4lf", acertos / conta_quebras_test);

}

int main(void){

  FILE* ftrain;//armazena o treino
  FILE* ftest;//armazena o teste
  int final;//variavel auxiliar para o fgets
  char* train;//nome do arquivo de treino
  char* test;//nome do arquivo de teste
  int conta_quebras_train;//conta quantos exemplos de treino tem
  int conta_quebras_test;//conta quantos exemplos de teste tem
  int k;//armazena o k
  int contador = 0;//contador para contar o numero de exemplos

  //alocando os arquivos na heap
  train = (char*) malloc(20 * sizeof(char));
  test = (char*) malloc(20 * sizeof(char));

  //pegando o nome dos arquivos a serem abertos
  scanf("%s ", train);
  scanf("%s ", test);

  //pegando o k
  scanf("%d", &k);

  //abrindo os arquivos de treino e teste
  if (DEBUG) printf("%s\n", train);
  if (DEBUG) printf("%s\n", test);
  ftrain = fopen(train, "r");
  ftest = fopen(test, "r");

  //contando o numero de exemplos de treino
  conta_quebras_train = n_quebras_linha(ftrain);

  //fazendo um vetor de struct na heap para armazenar os exemplos
  FLOR* flor_treino;
  FLOR* flor_teste;

  //armazenando na heap
  flor_treino = (FLOR*) malloc(conta_quebras_train * sizeof(FLOR));

  //contaondo o numero de exemplos de teste
  conta_quebras_test = n_quebras_linha(ftest);

  //se o k for menor que um ou maior que o numero de exemplos, printa que e invalido pois nao ha como procurar
  if (k < 1 || k > conta_quebras_test){
    printf("k is invalid\n");
    return 0;
  }

  //alocado o vetor na heap
  flor_teste = (FLOR*) malloc(conta_quebras_test * sizeof(FLOR));

  //criando matriz de distancia do tamanho do numero de exemplos de treino pelo numero do de exemplos de teste
  double distancia[conta_quebras_test][conta_quebras_train];
  //criando matriz de posicoes para sincronizar as mudancas da matriz de distancias com as posicoes, para saber qual distancia pertence a qual
  int matriz_posicoes[conta_quebras_test][conta_quebras_train];

  //enumerando os exemplos
  for(int i = 0; i < conta_quebras_test; i++){
    for (int j = 0; j < conta_quebras_train; j++) {
      matriz_posicoes[i][j] = j;
    }
  }

  //dando rewind para voltar ao comeco do arquivo, uma vez que uma das funcoes executadas anteriormente levou para o fim de ambos os arquivos para contar o numero de exemplos
  //(a funcao e' a "n_quebras_linha")
  rewind(ftrain);
  rewind(ftest);

  //funcao para escanear os exemplos que vem do arquivo de treino
  treinar(ftrain, &flor_treino);
  //funcao para escanear os exemplos que vem do arquivo de teste
  testar(ftest, &flor_teste);
  //funcao para remover as aspas do nome dos exemplos anteriormente dados
  remove_aspa(&flor_treino, &flor_teste, conta_quebras_train, conta_quebras_test);
  //funcao que calcula a distancia
  calcula_vizinhos(flor_treino, flor_teste, conta_quebras_train, conta_quebras_test, distancia);

  //auxiliar para reaalizar a troca do bubble sort
  double aux;

  //executando um bubble sort para ordenar as linhas e as colunas da matriz de distancia, sincronizando com a matriz de posicoes
  //fixando uma linha em i
  for(int i = 0; i < conta_quebras_test; i++){
    //mantendo uma coluna para comparar com os posteriores
    for(int j = 0; j < conta_quebras_train-1; j++){
      //comparando com os posteriores
      for(int k = j + 1; k < conta_quebras_train; k++){
        if(distancia[i][j] > distancia[i][k]){
          //relizando a troca na matriz de distancias
          aux = distancia[i][j];
          distancia[i][j] = distancia[i][k];
          distancia[i][k] = aux;
          //realizando a troca numa matriz de posicoes
          aux = matriz_posicoes[i][j];
          matriz_posicoes[i][j] = matriz_posicoes[i][k];
          matriz_posicoes[i][k] = aux;
        }
      }
    }
  }

  //checando os tipos de flores a partir da matriz de distancias (agora ordenada) e sincronizando com a matriz de posicoes, tambem ordenada
  checar(conta_quebras_test, conta_quebras_train, matriz_posicoes, distancia, &contador, k, flor_teste, flor_treino);

  //liberando o que foi alocado na heap
  free(train);
  free(test);
  free(flor_treino);
  free(flor_teste);
  fclose(ftrain);
  fclose(ftest);

  return 0;
}
