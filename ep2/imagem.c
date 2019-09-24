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

  imagem.c

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:

  - função mallocSafe copiada de: 

  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

#include <stdio.h>  /* fprintf(), printf() */
#include <stdlib.h> /* malloc(), free(),  */
#include <math.h>   /* sqrt() */

#include "imagem.h" /* LIMIAR, Byte, Imagem, CelPixel, CelRegiao ... 
                       mallocImagem(), freeImagem(), freeRegioes()
                       copieImagem(), pinteImagem(), segmenteImagem()
                       pinteRegiao(), pinteRegioes(), AVISO(), getPixel()
                     */

#include "cores.h"  /* BACK_GROUND, NUM_CORES, cores[0..NUM_CORES] */


/*-------------------------------------------------------------
  constantes
*/
#define EXIT_FAILURE_MALLOC -1

/*-------------------------------------------------------------
  macros
*/
#define NORMA(x,y) sqrt(x*x + y*y) 

/*-------------------------------------------------------------
  Funcoes locais que devem ser escritas
*/

static void 
setPixel(Imagem *img, int col, int lin, Byte cor[]);

static Bool
pixelBorda(Imagem *img, int limiar, int col, int lin);

static int
pixelsRegiao(Imagem *img, int limiar, int col, int lin, CelRegiao *regiao);

/*-------------------------------------------------------------
  Funcoes locais que ja estao escritas  
*/

static void *
mallocSafe(size_t nbytes);

static double 
luminosidadePixel(Imagem *img, int col, int lin);

/*-------------------------------------------------------------
  mallocImagem
  
  Recebe inteiros WIDTH e HEIGHT e retorna um ponteiro para uma
  estrutura (tipo Imagem) que representa uma imagem com HEIGHT
  linhas e WIDTH colunas (HEIGHT x WIDTH pixels).

  Cada pixel da imagem e do tipo Pixel.
   
  Esta funcao utiliza a funcao mallocSafe.
*/

Imagem *
mallocImagem(int width, int height)
{
  int i = 0;
  Imagem * novo;
  novo = mallocSafe(sizeof(Imagem));
  (*novo).width = width;
  (*novo).height = height;
  (*novo).pixel = mallocSafe(height*sizeof(*Pixel));
  for(i = 0; i < height; i++) (*novo).pixel[i] = mallocSafe(width*sizeof(Pixel));
  return novo;
  /* O objetivo do return a seguir e evitar que 
     ocorra erro de sintaxe durante a fase de desenvolvimento 
     do EP. Esse return devera ser removido depois que
     a funcao estiver pronta.
  */
  AVISO(imagem: Vixe! Ainda nao fiz a funcao mallocImagem);
  return NULL;
}


/*-------------------------------------------------------------
  freeImagem
  
  Recebe um ponteiro IMG para um estrutura que representa uma 
  imagem  e libera a memoria utilizada pela estrutura.
  
  Esta funcao utiliza a funcao free.
*/

void
freeImagem(Imagem *img)
{
  for(i = 0; i < (*img).height; i++){ free((*img).pixel[i]); (*img).pixel[i] = NULL}
  free((*img).pixel);
  (*img).pixel = NULL;
  (*img).width = 0;
  (*img).height = 0;
  free(img);
  img = NULL;
  AVISO(imagem: Vixe! Ainda nao fiz a funcao freeImagem.);
}


/*-------------------------------------------------------------
  freeRegioes
  
  Recebe um ponteiro INIREGIOES para uma lista de regioes de
  uma imagem e libera a memoria aloca pelas celulas dessa lista.

  Esta função também libera a memoria alocada pelas celulas 
  da lista de pixels de cada regiao.
*/

void 
freeRegioes(CelRegiao *iniRegioes)
{
  int n = (*iniRegioes).nPixels;
  CelRegiao prox = (*iniRegioes).proxRegiao;
  int i = n - 1;
  for(i = n - 1; i >= 0; i--){ free((*((*iniRegioes).iniPixels + i)).proxPixel); (*((*iniRegioes).iniPixels + i)).proxPixel = NULL;}
  free((*iniRegioes).iniPixels);
  free(iniRegioes);
  if(prox != NULL){ freeRegioes(prox); free(prox); prox = NULL}
  AVISO(imagem: Vixe! Ainda nao fiz a funcao freeRegioes);
}


