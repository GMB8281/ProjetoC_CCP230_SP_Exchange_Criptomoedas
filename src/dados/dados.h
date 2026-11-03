#ifndef DADOS_H
#define DADOS_H

#define MAX_USUARIOS 100
#define MAX_EXTRATO  10000
#define NUM_MOEDAS   3

// Valor devolvido quando não acha o usuário (nenhuma posição válida chega a 100)
#define NAO_ENCONTRADO MAX_USUARIOS

#define TAM_NOME     32   // 31 caracteres + '\0'
#define TAM_CPF      12   // 11 dígitos + '\0'
#define TAM_SENHA    7    // 6 dígitos + '\0'
#define TAM_DATA     17   // "DD-MM-AAAA HH:MM" + '\0'
#define TAM_ATIVO    5    // "REAL", "BTC", "ETH", "XRP" + '\0'

// Índices das criptomoedas (usado em saldo_cripto[] e lista_cotacoes[])
#define MOEDA_BTC 0
#define MOEDA_ETH 1
#define MOEDA_XRP 2

// Arquivos e seus cabeçalhos
#define ARQUIVO_USUARIOS "usuarios.csv"
#define ARQUIVO_COTACOES "cotacoes.csv"
#define ARQUIVO_EXTRATO  "extrato.csv"

#define CABECALHO_USUARIOS "user_id,nome,CPF,password,saldo_real,saldo_btc,saldo_eth,saldo_xrp\n"
#define CABECALHO_COTACOES "moeda_id,valor\n"
#define CABECALHO_EXTRATO  "user_id,data,sinal,valor,ativo,cotacao,taxa,saldo_real,saldo_btc,saldo_eth,saldo_xrp\n"

// Estruturas de dados
struct usuarios {
    int user_id;
    char nome[TAM_NOME];
    char CPF[TAM_CPF];                  // 11 dígitos, sem pontos e sem traços
    char senha[TAM_SENHA];              // 6 dígitos
    double saldo_real;                  // saldo em reais
    double saldo_cripto[NUM_MOEDAS];    // 0=BTC, 1=ETH, 2=XRP
};

struct cotacoes {
    int moeda_id;              // 0=BTC, 1=ETH, 2=XRP
    double valor;                       // preço de 1 unidade, em reais
};

struct extrato {
    unsigned int user_id;
    char data[TAM_DATA];                // "DD-MM-AAAA HH:MM"
    char sinal;                         // '+' ou '-'
    double valor;                       // valor da operação, em reais
    char ativo[TAM_ATIVO];              // "REAL", "BTC", "ETH" ou "XRP"
    double cotacao;                     // cotação usada (0 para depósito/saque)
    double taxa;                        // taxa aplicada (0.02 = 2%)
    double saldo_real;                  // saldos DEPOIS da operação
    double saldo_cripto[NUM_MOEDAS];
};

// memoria.c
extern struct usuarios lista_usuarios[MAX_USUARIOS];
extern int total_usuarios;
extern struct cotacoes lista_cotacoes[NUM_MOEDAS];
extern struct extrato lista_extrato[MAX_EXTRATO];
extern unsigned int total_extrato;

extern const char *sigla_moeda[NUM_MOEDAS];   // "BTC", "ETH", "XRP"
extern const char *nome_moeda[NUM_MOEDAS];    // "Bitcoin", "Ethereum", "Ripple"

// memoria.c (devolvem a POSIÇÃO na lista, ou NAO_ENCONTRADO)
int posicao_usuario_por_id(int user_id);
int posicao_usuario_por_cpf(const char *cpf);
int proximo_id_usuario();

// arquivos.c (criar_* devolvem true se deu certo)
bool verificar_arquivo(const char *nome_arquivo);   // true = existe, false = não existe
bool criar_arquivo_usuarios();
bool criar_arquivo_cotacoes();
bool criar_arquivo_extrato();

// carrega_dados.c (devolvem true se deu certo)
bool carregar_usuarios();
bool carregar_cotacoes();
bool carregar_extrato();
bool carregar_dados();

// salva_dados.c (devolvem true se deu certo)
bool salvar_usuarios();
bool salvar_cotacoes();
bool salvar_extrato_registro(int posicao);

#endif