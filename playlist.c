#include <stdio.h>
#include <string.h>
#include "feitv.h"

void removerVideoPlaylist(int idPlaylistAlvo, int idVideoRemover, int idUsuarioLogado) {
    FILE *arq = fopen("playlist.txt", "r");

    Playlist playlists[100];
    int i = 0;
    int total = 0;
    int encontrada = 0;

    if (arq == NULL) {
        printf("Arquivo de playlist nao encontrado.\n");
        return;
    }

    // l? todas as playlists
    while (fscanf(arq, "%d %s %d %s",
                  &playlists[i].id_p,
                  playlists[i].nome_p,
                  &playlists[i].id_u,
                  playlists[i].lista_videos) != EOF) {
        i++;
    }

    total = i;
    fclose(arq);

    // procura a playlist
    for (i = 0; i < total; i++) {
        if (playlists[i].id_p == idPlaylistAlvo &&
		playlists[i].id_u == idUsuarioLogado) {
            encontrada = 1;

            char novaLista[100] = "{";
            char copia[100];
            int primeiro = 1;

            strcpy(copia, playlists[i].lista_videos);

            // remove { e }
            copia[strlen(copia) - 1] = '\0';

            char *token = strtok(copia + 1, ",");

            while (token != NULL) {
                int idAtual = atoi(token);

                if (idAtual != idVideoRemover) {
                    if (!primeiro) {
                        strcat(novaLista, ",");
                    }

                    strcat(novaLista, token);
                    primeiro = 0;
                }

                token = strtok(NULL, ",");
            }

            strcat(novaLista, "}");
            strcpy(playlists[i].lista_videos, novaLista);
        }
    }

    if (!encontrada) {
        printf("Playlist nao encontrada ou nao pertence a voce.\n");
        return;
    }

    // regrava arquivo
    arq = fopen("playlist.txt", "w");

    for (i = 0; i < total; i++) {
        fprintf(arq, "%d %s %d %s\n",
                playlists[i].id_p,
                playlists[i].nome_p,
                playlists[i].id_u,
                playlists[i].lista_videos);
    }

    fclose(arq);

    printf("Video removido da playlist com sucesso!\n");
}

