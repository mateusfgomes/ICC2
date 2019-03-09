#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
Trabalho 5

Mateus Ferreira Gomes					NºUSP: 10734773

Instituto de Ciencias Matematicas e de Computacao - ICMC/USP

Programa que funciona como um navegador onde voce pode abrir abas, altera'-las, em relacao a posicao,
reordenar conforme o dia, mes, hora e minuto que foram criadas, alem de printar.
A estrutura Lista Simplesmente Encadeada foi usada para realizar o trabalho, conforme as instrucoes.
*/

/*
- Definindo um no' e a estrutura de um site, o qual esta contido dentro desse no'
- Definindo a estrutura de uma lista, com um ponteiro para o inicio dela, para o fim
e o tamanho dessa
*/
typedef struct node_ NODE;

typedef struct site_{
	int hour; //hora da criacao
	int minute; //minuto da criacao
	int month; //mes da criacao
	int day; //dia da criacao
	char title[35]; //titulo do site
	char url[1025]; //url do site
	int number; //"fusao" das informacoes sobre a criacao
	/*
	Exemplo:
	Hora: 01
	Dia: 22
	Mes: 10
	Minuto: 21

	Number sera': 10220121
	*/
}SITE;

struct node_{
	SITE* site;
	NODE* next;
};

typedef struct list_{

	NODE* start;
	NODE* end;
	int size;

}LIST;

/*Funcao que cria um no' e o retorna */
NODE* create_node(){

	NODE* aux = (NODE*) malloc(sizeof(NODE));

	return aux;
}

/*Funcao que deleta um no'
@Parametros:
- Esse no' a ser deletado
*/
void delete_node(NODE* node){

	if(node != NULL){
		free(node->site);
		free(node);
		node = NULL;
	}
}

/*Funcao que checa se a lista esta vazia
@Parametros:
- Lista a ser checada
Retorno:
- 1, se estiver vazia
- 0, se nao estiver
*/
int empty_list(LIST* L){

	if(L->size == 0)
		return 1;

	else
		return 0;

}

/*Funcao que deleta a lista
@Parametros:
- Lista a ser deletada
*/
void delete_list(LIST *L){
	if(L != NULL && !empty_list(L)){
   		NODE *N = L->start, *aux;
    	while(N != NULL){
       	 	aux = N;
       	 	N = N->next;
        	delete_node(aux);
        }
    	L->start = NULL;
    	free(L);
		L = NULL;
	}
}

/*Funcao que cria uma lista vazia
Retorno:
- Lista criada
*/
LIST* create_list(void){
	LIST *L; 
	L = (LIST *) malloc(sizeof(LIST)); 
	if(L != NULL){
		L->start = NULL;
		L->end = NULL; 
		L->size = 0; 
	}	
	return(L);	
}

/*Funcao que cria um site
Retorno:
- Site criado a ser preenchido
*/
SITE* create_site(void){
	SITE* new;
	new = (SITE*) malloc(sizeof(SITE)); 
	if (new != NULL){
		new->hour = 0; 
		new->minute = 0;
		new->month = 0;
		new->day = 0;
		new->number = 0;
	}
	return new;
}

/*Funcao que abre uma aba no navegador
@Parametros:
- Navegador (Lista) que recebera a nova aba (No')

Le do stdin o site a ser inserido, cria um "Number" para o site, 
coloca em um no e adiciona o no' no fim da lista

Retorno:
- Lista com o no' adicionado
*/
LIST* open_tab(LIST* browser){

	SITE* add;
	char c;

	add = create_site();

	c = getchar();
	scanf("%[^\n]%c", add->title, &c);
	scanf("%[^\n]%c", add->url, &c);
	scanf("%d", &(add->day));
	scanf("%d", &(add->month));
	scanf("%d", &(add->hour));
	scanf("%d", &(add->minute));
	add->number = 0;

	add->number += add->month * pow(10, 6);
	add->number += add->day * pow(10, 4);
	add->number += add->hour * pow(10, 2);
	add->number += add->minute;


	NODE* new = create_node();
	
	new->site = add;
	
	/*Caso a lista esteja vazia*/
	if(browser == NULL){
		browser = create_list();
		browser->start = new;
		browser->end = new;
	}

	browser->size++;
	browser->end->next = new;
	browser->end = new;

	return browser;

}

/*Funcao que printa as abas abertas do navegador
@Parametros:
- O navegador (Lista) que tera os sites (No's) printados

Printa todos os no's ate' o fim da lista, com base no tamanho dela
*/
void print_sites(LIST* browser){

	int count = 0;
	NODE* aux = NULL;
	aux = browser->start;

	while(count < browser->size){
		printf("%s ", aux->site->title);
		printf("%s ", aux->site->url);
		printf("%02d/%02d ", aux->site->day, aux->site->month);
		printf("%02d:%02d\n", aux->site->hour, aux->site->minute);
		count++;
		aux = aux->next;
	}
	printf("\n");
}

