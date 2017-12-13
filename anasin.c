#include "anasin.h"

void erroSintatico(char erro[]){

    printf("\nERRO SINTÁTICO na Linha %d: %s", linhas, erro);

    system("pause");
    exit(1);
}

void erroSemantico(char erro[]){

    printf("\nERRO SEMANTICO na Linha %d: %s", linhas, erro);

    system("pause");
    exit(1);
}

/*OK*/
int opr_rel(){
    if(tk.categoria == SN)
    {
        if(tk.cod == COMPARA || tk.cod == DIFERENTE || tk.cod == MAIOR_IG || tk.cod == MAIOR_Q || tk.cod == MENOR_IG || tk.cod == MENOR_Q)
        {
            return 1;
        }else{
            erroSintatico("Esperado operador relacional");
        }
    }else{
        erroSintatico("Esperado sinal");
    }

    return 0;
}

/*OK*/
int termo(){
    int tipo1, tipo2;

    tipo1 = fator();
    printf("\n****CODIGO T1 EM TERMO: %d\n",tipo1);

    //Se o proximo token for um sinal
    if(tknext.categoria == SN && (tknext.cod == MULT || tknext.cod == DIV || tknext.cod == AND)){


            analex();//vai pro sinal

            while(1){
                analex();
                tipo2 = fator();
                printf("\n****CODIGO T2 EM TERMO: %d\n",tipo2);
                if((tipo1==INTEIRO && (tipo2==BOOLEANO || tipo2==CARACTER)) || (tipo1==BOOLEANO && tipo2==INTEIRO))// SE FOR INTEIRO  E O OUTRO FOR UM BOOLEANO OU CARACTER
                {
                    if(!(tknext.cod == MULT || tknext.cod == DIV || tknext.cod == AND)){
                            break;
                        }
                    analex(); //vai pro sinal

                }else if(tipo1==CARACTER && tipo2==INTEIRO)//SE FOR CARACTER E O OUTRO UM INTEIRO
                {
                    if(!(tknext.cod == MULT || tknext.cod == DIV || tknext.cod == AND)){
                            break;
                        }
                    analex(); //vai pro sinal

                }else if(tipo1 == tipo2)
                {
                        if(!(tknext.cod == MULT || tknext.cod == DIV || tknext.cod == AND)){
                            break;
                        }
                    analex(); //vai pro sinal

                }else{
                    erroSemantico("Tipos de variavéis diferentes termo");
                }
                /*if(!(tknext.cod == MULT || tknext.cod == DIV || tknext.cod == AND)){
                    break;
                }
                analex(); //vai pro sinal*/

            }//fim-while


    }//fimSe o proximo token for um sinal

    return tipo1;
}

/*OK*/
int fator(){
    int tipo;

    /*Se for Inteiro, Real ou Caractere*/
    if(tk.categoria == CT_I || tk.categoria == CT_R || tk.categoria == CT_C  || tk.categoria == CT_LT){

        if(tk.categoria == CT_I) return INTEIRO;
        if(tk.categoria == CT_C) return CARACTER;
        if(tk.categoria == CT_R) return REAL;
    }

    /*Se for ID*/
    else if(tk.categoria == ID){


        if(!(tknext.categoria == SN && tknext.cod == PARENTESIS_ABRE)){
            //Se for somente ID
            printf("\n%c\n", tk.lexema);
            tipo = PesquisarTipo(tk);

            return tipo;
        }


        //Se for abrir parentesis
        if(tknext.categoria == SN && tknext.cod ==  PARENTESIS_ABRE){

            //vai pro parentesis
            analex();

            //printf("\n ta no abre parentesis");
            //Se o próximo token for fecha parentesis
            if(tknext.categoria == SN && tknext.cod ==  PARENTESIS_FECHA){
                //Ve o proximo token e sai
                //printf("\n nada dentro do parentesis");
                analex();
                return 1;
            }

            //Chama a função de expressÃ£o
            analex();
            expr();

            //Se o proximo token for um sinal E Se esse proximo token for VIRG
            if(tknext.categoria == SN && tknext.cod == VIRG){
                analex(); //vai pra virgula

                //vai chamando expr
                while(1){
                    //vai pra expr
                    analex();
                    expr();

                    if(!(tknext.categoria == SN && tknext.cod == VIRG)){
                        break;
                    }
                    analex(); //vai pra virgula

                }//fim-while
            }//fimSe o proximo token for um sinal E Se esse proximo token for VIRG

            //If para checar se houve fechar parentesis
            if(!(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)){
                //Erro faltando parentesis
                erroSintatico("Falta fecha parentesis");
            }
            //Se for fecha parentesis
            else{
                //Vai pro fecha parentesis
                analex();
                return 1;
            }

        }//FIM-Se for abrir parentesis

    }//Fim-Se for ID

    /* Se for somente ( expr ) */
    //Checar se houve abre parentesis
    else if(tk.categoria == SN && tk.cod ==  PARENTESIS_ABRE){
        analex();
        expr();

        //If para checar se houve fechar parentesis
        if(!(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)){
            //Erro faltando parentesis
            erroSintatico("Falta fecha parentesis");
        }
        //Se for fecha parentesis
        else{
            //Vai pro fecha parentesis
            analex();
            return 1;
        }

    }

    /* Se for negação de expressão */
    else if(tk.categoria == SN && tk.cod == NEGACAO){
        printf("\eh negacao");
        analex();
        fator();
    }
    else{
        erroSintatico("Era esperador fator");
    }

    return 0;

}

