#include<stdio.h>
#include<string.h>


int 
leiaString(char str[], int size)
{
    int len;
    char s[256];

    /* remove qualquer newline que estava no buffer */
    scanf(" ");

    /* leitura do string: ler info sobre fgets() */
    fgets(s, 256, stdin);

    /* sobreescreve  um possivel newline do final com '\0' */
    len = strlen(s); 
    if (s[len-1] == '\n') 
    {
        len--;
        s[len] = '\0';
    }

    if (len > size)
    {
        s[size-1] = '\0';
        len = size-1;
    }
    strncpy(str, s, size);

    return len;
}

int
achePalavra(unsigned char *pal, int tPal, unsigned char *texto, int tTex)
{
    int ult[256];
    int i, r, k;
    for(i = 0; i < 256; i++) ult[i] = 0;
    for(i = 0; i < tPal; i++) ult[pal[i]] = i+1;

    k = tPal;
    while(k <= tTex){
        r = 0;
        while(r < tPal && (pal[tPal-r-1] - texto[k-r-1] == 0)) r++;
        if(r == tPal) return 1;
        if(k == tTex) k++;
        else k += tPal - ult[texto[k+1]] + 1; 
    }
    return 0;
}

int main()
{
    char *text;
    char *p;
    int a, b;
    a = 0; b = 0;
    a = leiaString(p, 256);
    printf("%d\n", a);
    text = "hiroshi matou um gato";
    printf("resposta: %d\n", achePalavra(p, a, text, strlen(text)));
    return 0;

}

