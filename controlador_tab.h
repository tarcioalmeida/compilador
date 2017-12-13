#ifndef CONTROLADOR_TAB_H
#define CONTROLADOR_TAB_H
#define TAM_LEXEMA 1000
#define TAM_TABSIMB 1000
#define VERDADEIRO 1
#define FALSO 0

/*Estrutura da Tabela*/
typedef struct linhaTabSimb{
    int i; //Número da linha da tabela
    char lexema[TAM_LEXEMA]; //nome
    int tipo; //INT, FLOAT, CHAR, ETC...
    int escopo; //Global ou Local
    int categoria; //Função, Parametro, Variavel
    int zumbi; //Flag
    int prototipo;//flag
}Linha_TabSimb;

/*Enum para deixar o código claro */
enum{EMPILHAR = -2, DESEMPILHAR = -3, CONSULTAR = -4, CONSULTARPARAM = -50, CONSULTARFUNC = -51, CONSULTARPROTOTIPO = -52}operations;
enum{LOCAL = -6, GLOBAL = -7}escopo;
enum{SIM_ZUMBI = 1, NAO_ZUMBI = 0}zumbi;
enum{FUNC = -10, PARAM = -11, VAR = -12}categoria;
enum{PROTO = 1, N_PROTO =0}prototipo;

/*Declarações Globais*/
Linha_TabSimb tabela[TAM_TABSIMB];
int topo;

/*Assinatura das Funções*/
void iniciarTabelaDeSimbolos();
int controlador_TabSimb(int operation, char lexema[], int tipo, int escopo, int categoria, int zumbi, int prototipo);
void empilhar(char lexema[], int tipo, int escopo, int categoria, int zumbi, int prototipo);
void desempilhar();
int consultar(char *lexema, int escopo, int prototipo);
int consultarParam(char *lexema, int escopo);
int consultarFunc(char *lexema);
int consultarPrototipo(char *lexema);
int checarTipoParam(int indice, int tipo);
void imprimirTabela();
int PesquisarTipo(token tk);

#endif
