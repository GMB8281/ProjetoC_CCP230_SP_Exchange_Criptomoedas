#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "moedas.h"
#include "../dados/dados.h"

// Adiciona uma operação ao extrato (memória + arquivo).
// Retorna true se deu certo.
bool registrar_extrato(int user_id, char sinal, double valor, const char *ativo, double cotacao, double taxa) {
    int i = posicao_usuario_por_id(user_id);
    int n = total_extrato; // próxima posição livre do extrato

    if (i == NAO_ENCONTRADO || n >= MAX_EXTRATO) {
        return false;
    }

    time_t agora = time(NULL);

    lista_extrato[n].user_id = user_id;
    strftime(lista_extrato[n].data, TAM_DATA, "%d-%m-%Y %H:%M", localtime(&agora));

    lista_extrato[n].sinal = sinal;
    lista_extrato[n].valor = valor;

    strcpy(lista_extrato[n].ativo, ativo);

    lista_extrato[n].cotacao = cotacao;
    lista_extrato[n].taxa = taxa;

    lista_extrato[n].saldo_real = lista_usuarios[i].saldo_real;

    for (int k = 0; k < NUM_MOEDAS; k++) {
        lista_extrato[n].saldo_cripto[k] = lista_usuarios[i].saldo_cripto[k];
    }

    if (!salvar_extrato_registro(n)) {
        return false;
    }

    total_extrato++; // só conta na memória se foi gravado no arquivo
    return true;
}

int depositar(int user_id, double valor) {
    int i = posicao_usuario_por_id(user_id);
    if (i == NAO_ENCONTRADO) return OP_USUARIO_INEXISTENTE;

    valor = arredondar_centavos(valor);
    if (valor <= 0) return OP_VALOR_INVALIDO;

    struct usuarios copia = lista_usuarios[i];

    lista_usuarios[i].saldo_real = lista_usuarios[i].saldo_real + valor;

    if (!registrar_extrato(user_id, '+', valor, "REAL", 0, 0)) {
        lista_usuarios[i] = copia;
        return OP_ERRO_ARQUIVO;
    }

    if (!salvar_usuarios()) {
        return OP_ERRO_ARQUIVO;
    }

    return OP_OK;
}

int sacar(int user_id, double valor) {
    int i = posicao_usuario_por_id(user_id);
    if (i == NAO_ENCONTRADO) return OP_USUARIO_INEXISTENTE;

    valor = arredondar_centavos(valor);
    if (valor <= 0) return OP_VALOR_INVALIDO;

    if (lista_usuarios[i].saldo_real < valor) {
        return OP_SALDO_INSUFICIENTE;
    }

    struct usuarios copia = lista_usuarios[i];

    lista_usuarios[i].saldo_real = lista_usuarios[i].saldo_real - valor;

    if (!registrar_extrato(user_id, '-', valor, "REAL", 0, 0)) {
        lista_usuarios[i] = copia;
        return OP_ERRO_ARQUIVO;
    }

    if (!salvar_usuarios()) {
        return OP_ERRO_ARQUIVO;
    }

    return OP_OK;
}

// Mostra o nome e o CPF (123.456.789-00) do usuário da posição i
static float exibir_dados_usuario(unsigned int i) {
    char *cpf = lista_usuarios[i].CPF;

    printf("\nNome: %s\n", lista_usuarios[i].nome);
    printf("CPF: ");
    printf("%c%c%c.", cpf[0], cpf[1], cpf[2]);
    printf("%c%c%c.", cpf[3], cpf[4], cpf[5]);
    printf("%c%c%c-", cpf[6], cpf[7], cpf[8]);
    printf("%c%c\n", cpf[9], cpf[10]);

    return 0;
}

float exibir_saldo(unsigned int user_id) {
    unsigned int i = posicao_usuario_por_id(user_id);

    if (i == NAO_ENCONTRADO) {
        printf("Usuário não encontrado.\n");
        return 1;
    }

    exibir_dados_usuario(i);

    printf("Reais: %.2f\n", lista_usuarios[i].saldo_real);
    printf("Bitcoin: %.8f\n", lista_usuarios[i].saldo_cripto[MOEDA_BTC]);
    printf("Ethereum: %.8f\n", lista_usuarios[i].saldo_cripto[MOEDA_ETH]);
    printf("Ripple: %.8f\n", lista_usuarios[i].saldo_cripto[MOEDA_XRP]);

    return 0;
}

float exibir_extrato(unsigned int user_id) {
    unsigned int i = posicao_usuario_por_id(user_id);

    if (i == NAO_ENCONTRADO) {
        printf("Usuário não encontrado.\n");
        return 1;
    }

    exibir_dados_usuario(i);

    unsigned int encontradas = 0;

    for (unsigned int n = 0; n < total_extrato; n++) {
        if (lista_extrato[n].user_id != user_id) {
            continue;
        }

        printf("%s ", lista_extrato[n].data);
        printf("%c ", lista_extrato[n].sinal);
        printf("%.2f ", lista_extrato[n].valor);
        printf("%s ", lista_extrato[n].ativo);
        printf("CT: %.2f ", lista_extrato[n].cotacao);
        printf("TX: %.2f ", lista_extrato[n].taxa);
        printf("REAL: %.2f ", lista_extrato[n].saldo_real);
        printf("BTC: %.8f ", lista_extrato[n].saldo_cripto[MOEDA_BTC]);
        printf("ETH: %.8f ", lista_extrato[n].saldo_cripto[MOEDA_ETH]);
        printf("XRP: %.8f\n", lista_extrato[n].saldo_cripto[MOEDA_XRP]);

        encontradas++;
    }

    if (encontradas == 0) {
        printf("Nenhuma operação registrada.\n");
    }

    return 0;
}