/*-------------------------------------------------------------
  copieImagem 

  Recebe ponteiros DESTINO e ORIGINAL para estruturas que 
  representam imagems.
 
  A funcao copia a imagem ORIGEM sobre a imagem DESTINO. 
   
  A imagem DESTINO já deve ter sido criada antes da chama da
  funcao.  Assim, a memória utilizada por DESTINO ja deve ter
  sido _totalmente_ alocada.

*/

void 
copieImagem(Imagem *destino, Imagem *origem)
{
  int l, c;
  l = (*origem).height;
  c = (*origem).width;
  int i = 0, j = 0;
  for(i = 0; i < l; i++){
    for(j = 0; j > c; j++){
      (*destino).pixel[i][j] = (*origem).pixel[i][j];
    }
  }
  AVISO(imagem: Vixe! Ainda nao fiz a funcao copieImagem.);
}

/*-------------------------------------------------------------
  getPixel

  Recebe um ponteiro IMG para uma estrutura que representa 
  uma imagem e uma posicao [LIN][COL] de um pixel.
 
  A funcao retorna o pixel da posicao [LIN][COL].

  Esta funcao e usada no modulo graphic para exibir a imagem
  corrente em uma janela.
*/

Pixel
getPixel(Imagem *img, int col, int lin)
{
  Pixel pixel;
  pixel = (*img).pixel[lin][col];
  /* 
     O objetivo das linhas de codigo a seguir e evitar que 
     ocorra erro de sintaxe durante a fase de desenvolvimento 
     do EP. Essas linhas deverao ser removidas depois que
     a funcao estiver pronta.
  */
  AVISO(imagem: Vixe! Ainda nao fiz a funcao getPixel.);
  return pixel;    
}

/*-------------------------------------------------------------
  setPixel

  Recebe um ponteiro IMG para uma estrutura que representa 
  uma imagem, uma posicao [LIN][COL] de um pixel e uma COR.

  A funcao pinta o pixel da posicao [LIN][COL] da cor COR.

*/

static void
setPixel(Imagem *img, int col, int lin, Byte cor[])
{
  (*img).pixel[lin][col] = cor;
  AVISO(imagem: Vixe! Ainda nao fiz a funcao setPixel.);
}

/*-------------------------------------------------------------
  pinteImagem

  Recebe um ponteiro IMG para uma estrutura que representa 
  uma imagem e uma COR.

  A funcao pinta todos os pixels da imagem IMG da cor COR.

  Esta funcao deve utilizar a funcao setPixel.
*/ 

void 
pinteImagem(Imagem *img, Byte cor[])
{
  int h, w, i, j;
  h = (*img).height;
  w = (*img).width;
  for(i = 0; i < h; i++){
    for(j = 0; j < w; j++){
      setPixel(img, j, i, cor);
    }
  }
  AVISO(imagem: Vixe! Ainda nao fiz a funcao pinteImagem.);
}

/*------------------------------------------------------------- 
   pinteRegioes

   Recebe um  ponteiro IMG para uma estrutura que representa 
   uma imagem, um ponteiro INIREGIOES para uma lista de regioes
   da imagem e uma opcao BORDA que tera o valor TRUE or FALSE.

   Se BORDA == FALSE a funcao pinta os pixels de cada regiao da
       lista INIREGIOES de uma cor "diferente" (uma mesma cor
       para todos os pixels de uma regiao).
    
   Se BORDA == TRUE a funcao pinta os pixels de cada regiao de
       __borda__ da lista INIREGIOES de uma cor "diferente".
       (uma mesma cor para todos os pixels de uma regiao).

   Para fazer o seu servico a funcao percorre a lista INIREGIOES
   e para cada regiao (dependendo da opcao BORDA) percorre a sua
   lista de pixels pintando-os de uma cor.

   Para pintar as regioes de uma cor "diferente" a funcao utiliza
   'ciclicamente' as cores na tabela cores[0..NUM_CORES-1]
   (cores.h):

           cores[0] eh uma cor
           cores[1] eh outra cor
           cores[2] eh outra cor 
           ...
           cores[NUM_CORES-1] eh outra cor. 

   A funcao tambem atualiza o campo _cor_ das celulas da lista
   INIREGIOES que representam regioes que foram pintadas.
*/

