#include <stdio.h>

int main(){
	
	unsigned char analise[512]; //sequencia de bytes que serao analisados para procurar por uma imagem
	int auxiliar = 0; //auxiliar que contara o numero de imagens encontradas
	FILE* pic = NULL; //arquivo que aramazena as imagens encontradas

	//enquanto ha bytes, continue lendo o arquivo onde as imagens estao salvas
	while(fread(analise, sizeof(char), 512, stdin) != 0){
		//se achar algum desses bytes nessa ordem, e a imagem estiver fechada, crie uma nova, nomeando-a com auxilio do buffer
		if (analise[0] == 0xff && analise[1] == 0xd8 && analise[2] == 0xff){
			if (pic != NULL) fclose(pic);
			char buffer[8]; //usando o buffer
			sprintf(buffer, "%03d.jpg", auxiliar); //utilizando o buffer para dar nome ao arquivo
			pic = fopen(buffer, "wb"); //se nao existe o arquivo com aquele nome ainda, cria
			fwrite(analise, sizeof(char), 512, pic); // "escreve a imagem"
			auxiliar++; //aumenta um na contagem de imagens salvas
		}
		//se ha' imagem, a escreve
		else if (auxiliar != 0){
			fwrite(analise, sizeof(char), 512, pic);
		}
	}
	//se a imagem esta aberta, a fecha
	if (pic != NULL) fclose(pic);
	else if (pic == NULL) printf("Could not find pictures\n"); //se nao ha imagens, informa

	return 0;
}