/*\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__

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

  ep1.c

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:

  - função randomInteger() de: 

  http://www.ime.usp.br/~pf/algoritmos/aulas/random.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__*/

/*
 *  Sobre os nomes da variaveis:
 *
 *  Adotamos a especificacao das variaveis em (lower) "camel case":
 * 
 *       http://en.wikipedia.org/wiki/Camel_case
 *
 *  Sobre as especificações das funções:
 *
 *  Nos protótipos das funções, os nomes dos parametros
 *  seguem o padrao camel case. No entanto, nas especificações,
 *  esses nomes aparecem vertidos para maiúsculas para destacar 
 *  o papel de cada parâmetro. Isso corresponde a recomendacao do 
 *  GNU Coding Standards:
 *
 *       http://www.gnu.org/prep/standards/standards.html#Comments
 */

#include <stdio.h>  /* scanf(), printf(), ... */
#include <stdlib.h> /* srand(), rand(), atoi(), exit(), ...  */
#include <string.h> /* strlen(), strcmp(), ... */  

/*---------------------------------------------------------------*/
/* 
 * 0. C O N S T A N T E S 
 */

/* tamanho máximo de um turtledorm */
#define MAX      128

/* estado da turtle */
#define ACORDADO   '#'
#define DORMINDO   ' '
#define TAPINHA    'T'

#define TRUE       1
#define FALSE      0

#define ENTER      '\n'
#define FIM        '\0'
#define ESPACO     ' '

/*---------------------------------------------------------------*/
/*
 * 1. P R O T Ó T I P O S
 */

/* PARTE I */
void
leiaTurtledorm(int *nLin, int *nCol, int tDorm[][MAX]);

void 
mostreTurtledorm(int nLin, int nCol, int tDorm[][MAX], char c);

void
tapinhaTurtle(int nLin, int nCol, int tDorm[][MAX], int lin, int col);

int 
todosDormindo(int nLin, int nCol, int tDorm[][MAX]);

/* PARTE II */
void
sorteieTurtledorm(int *nLin, int *nCol, int tDorm[][MAX]);

int
graveTurtledorm(int nLin, int nCol, int tDorm[][MAX]);

/* PARTE III */

void
resolvaTurtledorm(int nLin, int nCol, int tDorm[][MAX]);

/* FUNÇõES AUXILIARES */
int 
randomInteger(int low, int high);

void 
pause();

int
valido(int x, int y, int nLin, int nColn);


/*---------------------------------------------------------------*/
/* 
 *  M A I N 
 */
int 
main(int argc, char *argv[])
{
    int lin, col, dorm[MAX][MAX];
    char soul; 
    printf("(s)ortear ou (l)er um turtledorm de arquivo: ");
    scanf("%c", &soul);
    if(soul == 's'){
      sorteieTurtledorm(&lin, &col, dorm);
    }
    else{
      leiaTurtledorm(&lin, &col, dorm);
    }

    mostreTurtledorm(lin, col, dorm, ACORDADO);

    int continuar = TRUE;
    char comando[MAX];
    int x, y;
    while(continuar){
      printf("Digite sua próxima jogada: ");
      scanf("%s", comando);
      if(strlen(comando) > 1){
        x = comando[0] - '0';
        y = comando[2] - '0';
        tapinhaTurtle(lin, col, dorm, x, y);
        mostreTurtledorm(lin, col, dorm, ACORDADO);
      }
      else{
        char auxiliar = comando[0];
        if(auxiliar == 'a'){
          resolvaTurtledorm(lin, col, dorm);
       }
        else if(auxiliar == 'g'){
          graveTurtledorm(lin, col, dorm);       
        }
        else{
          printf("Voce desistiu da partida\n");
          continuar = FALSE;
          pause();
        }
      }
      if(todosDormindo(lin, col, dorm)){
        printf("Parabéns, voce terminou o jogo!");
        continuar = FALSE;
        pause();
      }
    }

    return EXIT_SUCCESS;
}


/*---------------------------------------------------------------*/
/* 
 *  I M P L E M E N T A Ç Ã O   D A S   F U N Ç Õ E S   DA  
 *                        P A R T E   I 
 */

void leiaTurtledorm(int *nLin, int *nCol, int tDorm[][MAX]){
  FILE *arquivo_de_entrada;
  char nome_arquivo[80];
  printf("Digite o nome do arquivo de dados: ");
  scanf("%s", nome_arquivo);
  arquivo_de_entrada = fopen(nome_arquivo, "r"); /*abre o arquivo para leitura*/
  int linha = 0, coluna = 1;
  while(!feof(arquivo_de_entrada)){
      if(linha == 0){
          fscanf(arquivo_de_entrada,"%d %d\n", nLin, nCol);
      }
      else{
          coluna = 1;
          char string[300];
          if(fgets(string, 300, arquivo_de_entrada)){
              for(int aux = 0; aux < strlen(string); aux+=2){
                  tDorm[linha][coluna] = string[aux] - '0';
                  coluna++;
              }
          }
      }
      linha++;
  }
  fclose(arquivo_de_entrada);
}

