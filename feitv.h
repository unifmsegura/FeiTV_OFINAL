#ifndef FEITV_H
#define FEITV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char titulo[50];
    char categoria[30];
    char tipo;      // F ou S
    int duracao;    // filme
    int temporadas; // série
    int episodios;  // série
    int likes;
} Video;

typedef struct {
    int id_p;
    char nome_p[50];
    int id_u;
    char lista_videos[100];
} Playlist;

/* menus */
void menuPrincipal();
void menuUsuario(int idUsuarioLogado);

/* usu?rios */
void cadastrarUsuario();
int realizarLogin();
int idUsuarioExiste(int id);

/* v?deos */
void buscarVideo(char nomeBusca[]);
void curtirVideo(int idUsuarioLogado, int idProcurado);
void descurtirVideo(int idUsuarioLogado, int idProcurado);
int videoExiste(int idVideo);

/* playlists */
void listarMinhasPlaylists(int idUsuarioLogado);
void criarPlaylist(int idUsuarioLogado);
void adicionarVideoPlaylist(int idPlaylistAlvo, int idNovoVideo, int idUsuarioLogado);
void removerVideoPlaylist(int idPlaylistAlvo, int idVideoRemover, int idUsuarioLogado);
void excluirPlaylist(int idPlaylistExcluir, int idUsuarioLogado);

#endif
