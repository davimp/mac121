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
  NUSP: 11221988

  IMDB: filmes.c


  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/


/*----------------------------------------------------------*/
/* filmes.h e a interface para as funcoes neste modulo      */
#include "filmes.h" 

/*----------------------------------------------------------*/
#include <stdlib.h>  /* NULL, free() */
#include <stdio.h>   /* printf(), scanf() */ 
#include <string.h>  /* strlen(), strncpy(), strcmp(), strtok() */

#include "util.h"    /* Bool, mallocSafe() */
#include "iofilmes.h"
#include "st.h"      /* freeST(), initST(), putFilmeST(), getFilmeST(),
                        showST(), freeST() */

/*----------------------------------------------------------------------
 *  crieFilme
 *
 *  Recebe informacoes dobre um filme 
 *
 *      - DIST  : distribuicao de notas
 *      - VOTOS : numero de votos
 *      - NOTA  : nota do filme 
 *      - ANO   : ano de producao do filme
 *
 *  e cria uma celula do tipo Filme para armazenar essa informacao. 
 *  A funcao retorna o endereco da celula criada.
 */
Filme *
crieFilme (char dist[], int votos, float nota, char *nome, int ano)
{
    Filme *flm;
    int    len = strlen(nome) + 1; /* +1 para o '\0' */
    
    flm = mallocSafe(sizeof *flm);
    
    strncpy(flm->dist, dist, TAM_DIST+1); /* +1 para o '\0' */
    
    flm->votos = votos;
    flm->nota  = nota;
    
    flm->nome = mallocSafe(len*sizeof(char));
    strncpy(flm->nome, nome, len);
    
    flm->ano  = ano;

    flm->prox = flm->ant = NULL; /* paranoia */
    
    return flm;
}

/*----------------------------------------------------------------------
 *  crieListaFilmes
 * 
 *  Cria uma estrutura que representa lista de filmes vazia.
 *  A funcao retorna o endereco dessa estrutura.
 *
 *  Um lista de filmes e representada por uma lista encadeada 
 *  duplamente ligada com cabeca. 
 */
ListaFilmes *
crieListaFilmes()
{
    ListaFilmes * l;
    Filme * cabeca;
    l = mallocSafe(sizeof(ListaFilmes));
    cabeca = mallocSafe(sizeof(Filme));
    l->cab = cabeca;
    l->nFilmes = 0;
    cabeca->prox = cabeca;
    cabeca->ant = cabeca;
    return l;
}

/*----------------------------------------------------------------------
 *  libereListaFilmes(lst)
 *
 *  Recebe um ponteiro lst para uma estrutura que representa uma lista 
 *  de filmes e libera toda a memoria alocada para a lista.
 *
 *  Esta funcao utiliza a funcao libereFilme().
 */

void
libereListaFilmes(ListaFilmes *lst)
{
    Filme *j;
    Filme *i;
    i = lst->cab->prox;
    while(i->prox != lst->cab)
    {
        j = i->prox;
        i->prox = j->prox;
        libereFilme(j);
    }
    libereFilme(i);
    free(lst->cab);
    free(lst);
}

/*----------------------------------------------------------------------
 *  libereFilme
 *
 *  Recebe um ponteiro FLM para uma estrutura que representa um 
 *  filme e libera a area alocada.
 *
 */
void 
libereFilme(Filme *flm)
{
    if(flm->nome != NULL) free(flm->nome);
    free(flm);
}

/*----------------------------------------------------------------------
 *  insiraFilme
 *
 *  Recebe um ponteito LST para uma estrutura que representa
 *  uma lista de filmes e um ponteiro FLM para uma estrutura
 *  que representa uma filme.
 *
 *  A funcao insere o filme na lista.
 *  
 */
void 
insiraFilme(ListaFilmes *lst, Filme *flm)
{
    flm->prox = lst->cab->prox;
    flm->ant = lst->cab;
    lst->cab->prox->ant = flm;
    lst->cab->prox = flm;
    (lst->nFilmes)++;
}