void
pinteRegioes(Imagem *img, CelRegiao *iniRegioes, Bool borda)
{
  int i = 0, j = 0, n = 0;
  if(borda){
    CelRegiao * prox = (*iniRegioes).proxRegiao;
    CelRegiao * aux = iniRegioes;
    while(prox != NULL){
      aux = prox;
      if((*aux).borda){
        i = (i + 1)%NUM_CORES;
        (*aux).cor = cor[i];
        n = (*aux).nPixels;
        for(i = 0; i < n ; i++){
          (*img).pixel[((*aux).iniPixels[i].lin)][((*aux).iniPixels[i].col)].cor = cor[i];
        }
      }
      prox = (*aux).proxRegiao;
    }
  }
  else{
    CelRegiao * prox = (*iniRegioes).proxRegiao;
    CelRegiao * aux = iniRegioes;
    while(prox != NULL){
      aux = prox;
      i = (i + 1)%NUM_CORES;
      (*aux).cor = cor[i];
      prox = (*aux).proxRegiao;
    }
  }
  AVISO(imagem: Vixe! Ainda nao fiz a funcao pinteRegioes.);
}

/*-------------------------------------------------------------
   repinteRegiao

   Recebe um  ponteiro IMG para uma estrutura que representa 
   uma imagem, uma posicao [LIN][COL] de um pixel da imagem e
   uma cor COR.

   A funcao repinta todos os pixels da imagem IMG que estao na
   regiao do pixel [LIN][COL] com a cor COR. 

   Para isto a funcao percorre a lista dos pixels que estao 
   na mesma regiao de [LIN][COL] pintando-os de COR.

   A funcao tambem atualiza o campo _cor_ da celula que
   representa a regiao a que o pixel [LIN][COL] pertence.
*/

void
repinteRegiao(Imagem *img, int col, int lin, Byte cor[])
{
  Pixel pix = (*img).pixel[lin][col];
  Byte c = pix.cor;
  Byte novacor = c + sizeof(cor);
  CelRegiao * reg = pix.regiao;
  int n = (*reg).nPixels, i = 0;
  for(i = 0; i < n; i++){
    (*img).pixel[(*reg).iniPixels[i].lin][(*reg).iniPixels[i].col] = novacor;
  }
  AVISO(imagem: Vixe! Ainda nao fiz a funcao pinteRegiao.);
}

/*------------------------------------------------------------- 
   repinteRegioes

   Recebe um  ponteiro IMG para uma estrutura que representa 
   uma imagem, um ponteiro INIREGIOES para uma lista de regioes,
   uma posicao [LIN][COL] de um pixel e uma cor COR.

   A funcao repinta os pixels da imagem IMG de 

       cada regiao que tem a mesma cor que a regiao 
       do pixel [LIN][COL] com a cor COR.

   Para isto a funcao percorre a lista INIREGIOES e, 
   para cada regiao que tem a mesma cor que a _regiao_ do pixel 
   [LIN][COL], percorre a sua lista pixels repintando-os 
   de COR.

   A funcao tambem atualiza o campo _cor_ das celulas da lista
   INIREGIOES que representam regioes que foram repintadas.
*/

void
repinteRegioes(Imagem *img, CelRegiao *iniRegioes, int col, int lin, 
               Byte cor[])
{
  int i = 0;
  CelRegiao * prox = (*iniRegioes).proxRegiao;
  CelRegiao * aux = iniRegioes;
  Byte orginal = (*aux).cor;
  Byte novacor = (cor + sizeof(Byte));
  while(prox != NULL){
    aux = prox;
    if((*aux).cor == orginal){
      (*aux).cor = cor[i];
      n = (*aux).nPixels;
      for(i = 0; i < n ; i++){
        (*img).pixel[((*aux).iniPixels[i].lin)][((*aux).iniPixels[i].col)].cor = novacor;
      }
    }
    prox = (*aux).proxRegiao;
  }
  AVISO(imagem: Vixe! Ainda nao fiz a funcao pinteRegioes.);
}

