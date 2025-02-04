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

  stack.h
  Pitao II
  cla
  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.

  Exemplo:
  - função mallocc retirada de: 

  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/*
 * MAC0121 Algoritmos e Estruturas de Dados I
 *
 * stack.h 
 * INTERFACE: funcoes para manipular uma pilha 
 */
#include<stdio.h>
#include<stdlib.h>
#include"objetos.h"
#include"util.h"
#ifndef _STACK_H
#define _STACK_H

/*
 * TAREFA EP3
 *
 * Aqui voce deve colocar a interface para acesso a sua   
 * implementacao de uma pilha (stack.c).
 * 
 * Se julgar conveniente, defina o arquivo item.h o tipo Item
 * como nas notas de aula.
 *
 * TAREFA EP4: copiar a interface feita para o EP3.
 *
 */

#endif /* _STACK_H */


typedef CelObjeto* Item;

/* cada celula da stack e'um CelObjeto*/
typedef CelObjeto StackNode;

StackNode* s; /*inicio da pilha*/

/* inicializa a stack */  
void stackInit();

/* devolve se a pilha esta vazia(1) ou nao(0)*/
int stackEmpty();

/* coloca um novo item no topo da pilha */
void stackPush(Item item);

/* remove o elemento do topo da pilha e devolve ele (o item)*/
Item stackPop();

/* devolve o valor do CelObjeto que está no topo da pilha*/
Item stackTop();

void stackFree();

/* imprime cada elemento(item, valor do CelObjeto) da pilha, de cima para baixo*/
void stackPrint();