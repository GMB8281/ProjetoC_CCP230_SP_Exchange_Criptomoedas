#include <string.h>
#include <ctype.h>
#include "login.h"
#include "../dados/dados.h"

static bool so_digitos(const char *texto, int tamanho) {
    if ((int)strlen(texto) != tamanho) {
        return false;
    }

    for (int i = 0; i < tamanho; i++) {
        if (isdigit((unsigned char)texto[i]) == 0) {
            return false;
        }
    }

    return true;
}

bool cpf_valido(const char *cpf) {
    return so_digitos(cpf, TAM_CPF - 1);
}

bool senha_valida(const char *senha) {
    return so_digitos(senha, TAM_SENHA - 1);
}

int cadastrar(const char *nome, const char *cpf, const char *senha) {
    if (!cpf_valido(cpf)) return CAD_CPF_INVALIDO;
    if (!senha_valida(senha)) return CAD_SENHA_INVALIDA;
    if (nome[0] == '\0') return CAD_NOME_INVALIDO;
    if (posicao_usuario_por_cpf(cpf) != NAO_ENCONTRADO) return CAD_CPF_EXISTENTE;
    if (total_usuarios >= MAX_USUARIOS) return CAD_LIMITE_USUARIOS;

    int i = total_usuarios; // posição do novo usuário

    // salva na struct
    memset(&lista_usuarios[i], 0, sizeof(lista_usuarios[i])); // zera os saldos

    lista_usuarios[i].user_id = proximo_id_usuario();

    strncpy(lista_usuarios[i].nome, nome, TAM_NOME - 1);
    lista_usuarios[i].nome[TAM_NOME - 1] = '\0';

    strncpy(lista_usuarios[i].CPF, cpf, TAM_CPF - 1);
    lista_usuarios[i].CPF[TAM_CPF - 1] = '\0';

    strncpy(lista_usuarios[i].senha, senha, TAM_SENHA - 1);
    lista_usuarios[i].senha[TAM_SENHA - 1] = '\0';

    // a vírgula separa as colunas do CSV, então troca por espaço
    for (int k = 0; lista_usuarios[i].nome[k] != '\0'; k++) {
        if (lista_usuarios[i].nome[k] == ',') {
            lista_usuarios[i].nome[k] = ' ';
        }
    }

    total_usuarios++;

    // salva no arquivo (se falhar desfaz na memória)
    if (!salvar_usuarios()) {
        total_usuarios--;
        return CAD_ERRO_ARQUIVO;
    }

    return CAD_OK;
}

int login(const char *cpf, const char *senha) {
    // procura na struct
    int i = posicao_usuario_por_cpf(cpf);
    if (i == NAO_ENCONTRADO) {
        return 0;
    }

    if (strcmp(lista_usuarios[i].senha, senha) != 0) {
        return 0;
    }

    return lista_usuarios[i].user_id;
}