#include "controlador_tab.h"
#include "anasin.h"


/*Inicia o topo da pilha*/
void iniciarTabelaDeSimbolos(){
    topo = -1;
}

/*Função principal*/
int controlador_TabSimb(int operation, char lexema[], int tipo, int escopo, int categoria, int zumbi, int prototipo){

    switch (operation){
        case EMPILHAR:
            if(topo == TAM_TABSIMB){
                //Se a pilha estiver cheia
                printf("\nPilha cheia!");
                exit(1);
            }else{
                empilhar(lexema, tipo, escopo, categoria, zumbi, prototipo);
                return 1;
            }
            break;

        case DESEMPILHAR:
            desempilhar();
            return 1;
            break;

        case CONSULTAR:
            return(consultar(lexema, escopo, prototipo));
            break;

        case CONSULTARPARAM:
            return(consultarParam(lexema, escopo));
            break;
    }//fim-switch

    return 0;
}

void empilhar(char lexema[], int tipo, int escopo, int categoria, int zumbi, int prototipo){

    topo++;

    strcpy(tabela[topo].lexema, lexema);
    tabela[topo].i = topo;
    tabela[topo].tipo = tipo;
    tabela[topo].escopo = escopo;
    tabela[topo].categoria = categoria;
    tabela[topo].zumbi = zumbi;
    tabela[topo].prototipo = prototipo;
}

void desempilhar(){
    while(tabela[topo].zumbi == NAO_ZUMBI && tabela[topo].escopo == LOCAL){
        topo--;
    }
}

int consultar(char *lexema, int escopo, int prototipo){
    int i = topo;

    if(topo == -1){
        return FALSO;
    }

    for(i = topo; i!=-1; i--){

		if(strcmp(tabela[i].lexema, lexema)==0){
			if(tabela[i].escopo == escopo){
				return VERDADEIRO;
			}else{
				return FALSO;
			}
		}

    }

    return FALSO;
}

/*Vai consultar se os parametros de uma função se repete*/
int consultarParam(char *lexema, int escopo){
    int i = topo;

    if(topo == -1){
        return FALSO;
    }

    for(i = topo; i!=-1; i--){

        if(tabela[i].categoria == FUNC){
            break;
        }
		if(strcmp(tabela[i].lexema, lexema)==0){
			if(tabela[i].escopo == escopo){
				return VERDADEIRO;
			}else{
				return FALSO;
			}
		}
    }

    return FALSO;
}

/*Vai consultar o indice de uma determinada função na tabela de simbolos e retornar ele*/
/*Se não encontrar, retorna -1*/
int consultarFunc(char *lexema){
    int i = topo;

    if(topo == -1){
        return -1;
    }

    for(i = topo; i!=-1; i--){

        if(tabela[i].categoria == FUNC && strcmp(tabela[i].lexema, lexema)==0){
            return i;
        }
    }

    return -1;
}

/*vai consultar se determinada função tem prototipo*/
int consultarPrototipo(char *lexema){
    int i = topo;

    if(topo == -1){
        return FALSO;
    }

    for(i = topo; i!=-1; i--){

        if(tabela[i].categoria == FUNC && strcmp(tabela[i].lexema, lexema)==0){
            if(tabela[i].prototipo == PROTO){
                return VERDADEIRO;
            }else{
                return FALSO;
            }
        }
    }

    return FALSO;
}

int checarTipoParam(int indice, int tipo){

    if(tabela[indice].tipo == tipo){
        return VERDADEIRO;
    }else{
        return FALSO;
    }

}

void imprimirTabela(){
    int i = topo;

    if(topo == -1){
        return;
    }

    for(i = topo; i!=-1; i--){
        printf("\n----------- LINHA %d -----------", tabela[i].i);
        printf("\nLexema: %s",tabela[i].lexema);
        printf("\nI: %d",tabela[i].i);
        printf("\nTipo: %d",tabela[i].tipo);
        printf("\nEscopo: %d",tabela[i].escopo);
        printf("\nCategoria: %d",tabela[i].categoria);
        printf("\nZumbi: %d",tabela[i].zumbi);
        printf("\n----------- FIM-LINHA %d -----------", tabela[i].i);
    }
}

int PesquisarTipo(token tk)
{
    int i = topo;

    if(topo == -1){
        return FALSO;
    }

    for(i = topo; i!=-1; i--){

		if(strcmp(tabela[i].lexema, tk.lexema)==0){
			if(tabela[i].escopo == -6){
				return tabela[i].tipo;
			}else{
			    erroSemantico("Variavel não declarada!");
			}
		}

    }

    erroSemantico("Variavel nao declarada!");
}

int pegarFuncao()
{
    int i = topo;

    if(topo == -1){
        return FALSO;
    }

        for(i = topo; i!=-1; i--){
              if(tabela[i].categoria == FUNC)
                {
                    return tabela[i].tipo;
                    break;
                }
        }


}

