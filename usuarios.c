#include <stdio.h>
#include <string.h>
#include "feitv.h"

void cadastrarUsuario() {
    FILE *arq = fopen("usuarios.txt", "a");

    int id;
    char nome[50];
    char email[100];
    char senha[30];

    if (arq == NULL) {
        printf("Erro ao abrir banco de dados!\n");
        return;
    }

    printf("\n--- Cadastro de Novo Usuario ---\n");

    do {
	    printf("ID do usuario: ");
        scanf("%d", &id);

        if (idUsuarioExiste(id)) {
            printf("ID ja cadastrado.\n");
       }
	
	} while (idUsuarioExiste(id));

    printf("Nome: ");
    scanf("%s", nome);

    printf("Email: ");
    scanf("%s", email);

    printf("Senha: ");
    scanf("%s", senha);

    // salva COM id
    fprintf(arq, "%d %s %s %s\n", id, email, senha, nome);

    fclose(arq);

    printf("Usuario cadastrado com sucesso!\n");
}

int realizarLogin() {
    FILE *arq = fopen("usuarios.txt", "r");

    int idArquivo;
    char emailArquivo[100];
    char senhaArquivo[30];
    char nomeArquivo[50];

    char emailInformado[100];
    char senhaInformada[30];

    if (arq == NULL) {
        printf("Nenhum usuario cadastrado.\n");
        return 0;
    }

    printf("Email: ");
    scanf("%s", emailInformado);

    printf("Senha: ");
    scanf("%s", senhaInformada);

    while (fscanf(arq, "%d %s %s %s",
                  &idArquivo,
                  emailArquivo,
                  senhaArquivo,
                  nomeArquivo) == 4) {

        if (strcmp(emailInformado, emailArquivo) == 0 &&
            strcmp(senhaInformada, senhaArquivo) == 0) {

            printf("Bem-vindo, %s!\n", nomeArquivo);
            fclose(arq);

            return idArquivo; // retorna o ID real
        }
    }

    fclose(arq);
    printf("Email ou senha incorretos.\n");
    return 0;
}

int idUsuarioExiste(int id) {
    FILE *arq = fopen("usuarios.txt", "r");
    int idLido;
    char email[100], senha[30], nome[50];

    if (arq == NULL) return 0;

    while (fscanf(arq, "%d %s %s %s",
                  &idLido,
                  email,
                  senha,
                  nome) == 4) {

        if (idLido == id) {
            fclose(arq);
            return 1;
        }
    }

    fclose(arq);
    return 0;
}
