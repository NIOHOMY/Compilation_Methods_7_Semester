#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <conio.h>

char S[100];
int i;
int ST;
int CL;
int FIX;
int D[7][5]=
{ //      '  " 09 az ---
        { 3, 5, 1, 2, 0 }, //S0
        {-1,-1, 1, 1,-1 }, //S1
        {-2,-2, 2, 2,-2 }, //S2
        {-4, 4, 3, 3, 3 }, //S3
        { 3,-3,-3,-3,-3 }, //S4
        { 6,-6, 5, 5, 5 }, //S5
        {-5, 5,-5,-5,-5 }, //S6
	};

int     W[]={ 1,1,1,0,1,0 };

char    *out[]={
		"0..9 A..F :",	// fin-1
		"IDENT",		// fin-2
		"Error",		// fin-3
		"String 1",		// fin-4
		"Error",		// fin-5
		"String 2" };	// fin-6

int sclass(char c)
{
switch (c)
	{
        case    39:    return(0);
        case    '"':   return(1);
        default: 
        	{
            	if(isdigit(c)) return 2;
            	if(isalpha(c)) return 3;
            	return 4;
        	}
	}
}


void lexfile()
{
    char name[30];
    FILE *inp;
    do
    	{
    	printf("file name:");
    	gets(name);
    	}
    	while ((inp=fopen(name,"r"))==NULL);
    
    S[0]='\0';
    i=0;
    ST=0;
    
    while(1)
    	{
    
    	if ((ST==0) && (S[i]=='\0'))
    		{
    		if (fgets(S,100,inp)==NULL)
    			{
    			printf("The end\n");
    			return;
    			}
    		i=0;
    		}
    
    	if (ST==0) FIX=i;
    
    	CL=sclass(S[i]);
    	printf("%c[%d->",S[i],ST);
    	ST=D[ST][CL];
    	printf("%d]\n",ST);
    	i++;
    
    	if (ST  < 0)
    		{
    		int j;
    		i=i-W[-ST-1];
    		printf("(%d)<- %s ",W[-ST-1],out[-ST-1]);
    		for (j=FIX; j<i; j++)
    			putchar(S[j]);
    		puts("");
    		ST=0;
    		}
    	}

}



int main()
{
    lexfile();
    getch();
    return 0;
}