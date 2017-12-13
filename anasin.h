#ifndef _ANASIN_H
#define _ANASIN_H
#include "analex.c"
#include "controlador_tab.c"
#include "controlador_tab.h"

enum{
	BOOLEANO=0,
	CARACTER,
	DIGITO,
	ENQUANTO,
	IDPAL,
	INTCON,
	INTEIRO,
	LETRA,
	PARA,
	PROTOTIPO,
	REAL,
	REALCON,
	RETORNE,
	SE,
	SEMRETORNO,
	SEMPARAM,
	SENAO
}PalavrasReservadas;

/*Assinatura de Funções*/
void erroSintatico(char erro[]);
int termo();
int fator();
int expr();
int expr_simp();
int opr_rel();
int tipo();
void tipos_param();
void tipos_p_opc();
void atrib();
void cmd();
void prog();
#endif
