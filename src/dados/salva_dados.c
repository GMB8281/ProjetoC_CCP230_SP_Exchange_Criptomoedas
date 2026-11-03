#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dados.h"

// Regrava o usuarios.csv inteiro a partir da memória
bool salvar_usuarios() {
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "w");
    if (arquivo == NULL) {
        return false;
    }

    fprintf(arquivo, "%s", CABECALHO_USUARIOS);

    for (int i = 0; i < total_usuarios; i++) {
        fprintf(arquivo, "%d,", lista_usuarios[i].user_id);
        fprintf(arquivo, "%s,", lista_usuarios[i].nome);
        fprintf(arquivo, "%s,", lista_usuarios[i].CPF);
        fprintf(arquivo, "%s,", lista_usuarios[i].senha);
        fprintf(arquivo, "%.2f,", lista_usuarios[i].saldo_real);
        fprintf(arquivo, "%.10f,", lista_usuarios[i].saldo_cripto[MOEDA_BTC]);
        fprintf(arquivo, "%.10f,", lista_usuarios[i].saldo_cripto[MOEDA_ETH]);
        fprintf(arquivo, "%.10f\n", lista_usuarios[i].saldo_cripto[MOEDA_XRP]);
    }

    fclose(arquivo);
    return true;
}

// Regrava o cotacoes.csv inteiro a partir da memória
bool salvar_cotacoes() {
    FILE *arquivo = fopen(ARQUIVO_COTACOES, "w");
    if (arquivo == NULL) {
        return false;
    }

    fprintf(arquivo, "%s", CABECALHO_COTACOES);

    for (int i = 0; i < NUM_MOEDAS; i++) {
        fprintf(arquivo, "%d,", lista_cotacoes[i].moeda_id);
        fprintf(arquivo, "%.6f\n", lista_cotacoes[i].valor);
    }

    fclose(arquivo);
    return true;
}

// Acrescenta UMA operação (a da posição indicada) no fim do extrato.csv
bool salvar_extrato_registro(int posicao) {
    FILE *arquivo = fopen(ARQUIVO_EXTRATO, "a");
    if (arquivo == NULL) {
        return false;
    }

    fprintf(arquivo, "%u,", lista_extrato[posicao].user_id);
    fprintf(arquivo, "%s,", lista_extrato[posicao].data);
    fprintf(arquivo, "%c,", lista_extrato[posicao].sinal);
    fprintf(arquivo, "%.2f,", lista_extrato[posicao].valor);
    fprintf(arquivo, "%s,", lista_extrato[posicao].ativo);
    fprintf(arquivo, "%.6f,", lista_extrato[posicao].cotacao);
    fprintf(arquivo, "%.2f,", lista_extrato[posicao].taxa);
    fprintf(arquivo, "%.2f,", lista_extrato[posicao].saldo_real);
    fprintf(arquivo, "%.10f,", lista_extrato[posicao].saldo_cripto[MOEDA_BTC]);
    fprintf(arquivo, "%.10f,", lista_extrato[posicao].saldo_cripto[MOEDA_ETH]);
    fprintf(arquivo, "%.10f\n", lista_extrato[posicao].saldo_cripto[MOEDA_XRP]);

    fclose(arquivo);
    return true;
}