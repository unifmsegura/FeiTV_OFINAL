#include <stdio.h>
#include <string.h>
#include "feitv.h"

void buscarVideo(char nomeBusca[]) {
	FILE *arq = fopen("videos.txt", "r");
	
	Video v;
	int encontrado = 0;
	
	if (arq == NULL) {
		printf("Arquivo de videos nao encontrado.\n");
		return;
	}
	
	printf("\n--- Resultados de Busca ---\n");
	
	while(fscanf(arq, "%d %s %s %c %d %d %d %d",
	             &v.id,
	             v.titulo,
	             v.categoria,
	             &v.tipo,
	             &v.duracao,
	             &v.temporadas,
	             &v.episodios,
	             &v.likes) == 8) {
	             	
	             	if (strcmp(v.titulo, nomeBusca) == 0) {
	             		encontrado = 1;
	             		
	             		printf("ID: %d\n", v.id);
	             		printf("Titulo: %s\n", v.titulo);
	             		printf("Categoria: %s\n", v.categoria);
	             		
	             		if (v.tipo == 'F') {
	             			printf("Tipo: Filme\n");
	             			printf("Duracao: %d min\n", v.duracao);
						 } else if (v.tipo == 'S') {
						 	printf("Tipo: Serie\n");
						 	printf("Temporadas: %d\n", v.temporadas);
						 	printf("Episodios: %d\n", v.episodios);
						 }
						 
						 printf("Likes: %d\n", v.likes);
					 }
	}
				 
	fclose(arq);
				 
	if (!encontrado) {
	    printf("Nenhum video encontrado.\n");
	}
}
			
void curtirVideo(int idUsuarioLogado, int idProcurado) {
	FILE *arqCurtidas = fopen("curtidas.txt", "a+");
	int usuarioLido, videoLido;
	
	if (arqCurtidas == NULL) {
		printf("Erro ao abrir arquivo de curtidas.\n");
		return;
	}
	
	if(!videoExiste(idProcurado)) {
		printf("Video nao encontrado.\n");
		fclose(arqCurtidas);
		return;
	}
	
	// verifica se ja curtiu
	while (fscanf(arqCurtidas, "%d %d",
	              &usuarioLido,
	              &videoLido) == 2) {
	              	
	              	if (usuarioLido == idUsuarioLogado &&
					    videoLido == idProcurado) {
					    	
					    	printf("Voce ja curtiu esse video.\n");
					    	fclose(arqCurtidas);
					    	return;
						}
	}
	
	// curtida
	fprintf(arqCurtidas, "%d %d\n",
	        idUsuarioLogado,
	        idProcurado);
	        
	fclose(arqCurtidas);
	
	// atualiza likes
	FILE *arq = fopen("videos.txt", "r");
	
	Video catalogo[100];
	int i = 0, total = 0;
	
	if (arq == NULL) {
		printf("Arquivo nao encontrado.\n");
		return;
	}
	
	while(fscanf(arq, "%d %s %s %c %d %d %d %d",
	         &catalogo[i].id,
	         catalogo[i].titulo,
	         catalogo[i].categoria,
	         &catalogo[i].tipo,
	         &catalogo[i].duracao,
	         &catalogo[i].temporadas,
	         &catalogo[i].episodios,
	         &catalogo[i].likes) == 8) {
	         	
	         	if (catalogo[i].id == idProcurado) {
	         		catalogo[i].likes++;
				 }
				 
				 i++;
			 }
			 
	total = i;
	fclose(arq);
	
	arq = fopen("videos.txt", "w");
	
	for (i = 0; i < total; i++) {
		fprintf(arq, "%d %s %s %c %d %d %d %d\n",
		        catalogo[i].id,
		        catalogo[i].titulo,
		        catalogo[i].categoria,
		        catalogo[i].tipo,
		        catalogo[i].duracao,
		        catalogo[i].temporadas,
		        catalogo[i].episodios,
		        catalogo[i].likes);
 	}
 	
 	fclose(arq);
 	
 	printf("Like atualizado com sucesso!\n");
}
	
void descurtirVideo(int idUsuarioLogado, int idProcurado) {
	FILE *arqCurtidas = fopen("curtidas.txt", "r");
	FILE *temp = fopen("temp_curtidas.txt", "w");
	
	int usuarioLido, videoLido;
	int encontrouCurtida = 0;
	
	if (arqCurtidas == NULL || temp == NULL) {
		printf("Erro ao abrir arquivo de curtidas.\n");
		return;
	}
	
	// remove curtida do usuario
	while (fscanf(arqCurtidas, "%d %d",
	             &usuarioLido,
	             &videoLido) == 2) {
	             	
	    if (usuarioLido == idUsuarioLogado &&
			videoLido == idProcurado) {
					    	
			encontrouCurtida = 1; // nao ggrava no temp = remove
						
		} else {
			fprintf(temp, "%d %d\n",
					usuarioLido,
					videoLido);
		}
	}
					
	fclose(arqCurtidas);
	fclose(temp);
	
	remove("curtidas.txt");
	rename("temp_curtidas.txt", "curtidas.txt");
	
	if (!encontrouCurtida) {
		printf("Voce ainda nao curtiu esse video.\n");
		return;
	}
	
	// diminui o like
	FILE *arq = fopen("videos.txt", "r");
	
	Video catalogo[100];
	int i = 0, total = 0;
	
	if (arq == NULL) {
		printf("Arquivo nao encontrado.\n");
		return;
	}
	
	while (fscanf(arq, "%d %s %s %c %d %d %d %d",
	              &catalogo[i].id,
	              catalogo[i].titulo,
	              catalogo[i].categoria,
	              &catalogo[i].tipo,
	              &catalogo[i].duracao,
	              &catalogo[i].temporadas,
	              &catalogo[i].episodios,
	              &catalogo[i].likes) == 8) {
	              	
	              	if (catalogo[i].id == idProcurado &&
	              	    catalogo[i].likes > 0) {
	              	    	catalogo[i].likes--;
					}
					
					i++;
	}
	
	total = i;
	fclose(arq);
	
	arq = fopen("videos.txt", "w");
	
	for (i = 0; i < total; i++) {
		fprintf(arq, "%d %s %s %c %d %d %d %d\n",
		        catalogo[i].id,
		        catalogo[i].titulo,
		        catalogo[i].categoria,
		        catalogo[i].tipo,
		        catalogo[i].duracao,
		        catalogo[i].temporadas,
		        catalogo[i].episodios,
		        catalogo[i].likes);
	}
	
	fclose(arq);
	
	printf("Video descurtido com sucesso!\n");
}

int videoExiste(int idVideo) {
	FILE *arq = fopen("videos.txt", "r");
	Video v;
	
	if (arq == NULL) return 0;
	
	while (fscanf(arq, "%d %s %s %c %d %d %d %d",
	              &v.id,
	              v.titulo,
	              v.categoria,
	              &v.tipo,
	              &v.duracao,
	              &v.temporadas,
	              &v.episodios,
	              &v.likes) == 8) {
	              	
	              	if (v.id == idVideo) {
	              		fclose(arq);
	              		return 1;
					  }
	}
	
	fclose(arq);
	return 0;
}
