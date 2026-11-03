#include <stdio.h>
#include <string.h>
#include "../login/login.h"
#include "menu.h"
#include "../moedas/moedas.h"
#include "../dados/dados.h"

// Pede a senha de 6 dígitos novamente (exigido em saldo, extrato, compra e venda)
static bool confirmar_senha(int user_id) {
    int i = posicao_usuario_por_id(user_id);
    char senha[32];

    if (i == NAO_ENCONTRADO) {
        return false;
    }

    printf("Digite sua senha (6 dígitos): ");
    ler_palavra(senha);

    if (strcmp(senha, lista_usuarios[i].senha) != 0) {
        printf("Senha incorreta.\n");
        return false;
    }

    return true;
}

static float mostrar_erro_operacao(unsigned int codigo) {
    switch (codigo) {
        case OP_USUARIO_INEXISTENTE:
            printf("Usuário não encontrado.\n");
            break;
        case OP_VALOR_INVALIDO:
            printf("Valor inválido.\n");
            break;
        case OP_SALDO_INSUFICIENTE:
            printf("Saldo insuficiente.\n");
            break;
        case OP_MOEDA_INVALIDA:
            printf("Criptomoeda inválida.\n");
            break;
        default:
            printf("Erro ao gravar os dados no arquivo.\n");
            break;
    }

    return 0;
}

// Mostra as cotações e pergunta qual moeda. Devolve 0, 1 ou 2 (BTC, ETH, XRP),
// ou NUM_MOEDAS se o usuário cancelou ou digitou algo inválido.
static int escolher_moeda() {
    exibir_cotacoes();

    printf("Escolha a criptomoeda (1 a 3, ou 0 para cancelar): ");
    int opcao = ler_opcao();

    if (opcao == 0) {
        return NUM_MOEDAS;
    }

    if (opcao > NUM_MOEDAS) {
        printf("Opção inválida.\n");
        return NUM_MOEDAS;
    }

    return opcao - 1;
}

void menu_cadastrar() {
    char nome[64];
    char cpf[32];
    char senha[32];

    printf("Qual seu nome? (até 31 caracteres)\n> ");
    ler_nome(nome);

    printf("Digite seu CPF (11 dígitos, sem pontos e sem traços):\n> ");
    ler_palavra(cpf);

    printf("Crie uma senha numérica de 6 dígitos:\n> ");
    ler_palavra(senha);

    switch (cadastrar(nome, cpf, senha)) {
        case CAD_OK:
            printf("Cadastro realizado com sucesso! Faça login para continuar.\n");
            break;
        case CAD_CPF_INVALIDO:
            printf("CPF inválido: digite 11 números, sem pontos e sem traços.\n");
            break;
        case CAD_SENHA_INVALIDA:
            printf("Senha inválida: use exatamente 6 números.\n");
            break;
        case CAD_NOME_INVALIDO:
            printf("Nome inválido.\n");
            break;
        case CAD_CPF_EXISTENTE:
            printf("Já existe um usuário com esse CPF.\n");
            break;
        case CAD_LIMITE_USUARIOS:
            printf("Limite de usuários atingido.\n");
            break;
        default:
            printf("Erro ao gravar os dados no arquivo.\n");
            break;
    }
}

void menu_login() {
    char cpf[32];
    char senha[32];

    printf("Digite seu CPF (sem pontos e sem traços):\n> ");
    ler_palavra(cpf);

    printf("Digite sua senha (6 dígitos):\n> ");
    ler_palavra(senha);

    int user_id = login(cpf, senha);

    if (user_id == 0) {
        printf("CPF ou senha incorretos.\n");
        return;
    }

    int i = posicao_usuario_por_id(user_id);
    printf("\nBem-vindo(a), %s!\n", lista_usuarios[i].nome);

    // fica no menu do usuário até ele escolher "Sair"
    while (main_menu_pos_login(user_id) == 0) { }
}

void menu_consulta_saldo(int user_id) {
    if (!confirmar_senha(user_id)) {
        return;
    }

    exibir_saldo(user_id);
}

