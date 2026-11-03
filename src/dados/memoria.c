#include <string.h>
#include "dados.h"

struct usuarios lista_usuarios[MAX_USUARIOS];
int total_usuarios = 0;

struct cotacoes lista_cotacoes[NUM_MOEDAS];

struct extrato lista_extrato[MAX_EXTRATO];
unsigned int total_extrato = 0;

const char *sigla_moeda[NUM_MOEDAS] = {"BTC", "ETH", "XRP"};
const char *nome_moeda[NUM_MOEDAS] = {"Bitcoin", "Ethereum", "Ripple"};

// Procura o usuário pelo id e devolve a posição dele na lista
int posicao_usuario_por_id(int user_id) {
    for (int i = 0; i < total_usuarios; i++) {
        if (lista_usuarios[i].user_id == user_id) {
            return i;
        }
    }
    return NAO_ENCONTRADO;
}

// Procura o usuário pelo CPF e devolve a posição dele na lista
int posicao_usuario_por_cpf(const char *cpf) {
    for (int i = 0; i < total_usuarios; i++) {
        if (strcmp(lista_usuarios[i].CPF, cpf) == 0) {
            return i;
        }
    }
    return NAO_ENCONTRADO;
}

// Próximo id livre (maior id existente + 1)
int proximo_id_usuario() {
    int maior = 0;

    for (int i = 0; i < total_usuarios; i++) {
        if (lista_usuarios[i].user_id > maior) {
            maior = lista_usuarios[i].user_id;
        }
    }

    return maior + 1;
}