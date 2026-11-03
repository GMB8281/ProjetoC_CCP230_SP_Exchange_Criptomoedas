#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "menu/menu.h"
#include "dados/dados.h"

int main() {
    srand(time(NULL)); // para números aleatórios

    // garante que os arquivos existem e carrega tudo para a memória
    if (!criar_arquivo_usuarios() ||
        !criar_arquivo_cotacoes() ||
        !criar_arquivo_extrato()) {
        printf("Erro ao criar os arquivos de dados.\n");
        return 1;
    }

    if (!carregar_dados()) {
        printf("Erro ao carregar os dados.\n");
        return 1;
    }

    while (!main_menu_login()) { }

    return 0;
}