/*OK*/
int expr(){

    int tipo1, tipo2;

    tipo1 = expr_simp();
    printf("\nCODIGO T1 EM EXPR: %d\n",tipo1);

    if(tknext.categoria == SN){
        //se o proximo token for op relacional
        if(tknext.cod == COMPARA || tknext.cod == DIFERENTE || tknext.cod == MAIOR_Q || tknext.cod == MENOR_Q || tknext.cod == MAIOR_IG || tknext.cod == MENOR_IG){
            analex(); //vai pro op_rel
            opr_rel(); //confirma op_rel

            analex();
            tipo2 = expr_simp();
            printf("\nCODIGO T2 EM EXPR: %d\n",tipo2);
            if(tipo1 != tipo2)
            {
                erroSemantico("Tipos de variaveis diferentes!");
            }
        }//fim-se o proximo token for op relacional
        else{
            //Erro, esperando operador relacional
            //erroSintatico("Falta operador relacional");
        }
    }

    return tipo1;

}

/*OK*/
int expr_simp(){

    int tipo1, tipo2;

    /*Se o termo começar com + ou - */
    if(tk.categoria == SN && (tk.cod == SOMA || tk.cod == SUB)){
        analex();
    }

    tipo1 = termo();
    printf("\n****CODIGO T1 EM Exp_SIMPLES: %d\n",tipo1);

    if(tknext.categoria == SN && (tknext.cod == SOMA || tknext.cod == SUB || tknext.cod == OR)){
        analex(); //vai pro sinal
        while(1){

            analex();
            tipo2 = termo();
            printf("\n****CODIGO T2 EM EXPR_SIMPLES: %d\n",tipo2);
            if((tipo1==INTEIRO && (tipo2==BOOLEANO || tipo2==CARACTER)) || (tipo1==BOOLEANO && tipo2==INTEIRO))// SE FOR INTEIRO  E O OUTRO FOR UM BOOLEANO OU CARACTER
                {


                }else if(tipo1==CARACTER && tipo2==INTEIRO)//SE FOR CARACTER E O OUTRO UM INTEIRO
                {


                }else if(tipo1 == tipo2)
                {


                }else{
                    erroSemantico("Tipos de variavéis diferentes expr_simples");
                }

            if(!(tknext.categoria == SN && (tknext.cod == SOMA || tknext.cod == SUB || tknext.cod == OR))){
                break;
            }
            analex();
        }//fim-while

    }

        return tipo1;
}


/*OK*/
int tipo(){

    //Se for palavra reservada
    if(tk.categoria == PR){

        if(tk.cod == CARACTER){
            return tk.cod;
        }else if(tk.cod == INTEIRO){
            return tk.cod;
        }else if(tk.cod == REAL){
            return tk.cod;
        }else if(tk.cod == BOOLEANO){
            return tk.cod;
        }else{
            return -1;
        }

    }//FIM-se for PR
    return -1;
}

/*OK*/
void tipos_param(){

    int guardarTipo;

    //Se for palavra reservada
    if(tk.categoria == PR && tk.cod == SEMPARAM){
        //Se a palavra reservada for semparam
        return;
    }
    else{
        //SE FOR TIPO
        guardarTipo = tipo();
        if(guardarTipo>=0){
        analex();

            //se for id
            if(tk.categoria == ID){

                //Se nÃ£o houver o ID na tabela, ele insere
                if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, LOCAL, 0, 0, N_PROTO)){
                    controlador_TabSimb(EMPILHAR, tk.lexema, guardarTipo, LOCAL, PARAM, SIM_ZUMBI, N_PROTO);

                    //E Se esse proximo token for VIRG
                    if(tknext.categoria == SN && tknext.cod == VIRG){
                        analex();
                        while(1){
                            analex();
                            //Se for tipo
                            guardarTipo = tipo();
                            if(guardarTipo>=0){
                                analex();
                                //Se for ID
                                if(tk.categoria == ID){
                                    if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, LOCAL, 0, 0, N_PROTO)){
                                        controlador_TabSimb(EMPILHAR, tk.lexema, guardarTipo, LOCAL, PARAM, SIM_ZUMBI, N_PROTO);
                                    }else{
                                        erroSintatico("ID já existente");
                                    }
                                }//Fim-Se for ID
                                else{
                                    erroSintatico("ID inválido");
                                }

                            }//Fim-Se for tipo
                            else{
                                erroSintatico("Tipo inválido");
                            }


                            if(!(tknext.categoria == SN && tknext.cod == VIRG)){
                                break;
                            }
                            analex();

                        }//fim-while

                    }//fim-E Se esse proximo token for VIRG
                    else if(tknext.categoria == ID || tknext.categoria == PR){
                        erroSintatico("É esperado virgula");
                    }

                }else{
                    //Erro id jÃ¡ existente na tabela
                    erroSintatico("ID já existente na tabela");
                }

            }//fim-se for id
            else{
                //Esperado id
                erroSintatico("Era esperado ID");
            }

        }//fim-tipo
        else{
            //erro sintÃ¡tico TIPO INVALIDO
            erroSintatico("Tipo invalido");
        }

    }//FIM-ELSE PR

}

