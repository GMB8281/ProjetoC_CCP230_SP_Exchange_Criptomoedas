#include <stdio.h>
#include <stdlib.h>
#include "../dados/dados.h"

// Número aleatório entre -0.05 e +0.05 (variação de -5% a +5%)
double gerar_numero_aleatorio() {
    return ((double)rand() / RAND_MAX) * 0.1 - 0.05;
}

// Varia a cotação de todas as moedas e grava no arquivo
bool atualizar_cotacoes() {
    for (int i = 0; i < NUM_MOEDAS; i++) {
        lista_cotacoes[i].valor = lista_cotacoes[i].valor * (1.0 + gerar_numero_aleatorio());
    }

    return salvar_cotacoes();
}

float exibir_cotacoes() {
    printf("\n--- Cotações atuais ---\n");

    for (unsigned int i = 0; i < NUM_MOEDAS; i++) {
        printf("%u. ", i + 1);
        printf("%s ", nome_moeda[i]);
        printf("(%s): ", sigla_moeda[i]);
        printf("R$ %.2f\n", lista_cotacoes[i].valor);
    }

    return 0;
}