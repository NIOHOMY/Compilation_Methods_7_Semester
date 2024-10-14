#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <conio.h>

char S[100];
int i;
int ST;
int CL;
int FIX;
int D[5][5] = {
    //         '1'  '2'  '3'
    { -2,        1,   2,   3,  -1 }, // S0
    { -2,        1,   2,   3,  -1 }, // S1
    { -2,       -1,   2,   3,  -1 }, // S2
    { -2,       -1,  -1,   3,  -1 }  // S3
};

int W[] = { 0, 1 };

char* out[] = {
    "Error",     // error
    "Final",     // fin
};

int sclass(char c) {
    if (c == '\0') return 0;
    if (c == '1')  return 1;
    if (c == '2')  return 2;
    if (c == '3')  return 3;
    return 4; // Error case | other characters
}

void lexfile() {
    char name[30];
    FILE* inp;
    do {
        printf("file name:");
        gets(name);
    } while ((inp = fopen(name, "r")) == NULL);

    S[0] = '\0';
    i = 0;
    ST = 0;

    while (1) {
        if ((ST==0) && (S[i] == '\0')) {
            if (fgets(S, 100, inp) == NULL) {
                printf("The end\n");
                return;
            }
            i = 0;
        } 
        if (ST == 0) FIX = i;
        
        CL = sclass(S[i]);
        printf("%c[%d->", S[i], ST);
        ST = D[ST][CL];
        printf("%d]\n", ST);
        
        i++;
    
        if (ST < 0)
		{
    		int j;
    		i=i-W[-ST-1];
    		printf("(%d)<- %s ",W[-ST-1],out[-ST-1]);
    		for (j=FIX; j<i; j++)
    			putchar(S[j]);
    		puts("");
    		ST=0;
    		S[0] = '\0';
    		i=0;
		}
    }
}

int main() {
    lexfile();
    getch();
    return 0;
}