#ifndef MENU_H
#define MENU_H

// entrada.c
float limpar_buffer();                  // descarta o resto da linha digitada
float ler_nome(char *nome);             // lê um texto com espaços (até 63 caracteres)
float ler_palavra(char *palavra);       // lê uma palavra sem espaços (até 31 caracteres)
int ler_opcao();                        // retorna o número digitado, ou 0 se for inválido
double ler_valor();                     // retorna o valor em reais, ou -1 se for inválido

// menu_principal.c
bool main_menu_login();                  // retorna true para encerrar o programa
bool main_menu_pos_login(int user_id);   // retorna true quando o usuário escolhe "Sair"

// submenu.c
void menu_cadastrar();
void menu_login();
void menu_consulta_saldo(int user_id);
void menu_consulta_extrato(int user_id);
void menu_depositar(int user_id);
void menu_sacar(int user_id);
void menu_compra(int user_id);
void menu_venda(int user_id);
void menu_atualizar_cotacao();

#endif