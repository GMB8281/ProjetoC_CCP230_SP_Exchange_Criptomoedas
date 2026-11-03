#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dados.h"

// Cotações iniciais (usadas quando cotacoes.csv ainda está vazio)
static void cotacoes_padrao(void) {
    lista_cotacoes[MOEDA_BTC].moeda_id = MOEDA_BTC;
    lista_cotacoes[MOEDA_BTC].valor = 350000.00;

    lista_cotacoes[MOEDA_ETH].moeda_id = MOEDA_ETH;
    lista_cotacoes[MOEDA_ETH].valor = 16108.00;

    lista_cotacoes[MOEDA_XRP].moeda_id = MOEDA_XRP;
    lista_cotacoes[MOEDA_XRP].valor = 2.57;
}

// Lê o usuarios.csv: cada linha do arquivo vira um usuário na lista
bool carregar_usuarios() {
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "r");
    if (arquivo == NULL) {
        return false;
    }

    total_usuarios = 0;
    char linha[512];

    // Pula a primeira linha (o cabeçalho); se o arquivo estiver vazio, não há o que ler
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fclose(arquivo);
        return true;
    }

    // Repete até acabar o arquivo ou o limite de usuários
    while (total_usuarios < MAX_USUARIOS && fgets(linha, sizeof(linha), arquivo) != NULL) {
        int i = total_usuarios;

        int lidos = sscanf(linha,
            "%d,%31[^,],%11[^,],%6[^,],%lf,%lf,%lf,%lf",
            &lista_usuarios[i].user_id,
            lista_usuarios[i].nome,
            lista_usuarios[i].CPF,
            lista_usuarios[i].senha,
            &lista_usuarios[i].saldo_real,
            &lista_usuarios[i].saldo_cripto[MOEDA_BTC],
            &lista_usuarios[i].saldo_cripto[MOEDA_ETH],
            &lista_usuarios[i].saldo_cripto[MOEDA_XRP]
        );

        if (lidos != 8) {
            break; // A linha está incompleta ou houve erro de formatação
        }

        total_usuarios++;
    }

    fclose(arquivo);
    return true;
}

// Lê o cotacoes.csv. Se estiver vazio ou incompleto, grava as cotações padrão.
bool carregar_cotacoes() {
    cotacoes_padrao();

    FILE *arquivo = fopen(ARQUIVO_COTACOES, "r");
    if (arquivo == NULL) {
        return false;
    }

    int id;
    int lidas = 0;
    double valor;
    char linha[128];

    // Pula a primeira linha (o cabeçalho) e lê o resto
    if (fgets(linha, sizeof(linha), arquivo) != NULL) {
        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
            if (sscanf(linha, "%d,%lf", &id, &valor) == 2) {
                if (id >= 0 && id < NUM_MOEDAS && valor > 0) {
                    lista_cotacoes[id].valor = valor;
                    lidas++;
                }
            }
        }
    }

    fclose(arquivo);

    if (lidas < NUM_MOEDAS) {
        return salvar_cotacoes();
    }

    return true;
}

// Lê o extrato.csv: cada linha do arquivo vira uma operação na lista
bool carregar_extrato() {
    FILE *arquivo = fopen(ARQUIVO_EXTRATO, "r");
    if (arquivo == NULL) {
        return false;
    }

    total_extrato = 0;
    char linha[512];

    // Pula a primeira linha (o cabeçalho); se o arquivo estiver vazio, não há o que ler
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fclose(arquivo);
        return true;
    }

    while (total_extrato < MAX_EXTRATO && fgets(linha, sizeof(linha), arquivo) != NULL) {
        int i = total_extrato;

        int lidos = sscanf(linha,
            "%u,%16[^,],%c,%lf,%4[^,],%lf,%lf,%lf,%lf,%lf,%lf",
            &lista_extrato[i].user_id,
            lista_extrato[i].data,
            &lista_extrato[i].sinal,
            &lista_extrato[i].valor,
            lista_extrato[i].ativo,
            &lista_extrato[i].cotacao,
            &lista_extrato[i].taxa,
            &lista_extrato[i].saldo_real,
            &lista_extrato[i].saldo_cripto[MOEDA_BTC],
            &lista_extrato[i].saldo_cripto[MOEDA_ETH],
            &lista_extrato[i].saldo_cripto[MOEDA_XRP]
        );

        if (lidos != 11) {
            break; // A linha está incompleta ou com erro
        }

        total_extrato++;
    }

    fclose(arquivo);
    return true;
}

// Carrega tudo. Retorna true se todos os arquivos foram lidos com sucesso.
// (os três são sempre carregados, mesmo que um deles falhe)
bool carregar_dados() {
    bool usuarios_ok = carregar_usuarios();
    bool cotacoes_ok = carregar_cotacoes();
    bool extrato_ok = carregar_extrato();

    return usuarios_ok && cotacoes_ok && extrato_ok;
}