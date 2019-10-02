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
  novo->width = width;
  novo->height = height;
  novo->pixel = mallocSafe(height*sizeof(Pixel *));
  for(i = 0; i < height; i++) novo->pixel[i] = mallocSafe(width*sizeof(Pixel));
  return novo;
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
  int i = 0;
  for(i = 0; i < img->height; i++){ free(img->pixel[i]); img->pixel[i] = NULL;}
  free(img->pixel);
  img->pixel = NULL;
  (*img).width = 0;
  (*img).height = 0;
  free(img);
  img = NULL;
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
  CelRegiao *i, *q; CelPixel *p, *aux;
  i = iniRegioes;
  while(i != NULL){
    p = i->iniPixels;
    if(p->proxPixel != NULL) aux = p->proxPixel;
    else aux = NULL;
    while(aux != NULL){
      p->proxPixel = aux->proxPixel;
      free(aux);
      aux = p->proxPixel;
    }
    free(p);
    p = NULL;
    q = i;
    i = i->proxRegiao;
    free(q);
    q = NULL;
  }
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
  l = origem->height;
  c = origem->width;
  destino->width = c;
  destino->height = l; 
  int i = 0, j = 0;
  for(i = 0; i < l; i++){
    for(j = 0; j < c; j++){
      destino->pixel[i][j] = origem->pixel[i][j];
    }
  }
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
  pixel = img->pixel[lin][col];
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
  img->pixel[lin][col].cor[0] = cor[0];
  img->pixel[lin][col].cor[1] = cor[1];
  img->pixel[lin][col].cor[2] = cor[2];
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
  h = img->height;
  w = img->width;
  for(i = 0; i < h; i++){
    for(j = 0; j < w; j++){
      setPixel(img, j, i, cor);
    }
  }
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

