#ifndef MOEDAS_H
#define MOEDAS_H

#include <stdbool.h>

// Códigos de retorno das operações financeiras
#define OP_OK                  0
#define OP_USUARIO_INEXISTENTE 1
#define OP_VALOR_INVALIDO      2
#define OP_SALDO_INSUFICIENTE  3
#define OP_MOEDA_INVALIDA      4
#define OP_ERRO_ARQUIVO        5

// compra_venda.c
double arredondar_centavos(double valor);
double obter_taxa_compra(int tipo);
double obter_taxa_venda(int tipo);
double valor_compra(double valor, int tipo);      // quanto o usuário PAGA (valor + taxa)
double valor_venda(double valor, int tipo);       // quanto o usuário RECEBE (valor - taxa)

int efetivar_compra(int user_id, int tipo, double valor_reais);       // devolve um código OP_*
int efetivar_venda(int user_id, int tipo, double valor_reais);        // devolve um código OP_*

// cotacao.c
double gerar_numero_aleatorio();
bool atualizar_cotacoes();                // true se gravou no arquivo
float exibir_cotacoes();

// saldo_extrato.c
bool registrar_extrato(int user_id, char sinal, double valor, const char *ativo, double cotacao, double taxa);
int depositar(int user_id, double valor);       // devolve um código OP_*
int sacar(int user_id, double valor);            // devolve um código OP_*
float exibir_saldo(unsigned int user_id);
float exibir_extrato(unsigned int user_id);

#endif