/*OK*/
void tipos_p_opc(){
    int guardarTipo;

    //Se for palavra reservada
    if(tk.categoria == PR && tk.cod == SEMPARAM){
        //Se a palavra reservada for semparam
        return;
    }
    else{
        //SE FOR TIPO
        guardarTipo = tipo();
        if(guardarTipo>=0){

            //Se o próximo token for ID
            if(tknext.categoria == ID){
                analex();//to no id
                if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, LOCAL, 0, 0, N_PROTO)){
                    controlador_TabSimb(EMPILHAR, tk.lexema, guardarTipo, LOCAL, PARAM, SIM_ZUMBI, N_PROTO);
                }
                else{
                    erroSintatico("ID já existente");
                }
            }//fim-se for id

            //Se o próximo token for virgula
            while(tknext.categoria == SN && tknext.cod == VIRG){
                analex();//ta na virgula
                analex();//ta no tipo
                guardarTipo = tipo();
                if(guardarTipo>=0){

                    //Se o próximo token for ID
                    if(tknext.categoria == ID){
                        analex();//to no id
                	if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, LOCAL, 0, 0, N_PROTO)){
                    		controlador_TabSimb(EMPILHAR, tk.lexema, guardarTipo, LOCAL, PARAM, SIM_ZUMBI, N_PROTO);
                        }
                        else{
                            erroSintatico("ID já existente");
                        }
                    }//fim-se for id

                }//fim-se for tipo2
                else{
                    erroSintatico("Tipo invalido!");
                }
            }//fim-while



        }//fim-tipo
        else{
            //erro sintÃ¡tico TIPO INVALIDO
            erroSintatico("Tipo invalido");
        }

    }//FIM-ELSE PR

}

/*OK*/
void atrib(){
    int tipo1, tipo2;
    //Se  for ID
    if(tk.categoria == ID){
        analex();

        tipo1 = PesquisarTipo(tk);
        printf("\nCODIGO DO ID EM ATRIB: %d\n",tipo1);
        //Se for um sinal de atribuição
        if(tk.categoria == SN && tk.cod == ATRIB){

            analex();
            tipo2 = expr();
            printf("\n***CODIGO DO ATRIB: %d\n",tipo2);
            if((tipo1==INTEIRO && (tipo2==BOOLEANO || tipo2==CARACTER)) || (tipo1==BOOLEANO && tipo2==INTEIRO))// SE FOR INTEIRO  E O OUTRO FOR UM BOOLEANO OU CARACTER
                {


                }else if(tipo1==CARACTER && tipo2==INTEIRO)//SE FOR CARACTER E O OUTRO UM INTEIRO
                {


                }else if(tipo1 == tipo2)
                {


                }else{
                    erroSemantico("Tipos de variavéis diferentes!\n");
                }

        }//fim-Se for um sinal de atribuição
        else{
            erroSintatico("É esperado um sinal de atribuição");
        }

    }//fim- se for id
    else{
        erroSintatico("É esperado um ID");
    }

}


