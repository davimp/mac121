/*
  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__

  AO PREENCHER ESSE CABEÇALHO COM O MEU NOME E O MEU NÚMERO USP, 
  DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESSE PROGRAMA. 
  TODAS AS PARTES ORIGINAIS DESSE EXERCÍCIO PROGRAMA (EP) FORAM 
  DESENVOLVIDAS E IMPLEMENTADAS POR MIM SEGUINDO AS INSTRUÇÕES DESSE EP
  E QUE PORTANTO NÃO CONSTITUEM PLÁGIO. DECLARO TAMBÉM QUE SOU RESPONSÁVEL
  POR TODAS AS CÓPIAS DESSE PROGRAMA E QUE EU NÃO DISTRIBUI OU FACILITEI A
  SUA DISTRIBUIÇÃO. ESTOU CIENTE QUE OS CASOS DE PLÁGIO SÃO PUNIDOS COM 
  REPROVAÇÃO DIRETA NA DISCIPLINA.

  Nome: Davi de Menezes Pereira

  eval.c
  Pitao II
s
  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/*
  NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ESQUELETO
*/

/*------------------------------------------------------------*/
/* interface para as funcoes deste modulo */
#include "eval.h" 

/*------------------------------------------------------------*/
#include <stdlib.h>  /* atoi(), atof(), strtol() */
#include <string.h>  /* strncmp(), strlen(), strncpy(), strcpy(), strcmp() */
#include <math.h>    /* pow() */

/*------------------------------------------------------------*/
#include "categorias.h" /* Categoria, MAX_OPERADORES */
#include "util.h"       /* ERRO(), AVISO(), mallocSafe() */
#include "objetos.h"    /* CelObjeto, mostreObjeto(), freeObjeto()
                           mostreListaObjetos(), 
                           freeListaObjetos() */
#include "stack.h"      /* funcoes que manipulam uma pilha */ 
                        /* essa pilha sera usada para implementar 
                           a pilha de execucao */
#include "st.h"         /* getValorST(), setValorST() */    
    

/*------------------------------------------------------------*/
/* Protipos de funcoes auxiliares */

/*------------------------------------------------------------*/
/* Tabela com uma representacao da precedencia dos operadores
 * atraves de valores inteiros. 
 * Quanto maior o valor, maior o valor, maior a precedencia.
 *
 *  http://www.ime.usp.br/~pf/algoritmos/apend/precedence.html
 */
static const int precedencia[MAX_OPERADORES] =
{
    /* 4 operadores relacionais com 2 simbolos  */
    4   /* "==" */ /* da esquerda para a direita */
    ,4   /* "!=" */ /* da esquerda para a direita */
    ,5   /* ">=" */ /* da esquerda para a direita */
    ,5   /* "<=" */ /* da esquerda para a direita */
         
    /* 2 operadores aritmeticos com 2 simbolos */
    ,8 /* ,"**" */ /* da direita para a esquerda */
    ,7 /* ,"//" */ /* da esquerda para a direita */
 
    /* 2 operadores relacionais com 1 simbolo */
    ,5  /* ">"  */ /* da esquerda para a direita */
    ,5  /* "<"  */ /* da esquerda para a direita */ 
    
    /* 6 operadores aritmeticos */
    ,7  /* "%" */ /* da esquerda para a direita */
    ,7  /* "*" */ /* da esquerda para a direita */
    ,7  /* "/" */ /* da esquerda para a direita */
    ,6  /* "+" */ /* da esquerda para a direita */
    ,6  /* "-" */ /* da esquerda para a direita */
    ,8  /* "_" */ /* da direita para a esquerda */
    
    /* 3 operadores logicos  */
    ,3  /* "and" */ /* da esquerda para a direita */ 
    ,2  /* "or" */ /* da esquerda para a direita */
    ,8  /* "not"  */ /* da direita para a esquerda */
    
    /* operador de indexacao */
    ,9  /* "$"  */ /* da esquerda para a direita (IGNORAR)*/

    /* atribuicao */ 
    ,1  /* "=" */ /* da direita para a esquerda EP4 */ 
}; 

