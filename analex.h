#ifndef _ANALEX_H
#define _ANALEX_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define TAM 1001
#define TAM_LEXEMA 1000
#define TAM_DIGITOS 1000
#define TAM_STRINGLT 1000
#define TAM_TABLITERAL 1000
#define QTD_PALAVRASRESERV 20
#define TAM_TABSIMB 1000
#define VERDADEIRO 1
#define FALSO 0

//STRUCT DO TIPO TOKEN
typedef struct tk{
    int cod;
    int categoria;
    int valor_int;
    float valor_real;
    char valor_ch;
    char lexema[TAM];
    int posicao;
}token;

//ENUM CATEGORIAS
typedef enum{
    ID,
    PR,
    SN,
    CT_C,
    CT_LT,
    CT_I,
    CT_R,
    LT,
    FIMARQ
}categToken;

//ENUM SINAL
typedef enum{
    PT_VIRG,
    VIRG,
    SOMA,
    SUB,
    MULT,
    DIV,
    PARENTESIS_ABRE,
    PARENTESIS_FECHA,
    CHAVES_ABRE,
    CHAVES_FECHA,
    AND,
    OR,
    MAIOR_Q,
    MAIOR_IG,
    MENOR_Q,
    MENOR_IG,
    NEGACAO,
    DIFERENTE,
    ATRIB,
    COMPARA
}sinal;

//Tabela de Sinais
char TABELASINAIS[22][10] = {
    ";",
    ",",
    "+",
    "-",
    "*",
    "/",
    "(",
    ")",
    "{",
    "}",
    "&&",
    "||",
    ">",
    ">=",
    "<",
    "<=",
    "!",
    "!=",
    "=",
    "=="
};

//Palavras Resevadas
char PAL_RESERV[QTD_PALAVRASRESERV][100] = {
    "booleano",
    "caracter",
    "digito",
    "enquanto",
    "id",
    "intcon",
    "inteiro",
    "letra",
    "para",
    "prototipo",
    "real",
    "realcon",
    "retorne",
    "se",
    "semretorno",
    "semparam",
    "senao"
};

//Variáveis Globais
token tk, tknext;
FILE *arquivo;
int linhas;
char tabLiteral[TAM_TABLITERAL][TAM_TABLITERAL]; //Tabela de Cadeias Literais

//ASSINATURA DE FUNÇÕES
int addTabelaLiteral(char adcionar[]);
int comparaPalavraReservada(char palavra[]);
void erroAnalisadorLexico(char descricao[]);
token analisadorLexico();
void analex();
void imprimirTK(token aux);
#endif
