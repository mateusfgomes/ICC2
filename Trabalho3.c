#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEBUG 0
#define MAX 600000

/*
Trabalho 3 - Corretor Automatico

Mateus Ferreira Gomes 						NUSP: 10734773

Instituto de Ciencias Matematicas e de Computacao - Universidade de Sao Paulo

Programa que trabalha como um corretor automatico de tweets. Recebe um arquivo de "dicionario"(que e' um livro) e outro
para leitura dos tweets, e identificacao de palavras "erradas" (que nao estao no livro). Ao retornar printa essas palavras
*/

//definindo estrutura dos arquivos a serem lidos (tweets e dicionario)
typedef struct files{

	char* name; //nome do arquivo
	char** words; //palavras contidas no arquivo
	int size; //quantidade de palavras

}FILES;

//lendo o dicionario, recebe como entrada o arquivo do dicionario e um vetor para guardar
//os dados
int read_file(FILE* fp, char** c){

	int i = 0; //variavel que armazena o numero de palavras

	//lendo ate o final do arquivo com mascara para ignorar caracteres especiais
	while(fscanf(fp, "%[a-zA-Z]%*[^a-zA-Z]", c[i]) != EOF){
		i++;
	}

	return i; //retornando a quantidade de palavras lidas
}

//funcao merge sort para ordenar o vetor de strings do dicionario
//recebe o vetor de palavras a serem ordenadas, o inicio e o fim desse
void merge_sort(char** vet, int start, int end){


	if(start < end){

		int middle = (start+end)/2.0; //pegando o meio do vetor

		merge_sort(vet, start, middle); //executando recursao na primeira e segunda metade do vetor
		merge_sort(vet, middle+1, end);

		char** aux; //criando varios vetores auxiliares (com a recursao) para ordenar o vetor principal

		aux = (char**) malloc((end-start+1)*sizeof(char*));
		for (int i = 0; i < (end-start+1); i++){
			aux[i] = (char*) malloc(45*sizeof(char));
		}

		int i = start;
		int j = middle+1;
		int k = 0;

		//comparando os vetores divididos para copiar ordenadamente para o vetor auxiliar
		while(i <= middle && j <= end){
			if(strcmp(vet[i], vet[j]) <= 0){
				strcpy(aux[k], vet[i]);
				i++;
			}
			else{
				strcpy(aux[k], vet[j]);
				j++;
			}
			k++;
		}
		while(i <= middle){
			strcpy(aux[k], vet[i]);
			i++;
			k++;
		}
		while(j <= end){
			strcpy(aux[k], vet[j]);
			j++;
			k++;
		}

		//copiando para o vetor principal
		for(i = start, k = 0; i <= end; i++, k++){
			strcpy(vet[i], aux[k]);
		}

		//liberando os auxiliares
		for(int l = 0; l < (end-start+1); l++){
			free(aux[l]);
		}
		free(aux);
	}
	else{
		return;
	}
}

//funcao que remove as palavras repetidas do dicionario, recebe como parametro: o vetor de palavras do dicionario
//o tamanho, e o novo tamanho que o vetor tera depois de retirar as palavras
char** remove_repeats(char** words, int size, int* newsize){

	char** aux;
	//vetor auxiliar para retirar as palavras
	aux = (char**) malloc(size*sizeof(char*));
	for(int i = 0; i < size; i++){
		aux[i] = (char*) malloc(45*sizeof(char));
	}
	//vetor para receber uma string e fazer a comparacao
	char* word_aux;
	word_aux = (char*) malloc(45*sizeof(char));

	int i = 0;
	int k = 0;

	//copiando a primeira palavra para o aux
	strcpy(word_aux, words[0]);

	//enquanto nao chegar no final do dicionario, continua
	while(i <= size){
		if(strcmp(word_aux, words[i]) == 0){ //se for igual, vai passando ate achar diferente
			i++;
		}
		else{ //achou uma diferente, poe no vetor auxiliar
			strcpy(aux[k], word_aux);
			strcpy(word_aux, words[i]);
			k++;
		}
	}

	//muda o tamanho do dicionario
	*newsize = k;

	//libera parte do novo dicionario para economizar memoria
	for (int i = k; i < size; i++){
		free(aux[i]);
	}

	//libera o dicionario antigo
	for (int i = 0; i < MAX; i++){
		free(words[i]);
	}
	free(words);
	free(word_aux);
	

	return aux;

}