/*-------------------------------------------------------------
 *  itensParaValores
 *  
 *  RECEBE uma lista ligada com cabeca INIFILAITENS representando uma
 *  fila de itens lexicos. Inicialmente, o campo VALOR de cada celula
 *  contem um string com o item lexico.
 *
 *  A funca CONVERTE o campo VALOR de cada celula para um inteiro 
 *  ou double, como descrito a seguir. 
 *
 *  IMPLEMENTACAO
 *
 *  O campo VALOR de uma CelObjeto tem 3 subcampos:
 *
 *      - valor.vInt   (int)
 *      - valor.vFloat (float)
 *      - valor.pStr   (string)
 *
 *  Nessa conversao, o campo VALOR de cada celula recebe um valor 
 *  da seguinte maneira.
 *
 *     - Se o campo CATEGORIA da celula indica que o item e um
 *       string representando um int (INT_STR) entao o campo 
 *       valor.vFloat deve receber esse numero inteiro. 
 *
 *       Nesse caso a CATEGORIA do item deve ser alterada para 
 *       FLOAT.
 *
 *     - se o campo CATEGORIA da celula indica que o item e um
 *       string representando um float (FLOAT_STR) entao o campo 
 *       valor.vFloat deve receber esse float;
 *
 *       Nesse caso a CATEGORIA do item deve ser alterada para 
 *       FLOAT.
 *
 *     - se o campo CATEGORIA da celula indica que o item e um
 *       string representando um Bool (BOOL_STR) entao o campo 
 *       valor.vFloat deve receber o inteiro correspondente 
 *       (False = 0, True = 1);
 *
 *       Nesse caso a CATEGORIA do item deve ser alterada para 
 *       FLOAT.
 *
 *     - se o campo CATEGORIA da celula indica que o item e um
 *       string representando um operador (OPER_*) entao o campo 
 *       valor.vInt deve receber o inteiro correspondente 
 *       a precedencia desse operador. 
 *       
 *       Para isto utilize o vetor PRECEDENCIA declarado antes 
 *       desta funcao. 
 *
 * Nesta funcao (e nas outras) voce pode utilizar qualquer funcao da
 * biblioteca string do C, como, por exemplo, atoi(), atof().
 *
 * Esta funcao (e todas as outras) devem 'dar' free nas estruturas que
 * deixarem de ser necessarias.
 *
 * Esse e o caso das dos strings dos itens das categorias 
 * FLOAT_STR e INT_STR.  
 *
 * Os campos strings de objetos OPER_* e BOOL_STR apontam para
 * strings em tabelas definidas no modulo lexer.h. Nesse
 * caso, tentar liberar essa memoria e' um erro.
 *
 */

void
itensParaValores(CelObjeto *iniFilaItens)
{
    CelObjeto *i;
    double aux;

    for(i = iniFilaItens->prox; i != NULL; i = i->prox){
        /*verifica se contem string, pois, nesse caso, devemos dar free*/
        if(i->categoria == FLOAT_STR || i->categoria == INT_STR || i->categoria == BOOL_STR){
            aux = atof(i->valor.pStr);
            free(i->valor.pStr);
            i->valor.vFloat = aux;
            i->categoria = FLOAT;
        }
        /*abaixo, verifica se e' uma operacao*/
        else if(i->categoria <= 18){
            i->valor.vInt = precedencia[i -> categoria];
        }
    }

}

/*-------------------------------------------------------------
 * eval
 * 
 * RECEBE uma lista ligada como cabeca INIPOSFIXA que representa
 * uma expressao em notacao posfixa. 
 *
 * RETORNA o endereco de uma celula com o valor da expressao.
 *
 * PRE-CONDICAO: a funcao supoe que a expressao esta sintaticamente
 *               correta.
 *
 * IMPLEMENTACAO
 *
 * Para o calculo do valor da expressao deve ser usada uma 
 * pilha. O endereco retornado sera o da celula no topo no
 * topo dessa  "pilha de execucao".
 * 
 * A funcao percorre a expressao calculando os valores resultantes.
 * Para isto e utilizada uma pilha de execucao. 
 * 
 * A implementacao das funcoes que manipulam a pilham devem ser
 * escritas no modulo stack.c.
 * 
 * O arquivo stack.h contem a interface para essas funcoes. 
 * A pilha de execucao so deve ser acessada atraves dessa interface
 * (em caso contrario nao chamariamos stack.h de interface).
 * 
 * O parametro mostrePilhaExecucao indica se os valores
 * na pilha de execucao devem ser exibidos depois de qualquer 
 * alteracao.
 * 
 * Esta funcao deve "dar free" nas estruturas que deixarem de ser 
 * necessarias.
 *
 * EP4: funcao deve ser adaptada para tratar do operador de atribuicao
 *      '=' e variaveis. A maneira de proceder esta descrita no 
 *      enunciado na secao "Interpretacao da expressao posfixa".
 */
