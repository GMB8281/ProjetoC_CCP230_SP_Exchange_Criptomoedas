#include <stdio.h>
#include "menu.h"

bool main_menu_login() {
    printf("1. Fazer login\n2. Cadastre-se\n3. Encerrar programa\n");

    switch (ler_opcao()) {
        case 1:
            menu_login();
            return false;
        case 2:
            menu_cadastrar();
            return false;
        case 3:
            printf("Até logo!\n");
            return true;
        default:
            printf("Opção inválida.\n");
            return false;
    }
}

bool main_menu_pos_login(int user_id) {
    printf("\n1. Consultar saldo\n2. Consultar extrato\n3. Depositar\n4. Sacar\n5. Comprar criptomoedas\n6. Vender criptomoedas\n7. Atualizar cotação\n8. Sair\n");

    switch (ler_opcao()) {
        case 1:
            menu_consulta_saldo(user_id);
            return false;
        case 2:
            menu_consulta_extrato(user_id);
            return false;
        case 3:
            menu_depositar(user_id);
            return false;
        case 4:
            menu_sacar(user_id);
            return false;
        case 5:
            menu_compra(user_id);
            return false;
        case 6:
            menu_venda(user_id);
            return false;
        case 7:
            menu_atualizar_cotacao();
            return false;
        case 8:
            printf("Sessão encerrada.\n");
            return true;
        default:
            printf("Opção inválida.\n");
            return false;
    }
}