void excluirPlaylist(int idPlaylistExcluir, int idUsuarioLogado) {
    FILE *arq = fopen("playlist.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    Playlist p;
    int achou = 0;

    if (arq == NULL || temp == NULL) {
        printf("Erro ao abrir banco de dados!\n");
        return;
    }

    while (fscanf(arq, "%d %s %d %s",
                  &p.id_p,
                  p.nome_p,
                  &p.id_u,
                  p.lista_videos) == 4) {

        if (p.id_p == idPlaylistExcluir) {

            if (p.id_u == idUsuarioLogado) {
                achou = 1; // exclui
            } else {
                printf("Voce nao pode excluir a playlist de outro usuario.\n");
                fprintf(temp, "%d %s %d %s\n",
                        p.id_p, p.nome_p, p.id_u, p.lista_videos);
            }

        } else {
            fprintf(temp, "%d %s %d %s\n",
                    p.id_p, p.nome_p, p.id_u, p.lista_videos);
        }
    }

    fclose(arq);
    fclose(temp);

    remove("playlist.txt");
    rename("temp.txt", "playlist.txt");

    if (achou) {
        printf("Playlist excluida com sucesso!\n");
    }
}

void listarMinhasPlaylists(int idUsuarioLogado) {
    FILE *arq = fopen("playlist.txt", "r");
    Playlist p;
    int encontrou = 0;

    if (arq == NULL) {
        printf("Nenhuma playlist cadastrada.\n");
        return;
    }

    printf("\n--- Minhas Playlists ---\n");

    while (fscanf(arq, "%d %s %d %s",
                  &p.id_p,
                  p.nome_p,
                  &p.id_u,
                  p.lista_videos) == 4) {

        if (p.id_u == idUsuarioLogado) {
            printf("ID: %d\n", p.id_p);
            printf("Nome: %s\n", p.nome_p);
            printf("Videos: %s\n", p.lista_videos);

            char copia[100];
            strcpy(copia, p.lista_videos);

            copia[strlen(copia) - 1] = '\0';

            char *token = strtok(copia + 1, ",");

            while (token != NULL) {
                int idVideo = atoi(token);

                FILE *arqVideos = fopen("videos.txt", "r");
                Video v;

                if (arqVideos != NULL) {
                    while (fscanf(arqVideos,
                                  "%d %s %s %c %d %d %d %d",
                                  &v.id,
                                  v.titulo,
                                  v.categoria,
                                  &v.tipo,
                                  &v.duracao,
                                  &v.temporadas,
                                  &v.episodios,
                                  &v.likes) == 8) {

                        if (v.id == idVideo) {
                            printf("%d - %s, ",
                                   v.id,
                                   v.titulo);

                            if (v.tipo == 'F') {
                                printf("Filme\n");
                            } else {
                                printf("Serie\n");
                            }

                            break;
                        }
                    }

                    fclose(arqVideos);
                }

                token = strtok(NULL, ",");
            }

            printf("------------------------\n");
            encontrou = 1;
        }
    }

    fclose(arq);

    if (!encontrou) {
        printf("Voce nao possui playlists.\n");
    }
}

void criarPlaylist(int idUsuarioLogado) {
    FILE *arqLeitura = fopen("playlist.txt", "r");
    FILE *arqEscrita;

    Playlist p, temp;
    int idExiste = 0;

    printf("\n--- Criar Nova Playlist ---\n");

    printf("ID da Playlist: ");
    scanf("%d", &p.id_p);

    // verifica se ID j? existe
    if (arqLeitura != NULL) {
        while (fscanf(arqLeitura, "%d %s %d %s",
                      &temp.id_p,
                      temp.nome_p,
                      &temp.id_u,
                      temp.lista_videos) == 4) {

            if (temp.id_p == p.id_p) {
                idExiste = 1;
                break;
            }
        }

        fclose(arqLeitura);
    }

    if (idExiste) {
        printf("Erro: ja existe uma playlist com esse ID.\n");
        return;
    }

    printf("Nome da Playlist: ");
    scanf("%s", p.nome_p);

    p.id_u = idUsuarioLogado;

    printf("IDs dos videos (ex: {1,2,3}): ");
    scanf("%s", p.lista_videos);

    arqEscrita = fopen("playlist.txt", "a");

    if (arqEscrita == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    fprintf(arqEscrita, "%d %s %d %s\n",
            p.id_p,
            p.nome_p,
            p.id_u,
            p.lista_videos);

    fclose(arqEscrita);

    printf("Playlist criada com sucesso!\n");
}

void adicionarVideoPlaylist(int idPlaylistAlvo, int idNovoVideo, int idUsuarioLogado) {
	FILE *arq = fopen("playlist.txt", "r");
	
	Playlist playlists[100]; // vetor que carrega os dados na ram (aula 06)
	int i = 0;
	int total = 0;
	int encontrada = 0;
	
	char novoIdStr[20];
	
	if (arq == NULL) {
		printf("Arquivo de playlist nao encontrado.\n");
		return;
	}
	
	// lendo todas as playlists do arq (aula 08)
	while (fscanf(arq, "%d %s %d %s", 
	              &playlists[i].id_p, 
				  playlists[i].nome_p, 
				  &playlists[i].id_u, 
				  playlists[i].lista_videos) != EOF) {
		// se for playlist alvo, modifica a string (aula 07)
		if (playlists[i].id_p == idPlaylistAlvo &&
		playlists[i].id_u == idUsuarioLogado) {
			// remove '}' para concatenar
			int tam = strlen(playlists[i].lista_videos); // calcula comprimento
			
			if (tam > 0 && playlists[i].lista_videos[tam - 1] == '}') {
				playlists[i].lista_videos[tam - 1] = '\0'; // corta o fecha chaves
			}
			
			// formata o novo ID e anexa usando strcat [5, 10]
			sprintf(novoIdStr, ",%d}", idNovoVideo);
			strcat(playlists[i].lista_videos, novoIdStr);
			
			encontrada = 1;
		}
		
		i++;
	}
	
	total = i;
	fclose(arq);
	
	if (!encontrada) {
		// sobrescreve arq com dados atualizados
		printf("Playlist nao encontrada.\n");
		return;
	}
	
		arq = fopen("playlist.txt", "w");
		
		for (i = 0; i < total; i ++) {
			fprintf(arq, "%d %s %d %s\n", 
			        playlists[i].id_p, 
					playlists[i].nome_p, 
					playlists[i].id_u, 
					playlists[i].lista_videos);
		}
		
		fclose(arq);
		
		printf("Video %d adicionado com sucesso!\n", idNovoVideo);
}
