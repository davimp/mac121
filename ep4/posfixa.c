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

  posfixa.c
  Pitao II

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:

  - função mallocc retirada de: 

  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/*
  NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ESQUELETO
*/

/*------------------------------------------------------------*/
/* iterface para o uso da funcao deste módulo */
#include "posfixa.h"

/*------------------------------------------------------------*/
#include "categorias.h" /* Categoria, MAX_OPERADORES, INDEFINIDA, 
                           ABRE_PARENTESES, ... */
#include "objetos.h" /* tipo CelObjeto, freeObjeto(), freeListaObjetos() */
#include "stack.h"   /* stackInit(), stackFree(), stackPop() 
                        stackPush(), stackTop() */

/*-------------------------------------------------------------
 *  infixaParaPosfixa
 * 
 *  Recebe uma lista ligada com cabeca INIINFIXA representando uma
 *  fila de itens de uma expressao infixa e RETORNA uma lista ligada
 *  com cabeca contendo a fila com a representacao da correspondente
 *  expressao em notacao posfixa.
 */
 /*  As celulas da notacao infixa que nao forem utilizadas na notacao 
  *  posfixa (abre e fecha parenteses) devem ser liberadas 
  *  (freeObjeto()).
  */
CelObjeto *
infixaParaPosfixa(CelObjeto *iniInfixa)
{
    int valor;
    CelObjeto * p; /* vai guardar o inicio da fila com a notacao posfixa*/
    CelObjeto * i; /* percorre a lista com notacao infixa*/
    CelObjeto * aux;/* ajuda a inserir na fila com a notacao posfixa*/
    CelObjeto auxiliar; /*ajuda a manipular a pilha*/
    p = mallocSafe(sizeof(CelObjeto *));
    p->prox = NULL;
    aux = p;
    stackInit();
    for(i = iniInfixa->prox; i != NULL; i = i->prox){
      if(i->categoria <= 18){ /*se for operador*/
        if(stackEmpty()){
          stackPush((*i));
          /*se a pilha estiver vazia, empilha*/
        }
        else{
          /*se nao estiver vazia, compara com a precedencia do que ta em cima*/
          /*se na pilha for menor, empilha*/
          valor = i->valor.vInt;
          if(stackTop().valor.vInt < valor){
            stackPush(*i);
          }
          else{ /*se for maior ou igual, desempilha ate ...*/
            while(!stackEmpty() && stackTop().valor.vInt >= valor){
              if(stackTop().valor.vInt != valor || 
              (i->categoria != OPER_LOGICO_NOT && i->categoria != OPER_EXPONENCIACAO && i->categoria != OPER_MENOS_UNARIO && i->categoria != OPER_ATRIBUICAO)){
                auxiliar = stackPop();
                aux->prox = mallocSafe(sizeof(CelObjeto *));
                aux = aux->prox;
                aux->categoria = auxiliar.categoria;
                aux->valor = auxiliar.valor;
                aux->prox = NULL;
              }
              else{
                  break;
              }
            }
            stackPush((*i));
          }
        }
      }
      else{ /*se nao for operador, coloca na fila*/
        aux->prox = mallocSafe(sizeof(CelObjeto *));
        aux = aux->prox;
        aux->categoria = i->categoria;
        aux->valor = i->valor;
        aux->prox = NULL;
      }
    }
    /*acabou de percorrer a fila infixa*/

    while(!stackEmpty()){
      /*desempilhar o que sobrou na pilha*/
      auxiliar = stackPop();
      aux->prox = mallocSafe(sizeof(CelObjeto *));
      aux = aux->prox;
      aux->categoria = auxiliar.categoria;
      aux->valor = auxiliar.valor;
      aux->prox = NULL;
    }
    freeListaObjetos(iniInfixa);
    stackFree();
    return p;
}