/*OK*/
void cmd(){
    if(tk.categoria == PR || tk.categoria == ID || tk.categoria == SN){

        //Se for sinal
        if(tk.categoria == SN){

            switch(tk.cod){

                case PT_VIRG:
                    break;

                case CHAVES_ABRE:
                    while(1){
                        if((tknext.categoria == SN && tknext.cod==CHAVES_FECHA)){
                            analex();
                            break;
                        }
                        analex();
                        cmd();
                    }
                    break;

                default:
                    /*ERRO DE CATEGORIA*/
                    erroSintatico("Sinal inválido! É esperado { ou ;");

            }//fim-switch

        }//if SN

        //Se for palavra reservada
        else if(tk.categoria == PR){

            switch(tk.cod){

                case SE:
                    analex();
                    if(tk.categoria == SN && tk.cod == PARENTESIS_ABRE){
                        analex(); /*OBSERVAR AQUI*/
                        expr();
                        analex();
                        if(tk.categoria == SN && tk.cod == PARENTESIS_FECHA){
                            analex();
                            cmd();

                            //Saiu de CMD
                            //Se o próximo token for SENAO
                            if(tknext.categoria == PR && tknext.cod == SENAO){
                                analex(); //vai estar em senao
                                analex();
                                cmd();
                            }

                        }//if PARENTESIS_FECHA
                        else{
                            //Erro fecha parentesis esperado
                            erroSintatico("Esperado )");
                        }

                    }//if PARENTESIS_ABRE
                    else {
                        /*ERRO DE NÂO ABERTURA DE PARENTÊSE*/
                        erroSintatico("Esperado (");
                    }
                    break;
                case ENQUANTO:
                    analex();
                    if(tk.categoria == SN && tk.cod == PARENTESIS_ABRE){
                        analex();
                        expr();
                        analex();
                        if(tk.categoria == SN && tk.cod == PARENTESIS_FECHA){
                            analex();
                            cmd();
                        }else{

                            /*ESPERADO FECHA PARENTESE*/
                            erroSintatico("Esperado )");
                        }
                    }else {

                        /*ESPERADO ABRE PARENTESE*/
                        erroSintatico("Esperado (");
                    }
                    break;

                case PARA:
                    analex();
                    //Se for parentesis
                    if(tk.categoria == SN && tk.cod == PARENTESIS_ABRE){

                        //Se o proximo token for ID, então ocorrerá a atribuição 1
                        if(tknext.categoria == ID){
                            analex();
                            atrib();
                        }//-Fim possivel atrib 1

                        //Verifica se o próximo token é PT_VIRG 1
                        if(tknext.categoria == SN && tknext.cod == PT_VIRG){
                            analex();//to no pt_virg 1

                            //Verifica se próximo token é PT_VIRG 2
                            //Se for, não ocorrerá expr
                            //Se não for, pode ocorrer expr
                            if(tknext.categoria == SN && tknext.cod == PT_VIRG){
                                analex(); //to no pt_virg 2

                                //Se o proximo token for ID, então ocorrerá a atribuição 2
                                if(tknext.categoria == ID){
                                    analex();
                                    atrib();

                                    if(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA){
                                        analex();//ta no parentesis
                                        analex();//prepatativo pro cmd
                                        cmd();
                                    }else{
                                        erroSintatico("É esperado FECHA PARENTESIS1");
                                    }



                                }//-Fim possivel atrib 2
                                else if(!(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)){
                                    //sob observação
                                    erroSintatico("É esperado FECHA PARENTESIS3");
                                }


                            }//FIM-SE FOR PT_VIRG 2
                            //Caso não for PTVIRG, pode ocorrer expr
                            else{
                                analex();
                                expr();

                                //É esperado PT_VIRG 2 APOS EXPR
                                if(tknext.categoria == SN && tknext.cod == PT_VIRG){
                                    analex(); //to no pt_virg 2

                                    //Se o proximo token for ID, então ocorrerá a atribuição 2
                                    if(tknext.categoria == ID){
                                        analex();
                                        atrib();

                                        if(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA){
                                            analex();//ta no parentesis
                                            analex();//prepatativo pro cmd
                                            cmd();
                                        }else{
                                            erroSintatico("É esperado FECHA PARENTESIS2");
                                        }
                                    }else if(!(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)){
                                        //sob observação
                                        erroSintatico("É esperado FECHA PARENTESIS4");
                                    }


                                }//fIM-ESPERADO PT_VIRG2 APOS EXPR
                                else{
                                    erroSintatico("É esperado PT_VIRG");
                                }

                            }//fim-pode ocorrer expr

                        }//FIM-SE FOR PT_VIRG 1
                        else{
                            erroSintatico("É esperado PT_VIRG");
                        }


                    }//Fim- se for parentesis
                    else{
                        erroSintatico("É esperado PARENTESIS_ABRE");
                    }

                    break;

                case RETORNE:
                    if(tknext.categoria == SN && tknext.cod == PT_VIRG){
                        analex();
                        break;
                    }

                    //Se não entrou no if do pt e virg
                    analex();
                    expr();
                    analex();
                    if(tk.categoria == SN && tk.cod == PT_VIRG){
                        break;
                    }else{
                        erroSintatico("Falta PT_VIRG ;");
                    }
                    break;

                default:
                    /*ERRO de comando não esperado*/
                    erroSintatico("Cmd invalido");


            }//switch


        }//if PR

        //SE FOR ID
        else if(tk.categoria == ID){

            //olha o proximo token, se for atribuição
            //temos uma atribuição
            if(tknext.categoria == SN && tknext.cod == ATRIB){
                atrib(); //não chama analex pq ele já vai tá no id
                analex();
                if(tk.categoria == SN && tk.cod == PT_VIRG){
                    return;
                }else{
                    erroSintatico("Falta PT_VIRG ; após atribuição");
                }

            }//fim se for atrib


            //temos um id([expr{,expr}]);
            else if(tknext.categoria == SN && tknext.cod == PARENTESIS_ABRE){
                //Ta no id

                analex(); //passa pro próximo token

                //Se o próximo token for fecha parentesis
                if(tknext.categoria == SN && tknext.cod ==  PARENTESIS_FECHA){
                    analex();
                    //O proximo token tem que ser ponto e virgula

                    if(tknext.categoria == SN && tknext.cod == PT_VIRG){
                        analex();
                        return;
                    }else{
                        //Se não for, dá erro
                        erroSintatico("Falta PT_VIRG");
                    }
                }


                //Chama a função de expressÃ£o
                analex();
                expr();

                //Se o proximo token for uma virgula
                if(tknext.categoria == SN && tknext.cod == VIRG){
                    analex();
                    while(1){
                        analex();
                        expr();
                        if(!(tknext.categoria == SN && tknext.cod == VIRG)){
                            break;
                        }
                        analex();

                    }//fim-while
                }//fimSe o proximo token for uma virgula

                analex();
                //If para checar se houve fechar parentesis
                if((tk.categoria == SN && tk.cod == PARENTESIS_FECHA)){
                    //Agora vai ver se o proximo token é pt_virg
                    if(tknext.categoria == SN && tknext.cod == PT_VIRG){
                        analex();
                        return;
                    }//fim-se é pt_virg
                    else{
                        //não encontrou pt_virg
                        erroSintatico("Falta PT_VIRG");
                    }
                }//If encontrou fecha parentesis
                else{
                    //Erro faltando parentesis
                    erroSintatico("Falta fecha parentesis");
                }

                return;

            }////temos um id([expr{,expr}]);
            else{
                erroSintatico("Esperado cmd");
            }

        }//fim- if id
        else{
            erroSintatico("Cmd inválido");
        }


    }//If PR ou ID ou SN
    else{
        erroSintatico("Cmd inválido");
    }
}//void
/*Obs: se der erro em cmd, provavelmente seja por conta dos dois ultimos elses ;*/