void menu_consulta_extrato(int user_id) {
    if (!confirmar_senha(user_id)) {
        return;
    }

    exibir_extrato(user_id);
}

void menu_depositar(int user_id) {
    printf("Valor do depósito (R$): ");

    double valor = ler_valor();
    if (valor <= 0) {
        printf("Valor inválido.\n");
        return;
    }

    int resultado = depositar(user_id, valor);

    if (resultado == OP_OK) {
        printf("Depósito realizado com sucesso!\n");
        exibir_saldo(user_id);
    } else {
        mostrar_erro_operacao(resultado);
    }
}

void menu_sacar(int user_id) {
    printf("Valor do saque (R$): ");

    double valor = ler_valor();
    if (valor <= 0) {
        printf("Valor inválido.\n");
        return;
    }

    int resultado = sacar(user_id, valor);

    if (resultado == OP_OK) {
        printf("Saque realizado com sucesso!\n");
        exibir_saldo(user_id);
    } else {
        mostrar_erro_operacao(resultado);
    }
}

void menu_compra(int user_id) {
    if (!confirmar_senha(user_id)) {
        return;
    }

    int tipo = escolher_moeda();
    if (tipo == NUM_MOEDAS) {
        return;
    }

    printf("Quanto deseja comprar em reais (R$)? ");
    double valor = ler_valor();

    if (valor <= 0) {
        printf("Valor inválido.\n");
        return;
    }

    double quantidade = valor / lista_cotacoes[tipo].valor;

    printf("Taxa de compra: %.0f%%\n", obter_taxa_compra(tipo) * 100);
    printf("Você receberá %.8f %s\n", quantidade, sigla_moeda[tipo]);
    printf("Você pagará R$ %.2f (valor + taxa)\n", valor_compra(valor, tipo));
    printf("Confirmar? (1-Sim / 2-Não): ");

    if (ler_opcao() != 1) {
        printf("Compra cancelada.\n");
        return;
    }

    int resultado = efetivar_compra(user_id, tipo, valor);

    if (resultado == OP_OK) {
        printf("Compra realizada com sucesso!\n");
        exibir_saldo(user_id);
    } else {
        mostrar_erro_operacao(resultado);
    }
}

void menu_venda(int user_id) {
    if (!confirmar_senha(user_id)) {
        return;
    }

    int tipo = escolher_moeda();
    if (tipo == NUM_MOEDAS) {
        return;
    }

    int i = posicao_usuario_por_id(user_id);
    double quantidade_atual = lista_usuarios[i].saldo_cripto[tipo];

    printf("Você possui %.8f %s ", quantidade_atual, sigla_moeda[tipo]);
    printf("(R$ %.2f)\n", quantidade_atual * lista_cotacoes[tipo].valor);

    printf("Quanto deseja vender em reais (R$)? ");
    double valor = ler_valor();

    if (valor <= 0) {
        printf("Valor inválido.\n");
        return;
    }

    double quantidade = valor / lista_cotacoes[tipo].valor;

    printf("Taxa de venda: %.0f%%\n", obter_taxa_venda(tipo) * 100);
    printf("Você venderá %.8f %s\n", quantidade, sigla_moeda[tipo]);
    printf("Você receberá R$ %.2f (valor - taxa)\n", valor_venda(valor, tipo));
    printf("Confirmar? (1-Sim / 2-Não): ");

    if (ler_opcao() != 1) {
        printf("Venda cancelada.\n");
        return;
    }

    int resultado = efetivar_venda(user_id, tipo, valor);

    if (resultado == OP_OK) {
        printf("Venda realizada com sucesso!\n");
        exibir_saldo(user_id);
    } else {
        mostrar_erro_operacao(resultado);
    }
}

void menu_atualizar_cotacao() {
    if (!atualizar_cotacoes()) {
        printf("Erro ao gravar as cotações no arquivo.\n");
    }

    exibir_cotacoes();
}