#ifndef LOGIN_H
#define LOGIN_H

// Códigos de retorno de cadastrar()
#define CAD_OK              0
#define CAD_CPF_INVALIDO    1
#define CAD_SENHA_INVALIDA  2
#define CAD_NOME_INVALIDO   3
#define CAD_CPF_EXISTENTE   4
#define CAD_LIMITE_USUARIOS 5
#define CAD_ERRO_ARQUIVO    6

bool cpf_valido(const char *cpf);
bool senha_valida(const char *senha);
int cadastrar(const char *nome, const char *cpf, const char *senha);   // devolve um código CAD_*
int login(const char *cpf, const char *senha);   // retorna o user_id, ou 0 se falhar

#endif