void prog(){
    int guardarTipo;
    //analex();
    if(tk.categoria == PR && tipo() < 0)// if 1
    {
        printf("PALAVRA RESERVADA \n");
       if(tk.cod == PROTOTIPO)// if prototipo
       {
            printf("PROTOTIPO \n");
            if(tknext.categoria == PR && tknext.cod == SEMRETORNO)// if 2
            {
                    printf("SEMRETORNO \n");
                    analex();
                    if(tknext.categoria == ID)// if 3
                    {
                        printf("ID \n");
                        analex();
                       if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, GLOBAL, 0, 0, PROTO))// if 10
                        {
                            controlador_TabSimb(EMPILHAR, tk.lexema, tk.cod, GLOBAL, PARAM, SIM_ZUMBI, PROTO);

                            if(tknext.categoria == SN && tknext.cod == PARENTESIS_ABRE)// if 4
                            {
                                printf("PARENTESI ABRE \n");
                                analex();// vai para o parentesi
                                analex();// vai para o tipos_p_opc
                                printf("TIPOS_O_OPC \n");
                                tipos_p_opc();
                                if(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)// if 5
                                {
                                    printf("PARENTESI FECHA\n");
                                    analex();
                                        while(tknext.categoria == SN && tknext.cod == VIRG)// while VIRG
                                        {
                                                printf("VIRG \n");
                                                analex();// vai para VIRG
                                                if(tknext.categoria == ID)// if 40
                                                {
                                                    printf("ID");
                                                            analex();
                                                             if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, LOCAL, 0, 0, PROTO))// if 11
                                                            {
                                                                controlador_TabSimb(EMPILHAR, tk.lexema, tk.cod, LOCAL, PARAM, SIM_ZUMBI, PROTO);
                                                                 /*if(tknext.categoria == ID)// if 6
                                                                {

                                                                    analex();*/
                                                                    if(tknext.categoria == SN && tknext.cod == PARENTESIS_ABRE)// if 7
                                                                    {
                                                                        printf("PARENTESIS_ABRE\n");
                                                                        analex();// vai para o abre parentese
                                                                        analex();// vai para o tipos_p_opc
                                                                        printf("tipos_p_opc\n");
                                                                        tipos_p_opc();
                                                                        if(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)// if 8
                                                                        {
                                                                            printf("PARENTESIS_FECHA\n");
                                                                            analex();
                                                                        }else{
                                                                            erroSintatico("Esperava-se um fecha parentesi");
                                                                        }// fecha else do if 8
                                                                    }else{
                                                                            erroSintatico("Esperava-se um abre parentesi");
                                                                        }//fecha else do if 7

                                                            /*}// fecha if 6*/
                                                        }else{
                                                            erroSintatico("Dupla declaracao");
                                                        }// fim do else do if 11

                                                }// fim do if 40

                                    }// while VIRG

                                    //analex();
                                    if(tknext.categoria == SN && tknext.cod == PT_VIRG)// if 9
                                    {
                                        printf("PT_VIRG \n");
                                        analex();
                                    }
                                    else{
                                        erroSintatico("Esperava-se um ponto virgula");
                                    }// fecha else do if 9

                                }else
                                {
                                    erroSintatico("Esperava-se um fecha parentesi");
                                }// fecha else if 5
                            }else
                            {
                                erroSintatico("Esperava-se um abre parentesi");
                            }// fecha do if 4

                        } else // do if 10
                        {
                            erroSintatico("Dupla declaracao");
                        }// fecha else do if 10

                    }// fecha do if 3

                } else // else do if 2
                {
                    analex();
                    printf("Tipo\n");
                    tipo();
                    if(tknext.categoria == ID)// if 3
                    {
                        analex();
                         printf("ID\n");
                        if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, GLOBAL, 0, 0, PROTO))// if 10
                        {
                            controlador_TabSimb(EMPILHAR, tk.lexema, tk.cod, GLOBAL, PARAM, SIM_ZUMBI, PROTO);

                            if(tknext.categoria == SN && tknext.cod == PARENTESIS_ABRE)// if 4
                            {
                                 printf("PARENTESIS_ABRE\n");
                                analex();// vai para o abre parentesi
                                analex();// vai para o tipos_p_opc
                                printf("TIPOS_P_OPC\n");
                                tipos_p_opc();
                                if(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)// if 5
                                {
                                    analex();
                                    printf("PARENTESIS_FECHA\n");
                                    while(tknext.categoria == SN && tknext.cod == VIRG)
                                    {
                                        printf("VIRG \n");
                                        analex();
                                        if(tknext.categoria == ID)// if 45.
                                        {
                                            printf("ID\n");
                                            analex();
                                            if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, GLOBAL, 0, 0, PROTO))// if 11
                                                {
                                                    controlador_TabSimb(EMPILHAR, tk.lexema, tk.cod, GLOBAL, PARAM, SIM_ZUMBI, PROTO);
                                                    /* if(tknext.categoria == ID)// if 6
                                                    {

                                                        analex();*/
                                                        if(tknext.categoria == SN && tknext.cod == PARENTESIS_ABRE)// if 7
                                                        {
                                                            printf("PARENTESIS_ABRE \n");
                                                            analex();// vai para o abre parentese
                                                            analex();// vai para o tipos_p_opc
                                                            tipos_p_opc();
                                                            if(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)// if 8
                                                            {
                                                                printf("PARENTESIS_FECHA \n");
                                                                analex();
                                                            }else{
                                                                erroSintatico("Esperava-se um fecha parentesi");
                                                            }// fecha else do if 8
                                                        }else{
                                                                erroSintatico("Esperava-se um abre parentesi");
                                                            }//fecha else do if 7

                                               /* }// fecha if 6*/
                                            }else{
                                                erroSintatico("Dupla declaracao");
                                            }// fim do else do if 11

                                        }// fecha if 45.


                                }// while parentese

                                    //analex();
                                    if(tknext.categoria == SN && tknext.cod == PT_VIRG)// if 9
                                    {
                                        analex();
                                    }
                                    else{
                                        erroSintatico("Esperava-se um ponto virgula");
                                    }// fecha else do if 9
                                }else
                                {
                                    erroSintatico("Esperava-se um fecha parentesi");
                                }// fecha else if 5
                            }else
                            {
                                erroSintatico("Esperava-se um abre parentesi");
                            }// fecha do if 4

                        } else // do if 10
                        {
                            erroSintatico("Dupla declaracao");
                        }// fecha else do if 10

                    }// fecha do if 3
                }// fecha else do if 2

       }else if(tk.cod == SEMRETORNO)// fecha if prototipo
       {

                printf("SEM RETORNO \n");
                if(tknext.categoria == ID )// if 20
                {
                    printf("ID \n");
                    analex();
                    if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, GLOBAL, 0, 0, N_PROTO))// if 21
                      {
                           controlador_TabSimb(EMPILHAR, tk.lexema, tk.cod, GLOBAL, PARAM, SIM_ZUMBI, N_PROTO);
                           if(tknext.categoria == SN && tknext.cod == PARENTESIS_ABRE)// if 23
                            {
                                printf("ABRE PARENTESI \n");
                                analex();// vai para o parentesi
                                printf("TIPOS_PARM \n");
                                analex();//vai para o tipos_param
                                tipos_param();
                              if(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)// if 24
                               {


                                    printf("PAARENTESI FECHA \n");
                                    analex();
                                    //Espera pelo abre chaves
                                            if(tknext.categoria == SN && tknext.cod == CHAVES_ABRE){
                                                analex(); //ta no abre chaves

                                                //Se depois vier um fecha chaves, retorna
                                                if(tknext.categoria == SN && tknext.cod == CHAVES_FECHA){
                                                    analex();
                                                     printf("\n{}");
                                                    return;
                                                }
                                                //Enquanto tnext for tipo
                                                while(tknext.categoria == PR && (tknext.cod == CARACTER || tknext.cod == INTEIRO || tknext.cod == REAL ||tknext.cod == BOOLEANO)){
                                                    printf("\nEntrei no while");
                                                    analex();//ta no tipo
                                                    guardarTipo = tipo();
                                                    printf("\nTo no tipo");
                                                    //Se o próximo token for ID
                                                    if(tknext.categoria == ID){
                                                        analex();//tá no id

                                                        //Insere ID na tabela
                                                        if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, LOCAL, 0, 0, N_PROTO)){
                                                            controlador_TabSimb(EMPILHAR, tk.lexema, guardarTipo, LOCAL, VAR, NAO_ZUMBI, N_PROTO);
                                                        }else{
                                                            erroSintatico("ID já existente");
                                                        }

                                                        printf("\nTo no id");

                                                        //Enquanto TNEXT for vírgula
                                                        while(tknext.categoria == SN && tknext.cod == VIRG){
                                                            printf("\nEntrei no while2");
                                                            analex();//Pra pegar a virgula
                                                            if(tknext.categoria != ID){
                                                                erroSintatico("Eh esperado ID após virgula");
                                                            }
                                                            analex();//pra pegar o ID

                                                            //Insere ID na tabela
                                                            if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, LOCAL, 0, 0, N_PROTO)){
                                                                controlador_TabSimb(EMPILHAR, tk.lexema, guardarTipo, LOCAL, VAR, NAO_ZUMBI, N_PROTO);
                                                            }else{
                                                                erroSintatico("ID já existente");
                                                            }

                                                            if(!(tknext.categoria == SN && tknext.cod == VIRG) && !(tknext.categoria == SN && tknext.cod == PT_VIRG)){
                                                                erroSintatico("Eh esperado virgula ou PT_VIRG apos id");
                                                            }
                                                        }

                                                        if(!(tknext.categoria == SN && tknext.cod == PT_VIRG)){
                                                            erroAnalisadorLexico("É esperado PT_VIRG");
                                                        }
                                                        analex(); //pergou pt e virgula
                                                        printf("\nPeguei o ponto e virgula");

                                                    }//Fim-se tknext for id
                                                    else{
                                                        erroAnalisadorLexico("Após tipo é esperado ID");
                                                    }


                                                }//fim-Enquanto tnext for tipo

                                                //chamar CMD
                                                printf("\nTalvez ocorra cmd");
                                                while(!(tknext.categoria == SN && tknext.cod == CHAVES_FECHA)){
                                                    analex();
                                                    printf("\nvai ocorrer cmd");
                                                    cmd();
                                                }

                                                //Apos encontrar o fecha chaves, desempilha a tabela de sinais
                                                desempilhar();

                                            }

                                   // }






                            }else// else do if 24
                                {
                                    erroSintatico("Esperava-se um fecha parentesi");
                                }// fecha else do if 24
                            }else// else do if 23
                                {
                                    erroSintatico("Esperava-se um abre parentesi");
                                }//fim do else do if 23
                      }//fim do if 21
                }//fecha if 20

       }// fecha else if prototipo


    }else if(tipo() >= 0)//else if do if 1
    {
            printf("TIPO \n");
            if(tknext.categoria == ID )// if 72
            {
                 printf("ID \n");
                 analex();
                if(controlador_TabSimb(CONSULTAR, tk.lexema, 0, GLOBAL, 0, 0, N_PROTO))// if 71
                 {
                    controlador_TabSimb(EMPILHAR, tk.lexema, tk.cod, GLOBAL, PARAM, SIM_ZUMBI, N_PROTO);
                    if(tknext.categoria == SN && tknext.cod == PARENTESIS_ABRE)// if 73
                    {
                        analex();
                        analex();
                        tipos_param();
                        if(tknext.categoria == SN && tknext.cod == PARENTESIS_FECHA)// if 77
                        {
                            analex();
                            //Espera pelo abre chaves
                            if(tknext.categoria == SN && tknext.cod == CHAVES_ABRE){
                                analex(); //ta no abre chaves

                                //Se depois vier um fecha chaves, retorna
                                if(tknext.categoria == SN && tknext.cod == CHAVES_FECHA){
                                    analex();
                                     printf("\n{}");
                                    return;
                                }
                                //Enquanto tnext for tipo
                                while(tknext.categoria == PR && (tknext.cod == CARACTER || tknext.cod == INTEIRO || tknext.cod == REAL ||tknext.cod == BOOLEANO)){
                                    printf("\nEntrei no while");
                                    analex();//ta no tipo
                                    guardarTipo = tipo();
                                    printf("\nTo no tipo");
                                    //Se o próximo token for ID
                                    if(tknext.categoria == ID){
                                        analex();//tá no id

                                        //Insere ID na tabela
                                        if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, LOCAL, 0, 0, N_PROTO)){
                                            controlador_TabSimb(EMPILHAR, tk.lexema, guardarTipo, LOCAL, VAR, NAO_ZUMBI,N_PROTO);
                                        }else{
                                            erroSintatico("ID já existente");
                                        }

                                        printf("\nTo no id");

                                        //Enquanto TNEXT for vírgula
                                        while(tknext.categoria == SN && tknext.cod == VIRG){
                                            printf("\nEntrei no while2");
                                            analex();//Pra pegar a virgula
                                            if(tknext.categoria != ID){
                                                erroSintatico("Eh esperado ID após virgula");
                                            }
                                            analex();//pra pegar o ID

                                            //Insere ID na tabela
                                            if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, LOCAL, 0, 0, N_PROTO)){
                                                controlador_TabSimb(EMPILHAR, tk.lexema, guardarTipo, LOCAL, VAR, NAO_ZUMBI, N_PROTO);
                                            }else{
                                                erroSintatico("ID já existente");
                                            }

                                            if(!(tknext.categoria == SN && tknext.cod == VIRG) && !(tknext.categoria == SN && tknext.cod == PT_VIRG)){
                                                erroSintatico("Eh esperado virgula ou PT_VIRG apos id");
                                            }
                                        }

                                        if(!(tknext.categoria == SN && tknext.cod == PT_VIRG)){
                                            erroAnalisadorLexico("É esperado PT_VIRG");
                                        }
                                        analex(); //pergou pt e virgula
                                        printf("\nPeguei o ponto e virgula");

                                    }//Fim-se tknext for id
                                    else{
                                        erroAnalisadorLexico("Após tipo é esperado ID");
                                    }


                                }//fim-Enquanto tnext for tipo

                                //chamar CMD
                                printf("\nTalvez ocorra cmd");
                                while(!(tknext.categoria == SN && tknext.cod == CHAVES_FECHA)){
                                    analex();
                                    printf("\nvai ocorrer cmd");
                                    cmd();
                                }

                                //Apos encontrar o fecha chaves, desempilha a tabela de sinais
                                desempilhar();

                            }


                        }// fecha if 77

                    }else
                    {
                       // printf("Vim no while");
                        while(tknext.categoria == SN && tknext.cod == VIRG)// while 6
                        {
                            printf("VIRG c \n");
                            analex();
                            if(tknext.categoria == ID)
                            {
                                printf("ID \n");
                                analex();
                                 if(!controlador_TabSimb(CONSULTAR, tk.lexema, 0, GLOBAL, 0, 0, N_PROTO))// if 71
                                 {
                                    controlador_TabSimb(EMPILHAR, tk.lexema, tk.cod, GLOBAL, PARAM, SIM_ZUMBI, N_PROTO);
                                }else// if 71
                                {
                                    erroSintatico("Dupla decraração");
                                }// if 71
                            }

                        }// while 6


                         if(tknext.categoria == SN && tknext.cod == PT_VIRG)// if 74
                           {
                               printf("PT_VIRG \n");
                                  analex();

                           }else // if 74
                           {
                                erroSintatico("Esperava-se um ponto virgula");
                           }// fecha else do if 74



                    }// if 73

                 }else// if 71
                {
                    erroSintatico("Dupla decraração");
                }// if 71
            }// if 72

    }else// else do else if 1
    {
        printf("VAZIO \n");
        //return;
    }// fecha else do else if 1

}// fecha prog


void anasin(){
    iniciarTabelaDeSimbolos();

    analex();
    analex();

    //enquanto não for fim de arquivo
    while(!feof(arquivo)){
        prog();
        analex();
        printf("\nLinha: %d",linhas);
    }//fim-enquanto não for fim de arquivo
}

int main(){
    char nomeArquivo[1000];

    printf("\n\tDigite o nome do arquivo fonte: "); scanf("%s", nomeArquivo);

    /*ABRE O ARQUIVO*/
	if ( (arquivo = fopen(nomeArquivo,"r")) != NULL ){
        printf("\n\tArquivo aberto com sucesso!\n");
        iniciarTabelaDeSimbolos();

        anasin();

        fclose(arquivo);
    }
    else{
        printf("\n\tNão foi possível abrir o arquivo %s\n", nomeArquivo);
        system("pause");
        exit(1);
    }
    /*FIM-ABRE O ARQUIVO*/

    printf("\n");
    system("pause");
    return 0;
}

