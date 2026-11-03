#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "../moedas/moedas.h"

// Chamada quando o teclado acaba (Ctrl+D / Ctrl+Z): encerra para não ficar em loop infinito
static float fim_da_entrada() {
    printf("\nEntrada encerrada.\n");
    exit(0); // os dados já são gravados a cada operação
    return 0;
}

// Descarta o que sobrou na linha depois de um scanf
float limpar_buffer() {
    int c = getchar();

    while (c != '\n' && c != EOF) {
        c = getchar();
    }

    return 0;
}

static double ler_numero_da_linha() {
    char linha[128];

    if (fgets(linha, sizeof(linha), stdin) == NULL) {
        fim_da_entrada();
    }

    size_t tam = strlen(linha);

    // Se a linha for maior do que o buffer e não tiver terminado com '\n',descarta o restante da entrada e considera o valor inválido.
    if (tam > 0 && linha[tam - 1] != '\n' && feof(stdin) == 0) {
        limpar_buffer();
        return -1.0;
    }

    // Remove o final da linha ('\n' e possivelmente '\r')
    while (tam > 0 && (linha[tam - 1] == '\n' || linha[tam - 1] == '\r')) {
        linha[--tam] = '\0';
    }

    // Troca a vírgula decimal pelo ponto decimal.
    for (size_t i = 0; i < tam; i++) {
        if (linha[i] == ',') {
            linha[i] = '.';
        }
    }

    char *fim;
    errno = 0;

    double numero = strtod(linha, &fim);

    // Permite espaços depois do número, mas não outros caracteres
    while (*fim == ' ' || *fim == '\t') {
        fim++;
    }

    //Inválido
    if (errno != 0 || fim == linha || *fim != '\0') {
        return -1.0;
    }

    return numero;
}

float ler_nome(char *nome) {
    if (scanf(" %63[^\n]", nome) == EOF) {
        fim_da_entrada();
    }

    limpar_buffer();
    return 0;
}

float ler_palavra(char *palavra) {
    if (scanf("%31s", palavra) == EOF) {
        fim_da_entrada();
    }

    limpar_buffer();
    return 0;
}

int ler_opcao() {
    int opcao = 0;

    int lidos = scanf("%d", &opcao);
    if (lidos == EOF) {
        fim_da_entrada();
    }

    limpar_buffer();

    if (lidos != 1 || opcao < 0) {
        return 0; // não digitou um número (ou digitou um negativo)
    }

    return opcao;
}

double ler_valor() {
    double valor = ler_numero_da_linha();

    if (valor >= 0) {
        return arredondar_centavos(valor);
    }

    return -1; // não era um número válido, ou era negativo
}