/*---------------------------------------------------------------------
 *  contemFilme
 *
 *  Recebe um ponteito LST para uma estrutura que representa
 *  uma lista de filmes e um ponteiro FLM para uma estrutura
 *  que representa uma filme.
 *
 *  A funcao retorna  TRUE se o filme esta na lista e 
 *  FALSE em caso contrario. 
 *
 *  Consideramos que dois filmes f e g sao iguais se
 *
 *       - f->nome e' igual a g->nome 
 *       - f->nota e' igual a g->nota
 *       - f->ano  e' igual a g->ano 
 *
 *  Para comparar dois nomes voce pode usar alguma funcao da 
 *  bibliteca do c  como strcmp, strncmp (string,h) 
 *  ou a funcao strCmp (util.h).
 *
 */
Bool 
contemFilme(ListaFilmes *lst, Filme *flm)
{
    Filme *i;
    if(lst->cab->prox != NULL) i = lst->cab->prox;
    else i = lst->cab;
    while(i != lst->cab)
    {
        if(i->nome == flm->nome && i->nota == flm->nota && i->ano == flm->ano) return TRUE;
        i = i->prox;
    }
    return FALSE;
}

/*----------------------------------------------------------------------
 *  removaFilme
 *
 *  Remove da lista de filmes LST o filme apontado por FLM.
 *    
 *  Pre-condicao: a funcao supoe que o filme FLM esta 
 *                na lista LST.
 */
void 
removaFilme(ListaFilmes *lst, Filme *flm)
{
    Filme *i;
    Filme *anterior, *proximo;
    i = lst->cab->prox;
    while(i != lst->cab && strcmp(i->nome, flm->nome)) i = i->prox;
    anterior = i->ant;
    proximo = i->prox;
    anterior->prox = proximo;
    proximo->ant = anterior;
    libereFilme(i);
    (lst->nFilmes)--;
}


/*--------------------------------- */

void intercala(ListaFilmes* lst, ListaFilmes* q, Criterio criterio){
    Filme *i, *j;
    i = lst->cab->prox;
    j = q->cab->prox;
    if(criterio == NOTA){
        while(i != lst->cab && j != q->cab){
            if(i->nota >= j->nota){
                i = i->prox;
            }
            else if(i->nota < j->nota){
                j = j->prox;
                j->ant->ant = i->ant;
                i->ant->prox = j->ant;
                i->ant = j->ant;
                j->ant->prox = i;
            }
        }
        if(j != q->cab){
            lst->cab->ant->prox = j;
            j->ant = lst->cab->ant;
            lst->cab->ant = q->cab->ant;
            lst->cab->ant->prox = lst->cab;
        }
    }
    else{
         while(i != lst->cab && j != q->cab){
            if(strCmp(i->nome, j->nome) <= 0){
                i = i->prox;
            }
            else if(strCmp(i->nome, j->nome) > 0){
                j = j->prox;
                j->ant->ant = i->ant;
                i->ant->prox = j->ant;
                i->ant = j->ant;
                j->ant->prox = i;
            }
        }
        if(j != q->cab){
            lst->cab->ant->prox = j;
            j->ant = lst->cab->ant;
            lst->cab->ant = q->cab->ant;
            lst->cab->ant->prox = lst->cab;
        }
    }
}

/*----------------------------------------------------------------------
 *  mergeSortFilmes
 *
 *  Recebe uma lista de filmes LST e ordena a lista utilizando o
 *  algoritmo mergeSort recursivo adaptado para listas encadeadas
 *  duplamente ligadas com cabeca.
 *
 *  A funcao recebe ainda um parametro CRITERIO tal que:
 *
 *  Se CRITERIO == NOTA, entao a lista deve ser ordenada
 *      em ordem decrescente de nota.
 *
 *  Se CRITERIO == NOME, entao a lista deve ser ordenada
 *      em ordem crescente de nome (ordem alfabetica).
 *
 *  ------------------------------------------------------------------
 *  OBSERVACAO IMPORTANTE:
 *
 *  A ordenacao deve ser feita 'in-place', ou seja o conteudo das
 *  celulas _nao deve_ ser copiado, apenas os ponteiros devem ser
 *  alterados.
 *
 *  A funcao so deve utilizar espaco extra O(1).  
 *
 *  Hmmm, ok, sem levar em consideracao o espaco O(lg n) utilizado
 *  pela pilha da recursao.  Em outras palavras, a funcao pode conter
 *  apenas declaracoes de umas poucas variaveis (um vetor v[0..n]
 *  conta como n variaveis e nao vale).
 *
 *  ------------------------------------------------------------------
 *  Para ordenar por nome, veja a funcao strCmp em util.[h|c].
 */