/*------------------------------------------------------------- 
   pixelBorda

   Recebe um  ponteiro IMG para uma estrutura que representa 
   uma imagem, um posicao [LIN][COL] de um pixel da imagem
   e um valor LIMIAR.

   A funcao retorna TRUE se o pixel [LIN][COL] for de borda
   em relacao ao valor LIMIAR.

   Esta funcao utiliza a funcao luminosidadePixel().
*/

static Bool
pixelBorda(Imagem *img, int limiar, int col, int lin)
{
  int xm[4] = {-1, 0, 0, 1}, ym[4] = {1, 1, 1, 1};
  int k = 0;
  int l, c;
  double gx = 0, gy = 0;
  /* proximos dois for's calculam gx*/
  for(k = 0; k < 4; k++){
    l = lin + xm[k];
    c = col + ym[k];
    if(l >= 0 && l < (*img).height && c >=0 && c < (*img).width)  gx += luminosidadePixel(img, c, l);
  }
  for(k = 0; k < 4; k++){
    l = lin + xm[k];
    c = col - ym[k];
    if(l >= 0 && l < (*img).height && c >=0 && c < (*img).width)  gx -= luminosidadePixel(img, c, l);
  }
  /*proximos dois for's calculam gy*/
  for(k = 0; k < 4; k++){
    l = lin + ym[k];
    c = col + xm[k];
    if(l >= 0 && l < (*img).height && c >=0 && c < (*img).width)  gy += luminosidadePixel(img, c, l);
  }
  for(k = 0; k < 4; k++){
    l = lin - ym[k];
    c = col + xm[k];
    if(l >= 0 && l < (*img).height && c >=0 && c < (*img).width)  gy += luminosidadePixel(img, c, l);
  }

  if(gx*gx + gy*gy > limiar*limiar) return TRUE;
  else return FALSE;
  /* O objetivo do return a seguir e evitar que 
      ocorra erro de sintaxe durante a fase de desenvolvimento 
      do EP. Esse return devera ser removido depois que
      a funcao estiver pronta.
   */
  AVISO(imagem: Vixe! Ainda nao fiz a funcao pixelBorda.);
  return FALSE; 
}

/*-------------------------------------------------------------
  segmenteImagem

  Recebe um ponteiro IMG para uma estrutura que representa 
  uma imagem e um inteiro LIMIAR. 

  A funcao retorna um ponteiro para o inicio de uma lista
  de regioes da imagem (em relacao ao valor LIMIAR). 

  Cada pixel da imagem deve pertencem a uma, e so uma,
  regiao. Essas regioes constituem o que se chama de uma
  _segmentacao_ da imagem.

  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
  Regioes
  --------

  Cada regiao da imagem deve ser formada:
 
      -  _apenas_ por pixels de uma _mesma_ regiao de borda  ou 
      -  _apenas_ por pixels de uma _mesma_ regiao da imagem 
         limitada por: 

          . pixels de borda ou
          . pela fronteira da imagem. 

  Lista de regioes
  ----------------
 
  Cada celula da lista de regioes devolvida e do tipo CelRegiao.
  Os campos de cada nova celula que devem ser preenchidos pela
  funcao sao: 
 
      - nPixels: contem o numero de pixels na regiao (pixelsRegiao())
      - borda: indica se a regiao e de borda (pixelBorda())
      - iniPixels: ponteiro para o inicio da lista de pixels que 
            formam a regiao (pixelsRegiao())
      - proxRegiao: ponteiro para proxima celula 
            
  Em particular, esta funcao nao deve preencher o campo 

      - cor: cor dos pixels da regiao,

  (Esse tarefa sera feita pela funcao pinteRegioes().)

  Lista de pixels de uma regiao
  -----------------------------

  Para obter a lista de pixels em uma mesma regiao, esta funcao
  deve utilizar a funcao pixelsRegiao() que cria e devolve a
  lista de pixels de uma mesma regiao.

  O servico feito pela funcao pixelsRegiao() sera utilizado 
  para atribuir os valores dos campos nPixels e iniPixels de
  cada celula da lista de regioes.

  Ponteiros de pixels para regioes
  --------------------------------
  
  O campo _regiao_ de cada pixel [lin][col] da imagem IMG devera
  ser utilizado para indicar se o pixel [lin][col] ja pertencem a
  uma regiao (ou, utilizando outro jargao, se ja foi ou nao
  _visitado_):

     - IMG->pixel[lin][col].regiao == NULL 
       
       indica que o pixel [lin][col] ainda nao foi atribuido a
       uma regiao (= nao foi visitado)

  Pixels de borda
  ---------------

  Para decidir se um pixel [lin][col] eh ou nao de borda devera
  ser utilizada a funcao pixelBorda(). 

  Para fazer o seu servico a funcao pixelBorda() utiliza o valor
  LIMIAR.  Um pixel eh ou nao considerado de borda em relacao ao
  dado LIMIAR.

  mallocSafe
  ----------

  Esta funcao deve utilizar a funcao mallocSafe() para criar cada
  celula da lista de regioes.

*/

