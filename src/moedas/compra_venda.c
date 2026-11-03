#include "moedas.h"
#include "../dados/dados.h"

// Tipos:
// 0 => Bitcoin
// 1 => Ethereum
// 2 => Ripple

double arredondar_centavos(double valor) {
    long long centavos;

    if (valor >= 0.0) {
        centavos = (long long)(valor * 100.0 + 0.5);
    } else {
        centavos = (long long)(valor * 100.0 - 0.5);
    }

    return (double)centavos / 100.0;
}

double obter_taxa_compra(int tipo) {
    switch (tipo) {
        case MOEDA_BTC: return 0.02;
        case MOEDA_ETH: return 0.01;
        case MOEDA_XRP: return 0.01;
        default: return 0;
    }
}

double obter_taxa_venda(int tipo) {
    switch (tipo) {
        case MOEDA_BTC: return 0.03;
        case MOEDA_ETH: return 0.02;
        case MOEDA_XRP: return 0.01;
        default: return 0;
    }
}

double valor_compra(double valor, int tipo) {
    return arredondar_centavos(valor * (1.0 + obter_taxa_compra(tipo)));
}

double valor_venda(double valor, int tipo) {
    return arredondar_centavos(valor * (1.0 - obter_taxa_venda(tipo)));
}

// Compra 'valor_reais' em criptomoeda. O usuário paga o valor + a taxa, em reais.
int efetivar_compra(int user_id, int tipo, double valor_reais) {
    int i = posicao_usuario_por_id(user_id);
    if (i == NAO_ENCONTRADO) return OP_USUARIO_INEXISTENTE;

    if (tipo < 0 || tipo >= NUM_MOEDAS) return OP_MOEDA_INVALIDA;

    valor_reais = arredondar_centavos(valor_reais);
    if (valor_reais <= 0) return OP_VALOR_INVALIDO;

    double cotacao = lista_cotacoes[tipo].valor;
    double taxa = obter_taxa_compra(tipo);
    double custo = valor_compra(valor_reais, tipo);

    if (lista_usuarios[i].saldo_real < custo) {
        return OP_SALDO_INSUFICIENTE;
    }

    struct usuarios copia = lista_usuarios[i]; // para desfazer se der erro ao gravar

    // 1. altera a memória
    lista_usuarios[i].saldo_real = lista_usuarios[i].saldo_real - custo;
    lista_usuarios[i].saldo_cripto[tipo] = lista_usuarios[i].saldo_cripto[tipo] + valor_reais / cotacao;

    // 2. registra no extrato (memória + arquivo)
    if (!registrar_extrato(user_id, '+', valor_reais, sigla_moeda[tipo], cotacao, taxa)) {
        lista_usuarios[i] = copia;
        return OP_ERRO_ARQUIVO;
    }

    // 3. grava o novo saldo no arquivo
    if (!salvar_usuarios()) {
        return OP_ERRO_ARQUIVO;
    }

    return OP_OK;
}

// Vende 'valor_reais' em criptomoeda. O usuário recebe o valor - a taxa, em reais.
int efetivar_venda(int user_id, int tipo, double valor_reais) {
    int i = posicao_usuario_por_id(user_id);
    if (i == NAO_ENCONTRADO) return OP_USUARIO_INEXISTENTE;

    if (tipo < 0 || tipo >= NUM_MOEDAS) return OP_MOEDA_INVALIDA;

    valor_reais = arredondar_centavos(valor_reais);
    if (valor_reais <= 0) return OP_VALOR_INVALIDO;

    double cotacao = lista_cotacoes[tipo].valor;
    double taxa = obter_taxa_venda(tipo);
    double quantidade = valor_reais / cotacao; // quanto de cripto será vendido
    double ganho = valor_venda(valor_reais, tipo);

    if (lista_usuarios[i].saldo_cripto[tipo] < quantidade) {
        return OP_SALDO_INSUFICIENTE;
    }

    struct usuarios copia = lista_usuarios[i];

    lista_usuarios[i].saldo_cripto[tipo] = lista_usuarios[i].saldo_cripto[tipo] - quantidade;
    lista_usuarios[i].saldo_real = lista_usuarios[i].saldo_real + ganho;

    if (!registrar_extrato(user_id, '-', valor_reais, sigla_moeda[tipo], cotacao, taxa)) {
        lista_usuarios[i] = copia;
        return OP_ERRO_ARQUIVO;
    }

    if (!salvar_usuarios()) {
        return OP_ERRO_ARQUIVO;
    }

    return OP_OK;
}