void 
mergeSortFilmes(ListaFilmes *lst, Criterio criterio)
{
    ListaFilmes *q;
    Filme *i;
    int cont = 0;
    if(lst->nFilmes <= 1) return;
    q = crieListaFilmes();
    i = lst->cab->prox;
    for(cont = 1; cont <= (lst->nFilmes)/2; cont++) i = i->prox;
    q->cab->prox = i;
    q->cab->ant = lst->cab->ant;
    lst->cab->ant->prox = q->cab;
    lst->cab->ant = i->ant;
    i->ant = q->cab;
    lst->cab->ant->prox = lst->cab;

    q->nFilmes = lst->nFilmes - (lst->nFilmes)/2;
    lst->nFilmes = (lst->nFilmes)/2;

    mergeSortFilmes(lst, criterio);
    mergeSortFilmes(q, criterio);

    lst->nFilmes = lst->nFilmes + q->nFilmes;

    intercala(lst, q, criterio);
    libereFilme(q->cab);
    free(q);
}



/*----------------------------------------------------------------------
 *  quickSortFilmes [opcional]
 *
 *  Recebe uma lista de filmes LST e ordena a lista utilizando o
 *  algoritmo quickSort adaptado para listas encadeadas duplamente
 *  ligadas com cabeca.
 *
 *  A funcao recebe ainda um parametro CRITERIO tal que:
 * 
 *  Se CRITERIO == NOTA, entao a lista deve ser ordenada
 *      em ordem decrescente de nota.
 *
 *  Se CRITERIO == NOME, entao a lista deve ser ordenada
 *      em ordem crescente de nome (ordem alfabetica).
 *
 *  ------------------------------------------------------------------
 *  OBSERVACAO IMPORTANTE:
 *
 *  A ordenacao deve ser feita 'in-place', ou seja o conteudo das
 *  celulas _nao deve_ ser copiado, apenas os ponteiros devem ser
 *  alterados.
 *
 *  A funcao so deve utilizar espaco extra O(1).  
 *
 *  Hmmm, ok, sem levar em consideracao o espaco O(lg n) utilizado
 *  pela pilha da recursao.  Em outras palavras, a funcao pode conter
 *  apenas declaracoes de umas poucas variaveis (um vetor v[0..n]
 *  conta como n variaveis e nao vale).
 *
 *  ------------------------------------------------------------------
 *  Para ordenar por nome, veja a funcao strCmp em util.[h|c].
 */
void 
quickSortFilmes(ListaFilmes *lst, Criterio criterio)
{
    AVISO(quickSortFilmes em filmes.c: Vixe ainda nao fiz essa funcao...);
}

/*----------------------------------------------------------------------
 *  hashFilmes [opcional]
 *
 *  Recebe uma lista de filmes LST e distribui as palavras que
 *  ocorrem nos nomes do filmes em uma tabela de dispersao 
 *  (hash table):
 *
 *     http://www.ime.usp.br/~pf/mac0122-2002/aulas/hashing.html
 *     http://www.ime.usp.br/~pf/mac0122-2003/aulas/symb-table.html
 *
 *  Antes de inserir uma palavra na tabela de dispersao todas a
 *  letras da palavra devem ser convertidas para minusculo. Com
 *  isto faremos que a busca de filmes que possuam uma dada
 *  palavra em seu nome nao seja 'case insensitive'. Para essa
 *  tarefa a funcao tolower() pode ser utilizada.
 *
 *  Esta funcao utiliza as funcoes freeST(), initST() e putFilmeST()
 *
 *  Para obter as palavras podemos escrever uma funcao peguePalavra()
 *  inspirada na funcao pegueNome do modulo lexer.c do EP3/EP4 ou
 *  ainda utilizar a funcao strtok() da biblioteca string.h:
 *  
 *      http://linux.die.net/man/3/strtok    (man page)
 *
 */
void
hashFilmes(ListaFilmes *lst)
{
    AVISO(hashFilmes em filmes.c: Vixe ainda nao fiz essa funcao...);
}