CelObjeto *
eval (CelObjeto *iniPosfixa, Bool mostrePilhaExecucao)
{
    CelObjeto* i; /* ponteiro para percorrer a lista*/
    CelObjeto* atr;
    CelObjeto a, b; /* variaveis para pegar os operandos*/
    CelObjeto auxiliar;
    double resultado = 0.; /* varaivel para guardar o resultado de cada operacao*/
    int divisao; /* variavel para auxiliar na divisao inteira*/ 
    stackInit(); /* inicializacao da pilha */
    for(i = iniPosfixa->prox; i != NULL; i = i->prox){
        /* checa se e' uma operacao entre dois numeros */
        if(i->categoria < 18 && i->categoria != OPER_MENOS_UNARIO && i->categoria != OPER_LOGICO_NOT){
            if(!stackEmpty())
                b = stackPop();
            else{
                AVISO(eval.c);
                ERRO(eval em eval.c: abortando o calculo da expressao);
                stackFree();
                return NULL;
            }
            
            if(!stackEmpty())
                a = stackPop();
            else{
                AVISO(eval.c);
                ERRO(eval em eval.c: abortando o calculo da expressao);
                stackFree();
                return NULL;
            }
            switch (i->categoria)
            {
            case OPER_IGUAL:    
                if(a.valor.vFloat == b.valor.vFloat)
                    resultado = TRUE;
                else 
                    resultado = FALSE;
                break;
            case OPER_DIFERENTE:
                if(a.valor.vFloat != b.valor.vFloat)
                    resultado = TRUE;
                else
                    resultado = FALSE;
                break;
            case OPER_MAIOR_IGUAL:
                if(a.valor.vFloat >= b.valor.vFloat)
                    resultado = TRUE;
                else
                    resultado = FALSE;
                break;
            case OPER_MENOR_IGUAL:
                if(a.valor.vFloat <= b.valor.vFloat)
                    resultado = TRUE;
                else
                    resultado = FALSE;
                break;
            case OPER_EXPONENCIACAO:
                resultado = pow(a.valor.vFloat, b.valor.vFloat);
                break;
            case OPER_DIVISAO_INT:
                divisao = a.valor.vFloat/b.valor.vFloat;
                resultado = divisao;
                break;
            case OPER_MAIOR:
                if(a.valor.vFloat > b.valor.vFloat)
                    resultado = TRUE;
                else
                    resultado = FALSE;
                break;
            case OPER_MENOR:
                if(a.valor.vFloat < b.valor.vFloat)
                    resultado = TRUE;
                else
                    resultado = FALSE;
                break;
            case OPER_RESTO_DIVISAO:
                resultado = fmod(a.valor.vFloat, b.valor.vFloat);
                break;
            case OPER_MULTIPLICACAO:
                resultado = (a.valor.vFloat)*(b.valor.vFloat);
                break;
            case OPER_DIVISAO:
                resultado = (a.valor.vFloat)/(b.valor.vFloat);
                break;
            case OPER_ADICAO:
                resultado = (a.valor.vFloat)+(b.valor.vFloat);
                break;
            case OPER_SUBTRACAO:
                resultado = (a.valor.vFloat)-(b.valor.vFloat);
                break;
            case OPER_LOGICO_AND:
                if((a.valor.vFloat) && (b.valor.vFloat))
                    resultado = TRUE;
                else
                    resultado = FALSE;
                break;
            case OPER_LOGICO_OR:
                if(a.valor.vFloat || b.valor.vFloat)
                    resultado = TRUE;
                else
                    resultado = FALSE;
                break;
            default:
                break;
            }
            auxiliar.valor.vFloat = resultado;
            auxiliar.categoria = FLOAT;
            stackPush(auxiliar);
        }
        /* os proximos dois else if sao para as duas operacoes unarias */
        else if(i->categoria == OPER_LOGICO_NOT){
            if(!stackEmpty()){
                a = stackPop();
            }
            else{
                AVISO(stackPop em stack.c: pilha vazia...);
                ERRO(eval em eval.c: abortando o calculo da expressao);
                stackFree();
                return NULL;
            }
            auxiliar.categoria = FLOAT;
            auxiliar.valor.vFloat = !(a.valor.vFloat);
            stackPush(auxiliar);
        }
        else if(i->categoria == OPER_MENOS_UNARIO){
            if(!stackEmpty()){
                a = stackPop();
            }
            else{
                AVISO(stackPop em stack.c: pilha vazia...);
                ERRO(eval em eval.c: abortando o calculo da expressao);
                stackFree();
                return NULL;
            }
            auxiliar.categoria = FLOAT;
            auxiliar.valor.vFloat = -(a.valor.vFloat);
            stackPush(auxiliar);
        }
        else if(i->categoria == OPER_ATRIBUICAO){
            if(!stackEmpty())
                b = stackPop();
            else{
                AVISO(eval.c);
                ERRO(eval em eval.c: abortando o calculo da expressao);
                stackFree();
                return NULL;
            }
            
            if(!stackEmpty())
                a = stackPop();
            else{
                AVISO(eval.c);
                ERRO(eval em eval.c: abortando o calculo da expressao);
                stackFree();
                return NULL;
            }
            atr = mallocSafe(sizeof(CelObjeto *));
            atr->valor = b.valor;
            atr->categoria = b.categoria;
            setValorST(a.valor.pStr, atr);
            free(atr);

            auxiliar.categoria = FLOAT;
            auxiliar.valor.vFloat =  b.valor.vFloat;

            stackPush(auxiliar);
        }
        else if(i->categoria == ID){
            auxiliar = *getValorST(i->valor.pStr);
            stackPush(auxiliar);
        }
        /* se for um operando */
        else if(i->categoria == FLOAT){
            auxiliar.categoria = FLOAT;
            auxiliar.valor.vFloat = i->valor.vFloat;
            stackPush(auxiliar);
        }
        /* se for para mostra a pilha de execucao */
        if(mostrePilhaExecucao){
            stackPrint();
        }
    }
    /* resultado das operacoes ta no topo da pilha */
    if(stackEmpty()){
        stackFree();
        return NULL;
    }
    i = NULL;
    i = mallocSafe(sizeof(CelObjeto*));
    auxiliar = stackPop();
    i->categoria = auxiliar.categoria;
    i->valor.vFloat = auxiliar.valor.vFloat;
    i->prox = NULL;
    stackFree();
    return i;
}