/*Funcao que altera a ordem das abas do navegador
@Parametros:
- Navegador que tera a posicao de suas abas alterada

Le o nome da aba a alterar, verifica se ela existe, se existir,
muda sua posicao com base na escolhida
*/
void change_tab(LIST* browser){

	char search_title[35];
	NODE* aux = browser->start;
	NODE* back;
	NODE* save;
	NODE* aux2 = browser->start;
	char c;
	int new_position;
	int count = 1;
	int verify = 0;

	c = getchar();
	scanf("%[^\n]%c", search_title, &c);

	scanf("%d", &new_position);

	/*Procura a aba*/
	for(int i = 0; i < browser->size; i++){
		if(strcmp(search_title, aux->site->title) == 0)
			verify = 1;
		aux = aux->next;
	}
	aux = browser->start;

	/*Se existir*/
	if(verify == 1){
		while(strcmp(search_title, aux->site->title)){
			back = aux;
			aux = aux->next;
		}
		save = back;
		back->next = aux->next;

		/*Inserir no comeco*/
		if(new_position == 1){
			aux->next = browser->start;
			browser->start = aux;
			return;
		}

		/*Inserir no final*/
		else if(new_position >= browser->size){
			if(new_position == browser->size) browser->end = aux->next;
			browser->end->next = aux;
			browser->end = aux;
			return;
		}

		/*Inserir no meio*/
		else{
			while(count < new_position){
				back = aux2;
				aux2 = aux2->next;
				count++;
			}
			if(!(strcmp(browser->end->site->title, aux->site->title))){
				browser->end = save;
			}
			aux->next = aux2;
			back->next = aux;

			return;
		}
	}

	return;

}

/*Funcao que auxilia o radix sort pegar os digitos corretos a serem
ordenados

@Parametros:
- A base, isto e', para auxiliar a encontrar o digito
a ser pega do numero
- O numero que queremos escolher o digito

Retorno:
 - O digito
*/
int get_digits(int base, int number){

	int result = (int) (number/base)%10;

	return result;
}

/*Funcao recursiva que realiza o radix sort no navegador
@Parametros:
- A lista na qual sera realizada o radix sort
- Um vetor de listas para auxiliar na realizacao da ordenacao
- A base para ordenar em relacao ao digito certo

Ordena em relacao a um digito, chama, recursivamente, em seguida,
ordena em relacao ao proximo (direita)
*/
void sort_radix(LIST* browser, LIST** b, int base){

	if(base >= 100000000) return;

	int count = 0;
	int result;
	NODE* aux = NULL;
	NODE* save = NULL;
	aux = browser->start;

	/*Percorre a lista a ser ordenada*/
	while(count < browser->size){
		result = get_digits(base, aux->site->number);
		/*Se a lista do vetor de listas naquela posicao estiver vazia*/
		if(empty_list(b[result])){
			b[result]->start = aux;
			b[result]->end = aux;
			save = aux->next;
			aux->next = NULL;
			b[result]->size++;
		}
		/*Se a lista do vetor de listas auxiliar naquela posicao nao estiver vazia, insere no final*/
		else{
			b[result]->end->next = aux;
			b[result]->end = aux; 
			save = aux->next;
			aux->next = NULL;
			b[result]->size++;
		}
		aux = save;
		count++;
	}

	int flag = 0;

	/*Reunindo as listas de forma a criar uma so' ja ordenada*/
	for(int i = 0; i < 9; i++){
		if(!(empty_list(b[i]))){
			if(!(flag)) browser->start = b[i]->start;
			flag = 1;
			for(int j = i+1; j < 10; j++){
				if(!(empty_list(b[j]))){
					b[i]->end->next = b[j]->start;
					b[i]->end = b[j]->end;
					b[i]->end->next = NULL;
					browser->end = b[i]->end;

				}
			}
		}
	}

	/*Desfazendo o vetor auxiliar*/
	for(int i = 0; i < 10; i++){
		b[i]->start = NULL;
		b[i]->end = NULL;
		b[i]->size = 0;
	}

 	/*Chamando a recursao*/
	sort_radix(browser, b, base*10);
}

/*Funcao que cria um vetor auxiliar para chamar o radix sort
recursivamente e ordenar
@Parametros:
- Lista a ser ordenada
*/
void radix(LIST* browser){

	LIST** b = (LIST**) malloc(sizeof(LIST*)*10);

	for(int i = 0; i < 10; i++){
		b[i] = create_list();
	}

	sort_radix(browser, b, 1);

	for(int i = 0; i < 10; i++){
		free(b[i]);
	}

	free(b);

}

int main(void){
	
	LIST* browser = NULL;
	int option;

	/*Escaneando a opcao a ser realizada*/
	scanf("%d", &option);

	while(option != 5){
		switch(option){
			case 1:
				browser = open_tab(browser);
				break;

			case 2:
				change_tab(browser);
				break;

			case 3:
				radix(browser);
				break;

			case 4:
				print_sites(browser);
				break;
		}
		scanf("%d", &option);
	}

	/*Liberando a memoria*/
	delete_list(browser);


	return 0;
}