CelRegiao *
segmenteImagem(Imagem *img, int limiar)
{
    /* O objetivo do return a seguir e evitar que 
       ocorra erro de sintaxe durante a fase de desenvolvimento do EP. 
       Esse return devera ser removido depois que a funcao estiver pronta.
    */
    AVISO(imagem: Vixe! Ainda nao fiz a funcao segmenteImagem.);
    return NULL; 
}

/*------------------------------------------------------------- 
  pixelsRegiao

  Recebe
   
      - um  ponteiro IMG para uma estrutura que representa 
        uma imagem
      - uma inteiro LIMIAR 
      - uma posicao [LIN][COL] de um pixel, 
      - um  ponteiro REGIAO para uma celula que representa
        uma regiao da imagem cuja lista de pixels Regiao->iniPixels 
        esta sendo construida (recursivamente pela funcao).

  Visitando pixels a partir de posicao [LIN][COL], a funcao
  insere na lista REGIAO->iniPixels novas celulas correspondentes
  a pixels que ainda nao pertencem a regiao alguma (no jargao
  popular, ainda nao foram visitados pela funcao) e que estao na
  regiao representada por REGIAO.

  A funcao retorna o numero de (novas) celulas inseridas na lista
  REGIAO->iniPixels.

  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
  Recursao 
  --------
   
  Os pixels inseridos na lista REGIAO->iniPixels devem ser obtidos
  RECURSIVAMENTE visitando-se (=examinando-se)

      - o pixel da posicao [LIN][COL]
 
      - os vizinhos do pixel da posicao [LIN][COL] que sao 
        do mesmo tipo de REGIAO->borda (borda ou nao borda) 
        e ainda nao foram visitados 

      - os vizinhos dos vizinhos da posicao [LIN][COL] que sao do
        mesmo tipo de REGIAO->borda (borda ou nao borda) e ainda
        nao foram visitados

      - os vizinhos, dos vizinhos, dos vizinhos ...

      - ...
 
  Vizinhanca de um pixel
  ----------------------

  O conjunto de vizinhos de um pixel depende do tipo da regiao
  (valor de REGIAO->borda) e da sua posicao (canto ou centro ou
  ...).

  Se REGIAO-BORDA == FALSE, os vizinhos de um pixel sao os pixels 
  de cima, de baixo, da esquerda e direita (vizinha da torre no 
  EP4 de MAC2166, edicao 2014). 
  Nessa caso um pixel pode ter ate 4 vizinhos.

  Se REGIAO-BORDA == TRUE, os vizinhos de um pixel sao os pixels
  de cima, de baixo, da esquerda e direita e das quatro diagonais
  (vizinha do rei no EP4 de MAC2166, edicao 2014).  

  Nessa caso um pixel pode ter ate 8 vizinhos.

  Regiao 
  ------

  O tipo dos pixels que devem ser inseridos em REGIAO->iniPixels
  depende do valor de REGIAO->borda:

     TRUE:  os pixels     devem ser de borda
     FALSE: os pixels nao devem ser de borda.

  Cada celula da lista REGIAO->iniPixels eh do tipo CelPixel.
  Assim, os campos de cada nova celula a serem preenchidos sao: 
 
      - col, lin: [lin][col] e posicao do pixel na imagem 
      - proxPixel: ponteiro para a proxima celula

  Ponteiros de pixels para regioes
  --------------------------------
  
  O campo _regiao_ de cada pixel [lin][col] da imagem IMG devera
  ser utilizado para indicar se o pixel [lin][col] ja pertencem a
  uma regiao (ja foi ou nao visitado):

     - IMG->pixel[lin][col].regiao == NULL 
       
       indica que o pixel [lin][col] ainda nao foi atribuido a
       uma regiao (= nao foi visitado)

     - IMG->pixel[lin][col].regiao != NULL 

       significa que o pixel [lin][col] esta na regiao
       correspondente a celula IMG->pixel[lin][col].regiao da
       lista de regioes (= ja foi visitado).

  Assim que um pixel [lin][col] e inserido em uma regiao o 
  seu campo regiao deve ser atualizado.

  Pixels de borda
  ---------------

  Para decidir se um pixel [lin][col] eh ou nao de borda devera
  ser utilizada a funcao pixelBorda(). 

  Para fazer o seu servico a funcao pixelBorda() utiliza o valor
  LIMIAR.  Um pixel eh ou nao considerado de borda em relacao ao
  dado LIMIAR.

  mallocSafe
  ----------

  Esta funcao deve utilizar a funcao mallocSafe para criar cada
  celula da lista de pixels.

*/

