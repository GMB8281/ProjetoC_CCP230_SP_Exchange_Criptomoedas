#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dados.h"

// Gerais

// Retorna true se o arquivo existe e false se não existe
bool verificar_arquivo(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo != NULL) {
        fclose(arquivo);
        return true;
    } else {
        return false;
    }
}

// Cria o arquivo com o cabeçalho, se ele ainda não existir. Retorna true se deu certo.
static bool criar_arquivo(const char *nome_arquivo, const char *cabecalho) {
    if (verificar_arquivo(nome_arquivo)) {
        return true; // Arquivo já existe
    }

    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        return false; // Erro ao criar arquivo
    }

    fprintf(arquivo, "%s", cabecalho);
    fclose(arquivo);
    return true;
}

// Usuários do sistema
bool criar_arquivo_usuarios() {
    return criar_arquivo(ARQUIVO_USUARIOS, CABECALHO_USUARIOS);
}

// Cotações
bool criar_arquivo_cotacoes() {
    return criar_arquivo(ARQUIVO_COTACOES, CABECALHO_COTACOES);
}

// Extrato
bool criar_arquivo_extrato() {
    return criar_arquivo(ARQUIVO_EXTRATO, CABECALHO_EXTRATO);
}