//funcao que realiza a leitura do tweet
int read_file_tweet(FILE* fp, char** c){

	int i = 0;
	int j = 0;
	char aux; //auxiliar para fazer a leitura char por char
	int aux2 = 1; //auxiliar para controlar o numero de strings

	while(fscanf(fp, "%c", &aux) != EOF){
		//se for caractere especial que pode configurar o inicio de uma nova palavra, entao, comeca essa nova palavra
		if((((int)aux > 31 && (int)aux < 48) || (int)aux > 127 || (int)aux < 1) && aux2 == 0){
			aux2++; //para controlar o numero de palavras
			i++;
			j = 0;
		}
		//recebendo os caracteres letras nas palavras
		else if(((int)aux >= 65 && (int)aux <= 90) || ((int)aux >= 97 && (int)aux <= 122)){
			c[i][j] = aux;
			j++;
			aux2 = 0;
		}
		//ignorando os demais
		else{
			continue;
		}
	}

	//retornando a quantidade de palavras
	return i;

}

//realizando uma busca binaria, essa funcao recebe: o vetor de strings, a chave(string) a ser procurada, o inicio do vetor e o fim
int binary_search(char** vet, char* key, int start, int end){
	//define o meio
	int middle = (int) (start + end)/2.0;

	//se chegou no final e nao achou a palavra, retorna um
	if(start > end)
		return 1;

	//se achou, retorna zero
	if(strcmp(vet[middle], key) == 0){
		return 0;
	}

	//faz uma busca recursiva
	if(strcmp(vet[middle], key) > 0){
		return binary_search(vet, key, start, middle-1);
	}

	if(strcmp(vet[middle], key) < 0)
		return binary_search(vet, key, middle+1, end);

	//retorno padrao (nao vai ocorrer)
	return 5;


}

//funcao que procura pela pelo conteudo de um tweet no arquivo
//recebe como parametro: o dicionario, as palavras do tweet minusculas(tweets) e normais(tweets_temp), as posicoes onde comeca e termina
//o tweet(begin e finish), o numero de tweets (textcount) 
void search_text(char** dictionary, char** tweets, int* begin, int* finish, int dsize, int textcount, char** tweets_temp){

	//numero de vezes que executou o while (que examinou os tweets)
	int exec = 0;

	while(exec < textcount){
		int start; //para salvar o inicio do vetor
		int control; //para saber se a busca binaria achou ou nao a palavra. Caso sim, recebe 0; Caso nao, 1.

		//procurando pelo inicio do tweet
		while(strcmp(tweets[*begin], "text") != 0)
			*begin = *begin + 1;		
		

		start = *begin; //salvando o comeco
		*finish = *begin;
		
		//procurando pelo fim
		while(strcmp(tweets[start], "truncated") != 0){
			start++;
			*finish = *finish + 1;
		}

		*finish = *finish - 1;
		*begin = *begin + 1;

		//fazendo a busca binaria no intervano inicio-fim
		for(int i = *begin; i <= *finish; i++){
			control = binary_search(dictionary, tweets[i], 0, dsize);
			if(control == 1) //se nao achou, printa a palavra que nao achou
				printf("%s ", tweets_temp[i]);
		}
		exec++;
		printf("\n");
	}

	
}