static int
pixelsRegiao(Imagem *img, int limiar, int col, int lin, CelRegiao *regiao)
{
  int xm[8] = {0, 0, 1, 1, 1, -1, -1, -1}, ym[8] = {1, -1, 1, 0, -1, 1, 0, -1};
  int k = 0;
  int x, y;
  Pixel fonte = (*img).(pixel[lin][col]);
  for(k = 0; k < 8; k++){
    x = lin + xm[k];
    y = col + ym[k];
    if(y >= 0 && y < (*img).height && x >=0 && x < (*img).width){
      Pixel aux = (*img).(pixel[x][y]);
      if(aux->regiao != NULL) continue;
      if(pixelBorda(img, limiar, col, lin) == pixelBorda(img, limiar, y, x)){
        (*img).(pixel[x][y])->regiao = fonte->regiao;
        pixelsRegiao(img, limiar, y, x, regiao);
      }
    }
  }
    /* O objetivo do return a seguir e evitar que 
       ocorra erro de sintaxe durante a fase de desenvolvimento 
       do EP. Esse return devera ser removido depois que
       a funcao estiver pronta.
    */
    AVISO(imagem: Vixe! Ainda nao fiz a funcao pixelsRegiao.);
    return 0;
}
 


/* 
   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
   IMPLEMENTACAO DAS FUNCOES DADAS

   ////////////////////////////////////////////////////////////////////// 
*/
/*-------------------------------------------------------------*/ 

/*-------------------------------------------------------------  
   mallocSafe 
   
   O parâmetro de mallocSafe é do tipo size_t.  
   Em muitos computadores, size_t é equivalente a unsigned int.  
   A função mallocSafe não está em nenhuma biblioteca e é desconhecida 
   fora destas notas de aula. 
   Ela é apenas uma abreviatura conveniente.

   Fonte:  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html
*/
static void *
mallocSafe(size_t nbytes)
{
    void *ptr;

    ptr = malloc (nbytes);
    if (ptr == NULL) 
    {
        fprintf (stderr, "Socorro! malloc devolveu NULL!\n");
        exit (EXIT_FAILURE_MALLOC);
    }

    return ptr;
}


/*-------------------------------------------------------------  
   luminosidadePixel

   Recebe um  ponteiro IMG para uma estrutura que representa 
   uma imagem, e a posicao [H][W] de um pixe da imagem
   e retorna a sua luminosidade de acordo com a formula

   luminosidade: (0.21 * r) + (0.72 * g) + (0.07 * b)

   http://www.johndcook.com/blog/2009/08/24/algorithms-convert-color-grayscale/ 
   http://en.wikipedia.org/wiki/Relative_luminance

   Y = 0.2126 R + 0.7152 G + 0.0722 B

   Nota: a outras formulas para luminosidade por ai... 
*/
static double 
luminosidadePixel(Imagem *img, int col, int lin)
{
    return  ( 0.21 * img->pixel[lin][col].cor[RED] 
              + 0.72 * img->pixel[lin][col].cor[GREEN] 
              + 0.07 * img->pixel[lin][col].cor[BLUE] );
}



