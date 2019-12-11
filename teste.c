#include<stdio.h>
#include<string.h>

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
        while(r < tPal && (pal[tPal-r-1] - texto[k-r] == 0)) r++;
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
    a = 0;
    scanf("%s", &p);
    scanf("%s", &text);
    for(b = 0; p[b] != '\0'; b++);
    printf("%d\n", b);
    /*printf("resposta: %d\n", achePalavra(p, strlen(p), text, strlen(text)));*/
    return 0;

}

