#include "feitv.h"

int main() {
    menuPrincipal();
    return 0;
}

void menuPrincipal() {
    int opcao;
    int idLogado;

    do {
    	system("cls");
    	
        printf("\n--- FEITV: Bem-vindo ---\n");
        printf("1 - Cadastrar Usuario\n");
        printf("2 - Efetuar Login\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                cadastrarUsuario();
                break;

            case 2:
                idLogado = realizarLogin();

                if (idLogado != 0) {
                    menuUsuario(idLogado);
                }
                break;

            case 0:
                printf("Encerrando programa...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 0);
}

void menuUsuario(int idUsuarioLogado) {
    int opcao;
    char busca[50];
    int idVideo;
    int idPlaylist;
    int idVideoRemover;

    do {
        printf("\n--- FEITV: Menu do Usuario ---\n");
        printf("1 - Buscar Video\n");
        printf("2 - Curtir Video\n");
        printf("3 - Descurtir Video\n");
        printf("4 - Criar Playlist\n");
        printf("5 - Ver Minhas Playlists\n");
        printf("6 - Adicionar Video\n");
        printf("7 - Remover Video\n");
        printf("8 - Excluir Playlist\n");
        printf("9 - Logout\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Digite o nome: ");
                scanf(" %[^\n]", busca);
                buscarVideo(busca);
                break;

            case 2:
                printf("ID do video: ");
                scanf("%d", &idVideo);
                curtirVideo(idUsuarioLogado, idVideo);
                break;

            case 3:
                printf("ID do video: ");
                scanf("%d", &idVideo);
                descurtirVideo(idUsuarioLogado, idVideo);
                break;

            case 4:
                criarPlaylist(idUsuarioLogado);
                break;

            case 5:
                listarMinhasPlaylists(idUsuarioLogado);
                break;

            case 6:
                printf("ID da playlist: ");
                scanf("%d", &idPlaylist);

                printf("ID do video: ");
                scanf("%d", &idVideo);

                adicionarVideoPlaylist(idPlaylist, idVideo, idUsuarioLogado);
                break;

            case 7:
                printf("ID da playlist: ");
                scanf("%d", &idPlaylist);

                printf("ID do video: ");
                scanf("%d", &idVideoRemover);

                removerVideoPlaylist(idPlaylist, idVideoRemover, idUsuarioLogado);
                break;

            case 8:
                printf("ID da playlist: ");
                scanf("%d", &idPlaylist);

                excluirPlaylist(idPlaylist, idUsuarioLogado);
                break;
        }
        
        if (opcao != 9) {
        	system("pause");
        	system("cls");
		}

    } while(opcao != 9);
}