void mostreTurtledorm(int nLin, int nCol, int tDorm[][MAX], char c){
  if(c == ACORDADO){
    printf("  ");
    for(int k = 1; k <= nCol; k++){
      printf("   %d  ", k);
    }
    printf("\n");
    for(int i = 1; i <= nLin; i++){
      printf("  +");
      for(int k = 1; k <= nCol; k++){
        printf("-----+");
      }
      printf("\n");
      printf("%d |", i);
      for(int j = 1; j <= nCol; j++){
        if(tDorm[i][j] == 1){
          printf("  #  |");
        }
        else{
          printf("     |");
        }
      }
      printf("\n");
    }
    printf("  +");
    for(int k = 0; k < nCol; k++){
      printf("-----+");
    }
    printf("\n");
  }
  else{
    /*IMPRIMIR SOLUÇÃO ÓTIMA*/ 
  }
}

void tapinhaTurtle(int nLin, int nCol, int tDorm[][MAX], int lin, int col){
  int xm[5] = {0 , 0 , 1, -1, 0}, ym[5] = {1, -1, 0, 0, 0};
  int x, y;
  for(int k = 0; k < 5; k++){
    x = lin + xm[k];
    y = col + ym[k];
    if(valido(x, y, nLin, nCol)){
      tDorm[x][y] = !tDorm[x][y];
    }
  }
}


int todosDormindo(int nLin, int nCol, int tDorm[][MAX]){
  for(int i = 1; i <= nLin; i++){
    for(int j = 1; j <= nCol; j++){
      if(tDorm[i][j] == 1) return FALSE;
    }
  }
  return TRUE;
}


/*---------------------------------------------------------------*/
/* 
 *  I M P L E M E N T A Ç Ã O   D A S   F U N Ç Õ E S   DA  
 *                       P A R T E   II 
 */

void sorteieTurtledorm(int *nLin, int *nCol, int tDorm[][MAX]){
  int semente;
  char dificuldade;
  printf("Qual a dimensão do turtledorm? :");
  scanf("%d %d", nLin, nCol);
  printf("Qual a semente para sorteio? :");
  scanf("%d", &semente);
  printf("Qual a dificuldade [f/m/d]? : ");
  scanf("%c\n", &dificuldade);
  for(int i = 0; i <= (*nLin); i++){
    for(int j = 0; j <= (*nCol); j++){
      tDorm[i][j] = 0;
    }
  }
  int nTapinhas;
  int quantasCasas = (*nLin)*(*nCol);
  if(dificuldade == 'f'){
    nTapinhas = randomInteger((int)(0.05*quantasCasas),(int)(0.2*quantasCasas));
  }
  else if(dificuldade == 'm'){
    nTapinhas = randomInteger((int)(0.25*quantasCasas),(int)(0.5*quantasCasas));
  }
  else{
    nTapinhas = randomInteger((int)(0.55*quantasCasas),(int)(0.85*quantasCasas));
  }

  for(int k = 0; k < nTapinhas; k++){
    tapinhaTurtle(*nLin, *nCol, tDorm, randomInteger(1, (*nLin)), randomInteger(1, (*nCol)));
  }
}


int graveTurtledorm(int nLin, int nCol, int tDorm[][MAX]){
  FILE *arquivo_de_saida;
  char nome_arquivo[80];
  printf("Digite o nome do arquivo de dados: ");
  scanf("%s", nome_arquivo);
  arquivo_de_saida = fopen(nome_arquivo, "w"); /*abre o arquivo para escrita*/

  if(arquivo_de_saida == NULL){
    return EXIT_FAILURE;
  }
  else{
    fprintf(arquivo_de_saida, "%d %d", nLin, nCol);
    for(int i = 1; i <= nLin; i ++){
      fprintf(arquivo_de_saida, "\n");
      for(int j = 1; j <= nCol; j++){
	fprintf(arquivo_de_saida, "%d ",tDorm[i][j]);
      }
    }
    return EXIT_SUCCESS;
  }
  fclose(arquivo_de_saida);
}
/*---------------------------------------------------------------*/
/* 
 *  I M P L E M E N T A Ç Ã O   D A   F U N Ç Ã O   DA  
 *                       P A R T E   III 
 */

void resolvaTurtledorm(int nLin, int nCol, int tDorm[][MAX]){

}
/*---------------------------------------------------------------*/
/* 
 *  I M P L E M E N T A Ç Ã O   D A S   F U N Ç Õ E S   DA  
 *                     A U X I L I A R E S 
 */

/* 
 * randomInteger()
 *
 * A função recebe dois inteiros LOW e HIGH e retorna um 
 * inteiro aleatório entre LOW e HIGH inclusive, ou seja, 
 * no intervalo fechado LOW..HIGH.
 *
 * Pré-condição: a função supõe que 0 <= LOW <= HIGH < INT_MAX.
 *     O codigo foi copiado da página de projeto de algoritmos 
 *     de Paulo Feofiloff, que por sua vez diz ter copiado o 
 *     código da biblioteca random de Eric Roberts.
 */
int 
randomInteger(int low, int high)
{
    int k;
    double d;
    d = (double) rand( ) / ((double) RAND_MAX + 1);
    k = d * (high - low + 1);
    return low + k;
}

/* 
 * pause()
 * 
 * Para a execucao do programa ate que um ENTER seja digitado.
 */
void 
pause()
{
    char ch;

    printf("Digite ENTER para continuar. ");
    do 
    {
        scanf("%c", &ch);
    }
    while (ch != ENTER);
}

/*
 * Função para verificar se a casa de coordenadas x e y (linha e coluna, respectivamente) se encontra dentro do tabuleiro
 */

int valido(int x, int y, int nLin, int nColn){
  if((y <= 0)||(x <= 0)) return FALSE;
  if((x > nLin)||(y > nColn)) return FALSE;
  return TRUE;
}