int main(void){

	FILE* fdictionary; //ponteiro para arquivo do dicionario
	FILE* ftweets; //ponteiro para arquivo dos tweets
	FILES dictionary; //dicionario
	FILES tweets;  //tweets
	int newsize; //novo tamanho do dicionario
	int tweet_start = 0; //armazenara o comeco do tweet
	int tweet_end = 0; //e esse o final
	int textcount = 0; //numero de tweets
	char** tweets_temp; //armazena os tweets em tamanho normal

	//zerando o tamanho do dicionario
	dictionary.size = 0;
	//alocando o dicionario e os tweets na heap, tal como o nome de seus arquivos
	dictionary.words = (char**) malloc(MAX*sizeof(char*));
	for(int i = 0; i < MAX; i++)
		dictionary.words[i] = malloc(45*sizeof(char));

	tweets.words = (char**) malloc(50000*sizeof(char*));
	for(int i = 0; i < 50000; i++)
		tweets.words[i] = (char*) malloc(45*sizeof(char));

	dictionary.name = (char*) malloc(45*sizeof(char));
	tweets.name = (char*) malloc(30 * sizeof(char));

	//pegando o nome dos arquivos
	scanf("%s", dictionary.name);
	scanf("%s", tweets.name);

	//printando para debug os nomes
	if (DEBUG) printf("%s\n", dictionary.name);
	if (DEBUG) printf("%s\n", tweets.name);

	//abrindo os arquivos
	fdictionary = fopen(dictionary.name, "r");
	ftweets = fopen(tweets.name, "r");
	
	//lendo o arquivo de tweets e armazenando o numero de palavras
	tweets.size = read_file_tweet(ftweets, tweets.words);

	//contando o numero de tweets
	for(int i = 0; i < tweets.size; i++){
		if(strcmp(tweets.words[i], "text") == 0)
			textcount++;
	}

	//se nao tiver nenhum, fecha o programa
	if(textcount == 0){
		printf("No tweets to check\n");
		return 0;
	}

	//lendo o dicionario (livro) e armazenando o numero de palavras
	dictionary.size = read_file(fdictionary, dictionary.words);

	//deixando todas as palavras do dicionario minusculas
	for(int i = 0; i < dictionary.size; i++){
		for (int j = 0; j < 45; j++){
			int aux = (int) dictionary.words[i][j];
			if (aux >= 65 && aux <= 90){
				aux += 32; //por conta da tabela ASCII
				dictionary.words[i][j] = aux;
			}
		}
	}

	//alocando na heap o vetor que recebera as palavras do tweet na sua forma normal
	tweets_temp = (char**) malloc(tweets.size * sizeof(char*));
	for (int i = 0; i < tweets.size; i++){
		tweets_temp[i] = (char*) malloc(45*sizeof(char));
	}

	//copiando
	for (int i = 0; i < tweets.size; i++){
		strcpy(tweets_temp[i], tweets.words[i]);
	}

	//colocando todas as palavras do tweet em letras minusculas para comparar com as do dicionario
	for(int i = 0; i < tweets.size; i++){
		for (int j = 0; j < 45; j++){
			int aux = (int) tweets.words[i][j];
			if (aux >= 65 && aux <= 90){
				aux += 32;
				tweets.words[i][j] = aux;
			}
		}
	}

	//fazendo o merge sort para ordenar as palavras do dicionario
	merge_sort(dictionary.words, 0, dictionary.size-1);
	dictionary.words = remove_repeats(dictionary.words, dictionary.size, &newsize); //removendo as palavras repetidas
	dictionary.size = newsize; //recebendo o novo tamanho do dicionario
	//realizando as operacoes finais de comparar as palavras do dicionarios e do tweet, essa funcao tambem printa
	search_text(dictionary.words, tweets.words, &tweet_start, &tweet_end, dictionary.size, textcount, tweets_temp);

	//liberando o que foi armazenado na heap, nao ha' memory leak
	for(int i = 0; i < dictionary.size; i++)
		free(dictionary.words[i]);

	free(dictionary.words);

	for(int i = 0; i < 50000; i++)
		free(tweets.words[i]);

	free(tweets.words);

	for (int i = 0; i < tweets.size; i++)
		free(tweets_temp[i]);
	
	free(tweets_temp);
	free(dictionary.name);
	free(tweets.name);
	fclose(fdictionary);
	fclose(ftweets);

	return 0;
	//fim do programa
}