int colorir = 0;
void
pinteRegioes(Imagem *img, CelRegiao *iniRegioes, Bool borda)
{
  int i = 0, j = 0;
  Byte * cor;
  cor = cores[0];
  if(borda){
    CelRegiao *q; CelPixel *p;
    for(q = iniRegioes; q != NULL; q = q->proxRegiao){
      colorir = (colorir+1)%NUM_CORES;
      cor = cores[colorir];
      if(q->borda){
        q->cor[0] = cor[0];
        q->cor[1] = cor[1];
        q->cor[2] = cor[2];
        p = q->iniPixels;
        for(; p != NULL; p = p->proxPixel){
          i = p->lin;
          j = p->col;
          setPixel(img, j, i, cor);
        }
      }
    }
  }
  else{
    CelRegiao *q; CelPixel *p;
    for(q = iniRegioes; q != NULL; q = q->proxRegiao){
      colorir = (colorir+1)%NUM_CORES;
      cor = cores[colorir];
      p = q->iniPixels;
      q->cor[0] = cor[0];
      q->cor[1] = cor[1];
      q->cor[2] = cor[2];
      for(; p != NULL; p = p->proxPixel){
        i = p->lin;
        j = p->col;
        setPixel(img, j, i, cor);
      }
    }
  }
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
  CelRegiao *reg;
  CelPixel *p;
  reg = img->pixel[lin][col].regiao;
  p = reg->iniPixels;
  reg->cor[0] = cor[0];
  reg->cor[1] = cor[1];
  reg->cor[2] = cor[2];
  for(; p!= NULL; p = p->proxPixel){
    setPixel(img, p->col, p->lin, cor);
  }
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
  Byte original[3];
  CelRegiao *r;
  CelRegiao *aux;
  aux = img->pixel[lin][col].regiao;
  original[0] = aux->cor[0];
  original[1] = aux->cor[1];
  original[2] = aux->cor[2];

  for(r = iniRegioes; r != NULL; r = r->proxRegiao){
    if((r->cor[0] == original[0]) && (r->cor[1] == original[1]) && (r->cor[2] == original[2])){
      r->cor[0] = cor[0]; r->cor[1] = cor[1]; r->cor[2] = cor[2];
      repinteRegiao(img, r->iniPixels->col, r->iniPixels->lin, cor);
    }
  }
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
  int v[4] = {-1, 0, 0, 1};
  int k = 0;
  int l, c;
  double gx = 0, gy = 0;
  /* proximos dois for's calculam gx*/
  for(k = 0; k < 4; k++){
    l = lin + v[k];
    c = col + 1;
    if(l >= 0 && l < (*img).height && c >= 0 && c < (*img).width)  gx += luminosidadePixel(img, c, l);
  }
  for(k = 0; k < 4; k++){
    l = lin + v[k];
    c = col - 1;
    if(l >= 0 && l < (*img).height && c >= 0 && c < (*img).width)  gx -= luminosidadePixel(img, c, l);
  }
  /*proximos dois for's calculam gy*/
  for(k = 0; k < 4; k++){
    l = lin + 1;
    c = col + v[k];
    if(l >= 0 && l < (*img).height && c >= 0 && c < (*img).width)  gy += luminosidadePixel(img, c, l);
  }
  for(k = 0; k < 4; k++){
    l = lin - 1;
    c = col + v[k];
    if(l >= 0 && l < (*img).height && c >= 0 && c < (*img).width)  gy -= luminosidadePixel(img, c, l);
  }

  if(NORMA(gx, gy) > limiar) return TRUE;
  else return FALSE;

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
  CelRegiao * aux = NULL, *ini = NULL;
  Pixel **p;
  p = img->pixel;
  int i = 0, j = 0, h, w;
  h = img->height; w = img->width;

  for(i = 0; i < h; i++)
    for(j = 0; j < w; j++)
      img->pixel[i][j].regiao = NULL;

  for(i = 0; i < h; i++){
    for(j = 0; j < w; j++){
      if(p[i][j].regiao == NULL){
        aux = mallocSafe(sizeof(CelRegiao));
        aux->borda = pixelBorda(img, limiar, j, i);
        aux->iniPixels = NULL;
        aux->nPixels = pixelsRegiao(img, limiar, j, i, aux);
        aux->proxRegiao = ini;
        ini = aux;
      }
    }
  }
  return ini;
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
int xm[8] = {1, -1, 0, 0, 1, 1, -1, -1}, ym[8] = {0, 0, 1, -1, 1, -1, 1, -1};
static int
pixelsRegiao(Imagem *img, int limiar, int col, int lin, CelRegiao *regiao)
{
  int k = 0;
  int x, y;
  int novas = 0;
  CelPixel *aux;
  aux = mallocSafe(sizeof(CelPixel));
  aux->lin = lin;
  aux->col = col;
  aux->proxPixel = regiao->iniPixels;
  regiao->iniPixels = aux;
  img->pixel[lin][col].regiao = regiao;
  if(regiao->borda){
    for(k = 0; k < 8; k++){
      x = lin + xm[k];  
      y = col + ym[k];
      if(x >= 0 && y >= 0 && y < (img->width) && x < (img->height)){
        if(pixelBorda(img, limiar, y, x)){
          if(img->pixel[x][y].regiao == NULL){
            img->pixel[x][y].regiao = regiao;
            novas += 1;
            novas += pixelsRegiao(img, limiar, y, x, regiao);
          }
        }
      }
    }
  }
  else{
    for(k = 0; k < 4; k++){
      x = lin + xm[k];
      y = col + ym[k];
      if(x >= 0 && y >= 0 && y < (img->width) && x < (img->height)){
        if(!pixelBorda(img, limiar, y, x)){
           if(img->pixel[x][y].regiao == NULL){
            img->pixel[x][y].regiao = regiao;
            novas += 1;
            novas += pixelsRegiao(img, limiar, y, x, regiao);
          }
        }
      }
    }
  }
  return novas;
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



