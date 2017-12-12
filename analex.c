#include "analex.h"

//Varíavel Global para contar linha
linhas = 1;

//Função para adcionar string à tabela literal
int addTabelaLiteral(char adcionar[]){
    int pos = 0;

    //Encontrar o ultimo espaço vazio
    while(strcmp(tabLiteral[pos], "") != 0){
        pos++;
    }

    strcpy(tabLiteral[pos], adcionar);

    return pos;
}

int comparaPalavraReservada(char palavra[]){
    int i = 0;

    while(strcmp(PAL_RESERV[i], "") > 0) //enquanto houver strings no vetor
    {
        if(strcmp(palavra, PAL_RESERV[i]) == 0){
            return i; //retorna o indice se tiver na tabela de palavras reservadas
        }
        i++;
    }

    return -1; //se não estiver na tabela de palabras reservadas
}

void erroAnalisadorLexico(char descricao[]){
    printf("ERRO: %s na Linha %d\n", descricao, linhas);
    system("pause");
    exit(1);
}


token analisadorLexico(){
    /*DECLARAÇÕES*/
    int estado = 0;
    char ch, armazenarCaracter;
    int contadorLexema, contadorDigito, contadorLT;
    char digitos[TAM_DIGITOS], lexema[TAM_LEXEMA], stringLT[TAM_STRINGLT];

    /*ASSUME ESSES VALORES DO TOKEN, CASO NÃO MUDAREM, DEU ERRO*/
    token tkfunc;
    tkfunc.categoria = FIMARQ;
    tkfunc.cod = 0;

    /*PEGA O PRIMEIRO CARACTERE*/
    ch = fgetc(arquivo);

    /*CASO SEJA FIM DO ARQUIVO*/
    if(feof(arquivo)){
        return tkfunc;
    }

    /*LOOP-PRINCIPAL*/
    while(1){
        switch(estado){
        case 0: /*ESTADO INICIAL*/
            /*CASO SEJA BRANCO, \T OU \N*/
            if((ch == ' ') || (ch == '\t') || (ch == '\n')){
                estado = 0;

                /*CONTAR LINHAS*/
                if(ch == '\n'){
                    linhas++;
                }
            }
            /*CASO SEJA DIGITO*/
            else if(isdigit(ch)){
                estado = 3;
                contadorDigito = 0; //inicia o contador em 0
                digitos[contadorDigito] = ch; //armazena o digito na posição 0
            }
            /*CASO SEJA LETRA*/
            else if(isalpha(ch)){
                estado = 1;
                contadorLexema = 0; //inicia o contador em 0
                lexema[contadorLexema] = ch; //armazena a letra na posição 0
            }
            /*CASO SEJA PT_VIRG*/
            else if(ch == ';'){
                estado = 8;
                tkfunc.categoria = SN;
                tkfunc.cod = PT_VIRG;
                return tkfunc;
            }
            /*CASO SEJA VIRGULA*/
            else if(ch == ','){
                estado = 11;
                tkfunc.categoria = SN;
                tkfunc.cod = VIRG;
                return tkfunc;
            }
            /*CASO SEJA SOMA*/
            else if(ch == '+'){
                estado = 12;
                tkfunc.categoria = SN;
                tkfunc.cod = SOMA;
                return tkfunc;
            }
            /*CASO SEJA SUBTRAÇÃO*/
            else if(ch == '-'){
                estado = 13;
                tkfunc.categoria = SN;
                tkfunc.cod = SUB;
                return tkfunc;
            }
            /*CASO SEJA MULTIPLICAÇÃO*/
            else if(ch == '*'){
                estado = 14;
                tkfunc.categoria = SN;
                tkfunc.cod = MULT;
                return tkfunc;
            }
            /*CASO SEJA BARRA*/
            else if(ch == '/'){ estado = 15; }
            /*CASO SEJA ABRE PARENTESIS*/
            else if(ch == '('){
                estado = 19;
                tkfunc.categoria = SN;
                tkfunc.cod = PARENTESIS_ABRE;
                return tkfunc;
            }
            /*CASO SEJA FECHA PARENTESIS*/
            else if(ch == ')'){
                estado = 20;
                tkfunc.categoria = SN;
                tkfunc.cod = PARENTESIS_FECHA;
                return tkfunc;
            }
            /*CASO SEJA ABRE CHAVES*/
            else if(ch == '{'){
                estado = 21;
                tkfunc.categoria = SN;
                tkfunc.cod = CHAVES_ABRE;
                return tkfunc;
            }
            /*CASO SEJA FECHA CHAVES*/
            else if(ch == '}'){
                estado = 22;
                tkfunc.categoria = SN;
                tkfunc.cod = CHAVES_FECHA;
                return tkfunc;
            }
            /*CASO SEJA & */
            else if(ch == '&'){ estado = 23; }
            /*CASO SEJA | */
            else if(ch == '|'){ estado = 25; }
            /*CASO SEJA > */
            else if(ch == '>'){ estado = 27; }
            /*CASO SEJA < */
            else if(ch == '<'){ estado = 30; }
            /*CASO SEJA ! */
            else if(ch == '!'){ estado = 33; }
            /*CASO SEJA = */
            else if(ch == '='){ estado = 36; }
            /*CASO SEJA ABRE ASPAS DUPLAs " */
            else if(ch == '"'){
                estado = 39;
                contadorLT = -1;
            }
            /*CASO SEJA ABRE ASPAS SIMPLES ' */
            else if(ch == '\''){ estado = 42; }
            else{
                /*CASO SEJA FIM DO ARQUIVO*/
                if(feof(arquivo)){
                    return tkfunc;
                }
                erroAnalisadorLexico("Caractere não valido");

            }

            break; /*BREAK ESTADO 0*/

        case 1: /*ID OU PR*/

            /*CASO SEJA _ OU DIGITO OU LETRA CONTINUA NO ESTADO 1*/
            if((ch=='_') || (isdigit(ch)) || (isalpha(ch))){
                estado = 1;
                lexema[++contadorLexema] = ch;
            }else{
                estado = 2; //vai pro estado 2
                ungetc(ch, arquivo);
                lexema[++contadorLexema] = '\0'; //fecha a string
                int testar = comparaPalavraReservada(lexema); //compara com as palavras reservadas

                //SE ESTIVER NA LISTA DE TABELA RESERVADA
                if(testar>=0){
                    tkfunc.cod = testar; //indice da palavra reservada
                    tkfunc.categoria = PR;
                }else{
                    //Se não for palavra reservada, é ID
                    tkfunc.categoria = ID;
                    strcpy(tkfunc.lexema, lexema);
                }

                return tkfunc;

            }

            break; /*BREAK ESTADO 1 */

        case 3: /*CT_I, CT_R*/

            /*SE FOR DIGITO */
            if(isdigit(ch)){
                estado = 3;
                digitos[++contadorDigito] = ch;

            /*SE FOR PONTO */
            }else if(ch == '.'){
                estado = 4;
                digitos[++contadorDigito] = ch;

            /* SE FOR OUTRO ENTÃO É CT_I* */
            }else{
                estado = 5;
                ungetc(ch, arquivo);
                digitos[++contadorDigito] = '\0'; //fecha a string
                tkfunc.categoria = CT_I;
                tkfunc.valor_int = atoi(digitos);
                return tkfunc;
            }
            break; /*BREAK ESTADO 3 */

        case 4: /*SAIU DO 3 LENDO PONTO */

            /*SE LER DIGITO */
            if(isdigit(ch)){
                estado = 6;
                digitos[++contadorDigito] = ch;
            }
            break; /*BREAK ESTADO 4 */

        case 6: /* SAIU DO 4 E LEU DIGITO */

            /*SE LER DIGITO */
            if(isdigit(ch)){
                estado = 6;
                digitos[++contadorDigito] = ch;
            /*SE FOR OUTRO */
            }else{
                estado=7;
                ungetc(ch, arquivo);
                digitos[++contadorDigito]='\0'; //fecha a string
                tkfunc.categoria=CT_R; tkfunc.valor_real=atof(digitos);
                return tkfunc;
            }
            break; /*BREAK ESTADO 6 */

        case 15: /* LEU BARRA / */

            /*SE LER ASTERISCO* */
            if(ch == '*'){
                estado = 16;
            /*SE LER OUTRO ENTÃO É DIVISÃO */
            }else{
                estado = 18;
                ungetc(ch, arquivo);
                tkfunc.categoria = SN;
                tkfunc.cod = DIV;
                return tkfunc;
            }
            break; /*BREAK ESTADO 15 */

        /*COMENTÁRIO*/
        case 16: /* SAI DO 15 E LI ASTERISCO * */

            /*SE LER ASTERISCO* */
            if(ch == '*'){
                estado = 17;
            /*SE LER OUTRO CONTINUA NO ESTADO 16 */
            }else if(isprint(ch) || iscntrl(ch)){
                estado = 16;
                /*CONTA LINHA*/
                if(ch == '\n'){
                    linhas++;
                }
                /*SE FIM DO ARQUIVO*/
                if(feof(arquivo)){
                    return tkfunc;
                }
            }else{
                erroAnalisadorLexico("Erro no comentário!");
            }

            //se não contar, adcionar else if contando linha

            break; /*BREAK ESTADO 16 */

        case 17: /* SAI DO 16 E LEU ASTERISCO * */
            /*SE LER ASTERISCO* */
            if(ch == '*'){
                estado = 17;
            /*SE LER BARRA VAI PRO ESTADO 0 */
            }else if(ch == '/'){
                estado = 0;
            /*SE LER OUTRO VOLTA PRO ANTERIOR 16 */
            }else if(isprint(ch) || iscntrl(ch)){
                estado = 16;
                if(ch == '\n'){
                    linhas++;
                }
            }else{
                erroAnalisadorLexico("Erro no comentário!");
            }
            break; /*BREAK ESTADO 17 */

        case 23: /* LEU & E COMERCIAL */
            /*SE LER OUTRO &*/
            if(ch == '&'){
                estado = 24;
                tkfunc.categoria = SN;
                tkfunc.cod = AND;
                return tkfunc;
            }else{
                erroAnalisadorLexico("Erro!");
            }
            break; /*BREAK ESTADO 23 */

        case 25: /* LEU | BARRA VERTICAL */
            /*SE LER OUTRO |*/
            if(ch == '|'){
                estado = 26;
                tkfunc.categoria = SN;
                tkfunc.cod = OR;
                return tkfunc;
            }else{
                erroAnalisadorLexico("Erro!");
            }
            break; /*BREAK ESTADO 25 */

        case 27: /* LEU > */
            /*SE LER = É MAIOR_IG*/
            if(ch == '='){
                estado = 29;
                tkfunc.categoria = SN;
                tkfunc.cod = MAIOR_IG;
                return tkfunc;
            /*SE LER OUTRO* É MAIOR_Q */
            }else{
                estado = 28;
                ungetc(ch, arquivo);
                tkfunc.categoria = SN;
                tkfunc.cod = MAIOR_Q;
                return tkfunc;
            }
            break; /*BREAK ESTADO 27 */

        case 30: /* LEU < */
            /*SE LER = É MENOR_IG*/
            if(ch == '='){
                estado = 31;
                tkfunc.categoria = SN;
                tkfunc.cod = MENOR_IG;
                return tkfunc;
            /*SE LER OUTRO* É MENOR_Q */
            }else{
                estado = 32;
                ungetc(ch, arquivo);
                tkfunc.categoria = SN;
                tkfunc.cod = MENOR_Q;
                return tkfunc;
            }
            break; /*BREAK ESTADO 30 */

        case 33: /* LEU ! */
            /*SE LER = É DIFERENTE*/
            if(ch == '='){
                estado = 34;
                tkfunc.categoria = SN;
                tkfunc.cod = DIFERENTE;
                return tkfunc;
            /*SE LER OUTRO* É NEGAÇÃO */
            }else{
                estado = 35;
                ungetc(ch, arquivo);
                tkfunc.categoria = SN;
                tkfunc.cod = NEGACAO;
                return tkfunc;
            }
            break; /*BREAK ESTADO 33 */

        case 36: /* LEU = */
            /*SE LER = É COMPARAÇÃO*/
            if(ch == '='){
                estado = 37;
                tkfunc.categoria = SN;
                tkfunc.cod = COMPARA;
                return tkfunc;
            /*SE LER OUTRO* É ATRIB */
            }else{
                estado = 38;
                ungetc(ch, arquivo);
                tkfunc.categoria = SN;
                tkfunc.cod = ATRIB;
                return tkfunc;
            }
            break; /*BREAK ESTADO 36 */

        case 39: /* LEU " */
            /*SE LER " É PQ FECHOU STRING */
            if(ch == '"'){
                estado = 40;
                stringLT[++contadorLT] = '\0'; //fecha a string
                int aux = addTabelaLiteral(stringLT);
                tkfunc.categoria = CT_LT;
                tkfunc.posicao = aux;
                return tkfunc;
            /*SE LER OUTRO* CONTINUA NO 39 ATÉ */
            }else{
                estado = 39;
                stringLT[++contadorLT] = ch;
                if(ch == '\n'){
                    linhas++;
                }
                if(feof(arquivo)){
                    erroAnalisadorLexico("Cadeia mal formada");
                }
            }
            break; /*BREAK ESTADO 39 */

        case 42: /* LEU ' */
            /*SE LER \ */
            if (ch == '\\'){
                estado=45;
            /*SE LER OUTRO* */
            }else{
                estado=43;
                if(isprint(ch)){
                    armazenarCaracter = ch;
                }else{
                    erroAnalisadorLexico("Caracter mal formada");
                }
            }
            break; /*BREAK ESTADO 42 */

        case 43: /* LEU OUTRO* */
            /*SE LER ' */
            if (ch == '\''){
                estado=44;
                tkfunc.categoria = CT_C;
                tkfunc.valor_ch = armazenarCaracter;
                return tkfunc;
            /*SE LER OUTRO* */
            }else{
                erroAnalisadorLexico("Caracter mal formada");
            }
            break; /*BREAK ESTADO 43 */

        case 45: /* LEU \ */
            /* SE LEU 0 */
            if(ch == '0'){
                estado = 47;
            /* SE LEU n */
            }else if(ch == 'n'){
                estado = 46;
            }else{
                erroAnalisadorLexico("Caracter mal formada");
            }
            break; /*BREAK ESTADO 45 */

        case 47: /* LEU 0 */
            /* SE LEU ' */
            if (ch == '\'') {
                estado = 49;
                tkfunc.categoria = CT_C;
                tkfunc.valor_ch = '\0';
                return tkfunc;
            }else{
                erroAnalisadorLexico("Caracter mal formada");
            }
            break; /*BREAK ESTADO 47 */

        case 46: /* LEU n */
            /* SE LEU ' */
            if (ch == '\'') {
                //ao invés de retornar token, aumenta a linha
                //estado = 48;
                //tkfunc.categoria = CT_C;
                //tkfunc.valor_ch = '\n';
                estado = 0;
                linhas++;
            }else{
                erroAnalisadorLexico("Caracter mal formada");
            }
            break; /*BREAK ESTADO 46 */

        default:
            printf("Estado: %d ", estado);
            erroAnalisadorLexico("Estado nao encontrado.");

        }//fim-switch estado


        ch=fgetc(arquivo);
    }//fim-loop principal

}//fim-função analisador lexico

void analex(){
    tk = tknext;
    tknext = analisadorLexico();
}

void imprimirTK(token aux){

     printf("<");

        if(aux.categoria == PR){
            printf("PR, %s> ", PAL_RESERV[aux.cod]);
        }
        if(aux.categoria == ID){
            printf("ID, %s> ", aux.lexema);
        }
        if(aux.categoria == SN){
            printf("SN, %s> ", TABELASINAIS[aux.cod]);
        }
        if(aux.categoria == CT_I){
            printf("CT_I, %d> ", aux.valor_int);
        }
        if(aux.categoria == CT_R){
            printf("CT_R, %8.2f> ", aux.valor_real);
        }
        if(aux.categoria == CT_C){
            CT_C: printf("CT_C, %c> ", aux.valor_ch);
        }
        if(aux.categoria == CT_LT){
            CT_LT: printf("CT_LT, %s> ", tabLiteral[aux.posicao]);
        }
        if(aux.categoria == FIMARQ){
            printf("FIMARQ, 0>");
        }

        if(aux.categoria==FIMARQ){
            //exit(1);
        }
}

