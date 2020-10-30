/* GRR20192786 Matheus Pacheco dos Santos */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#include "dicionario.h"

int main ()
{
    vetor_string dicionario; 
    /* O limite de caracteres foi definido no dicionario.h (50) */
    char palavra[SIZE], c, *locale; 
    int i;

    locale = setlocale (LC_CTYPE, "pt_BR.ISO-8859-1");
    if(locale == NULL){
        printf("Erro locale");
        exit(0);
    }

    inicializa_dicionario(&dicionario);
    while(scanf("%c", &c) != EOF)
    {    
        /* Escreve até achar uma letra, ou fim do arquivo. */
        while(!isalpha(c) && !feof(stdin))
        {
            printf("%c", c);
            scanf("%c", &c);
        }
        
        /* Quando acha a primeira letra, acha as demais. */
        for(i = 0;isalpha(c) && !feof(stdin) ; i++)
        {
            palavra[i] = c;
            scanf("%c", &c);
        }
        palavra[i] = '\0';

        if(!feof(stdin)){
            if(busca_palavra(&dicionario, palavra))
                printf("%s%c", palavra, c);
            else 
                printf("[%s]%c", palavra, c);
        }
    }
    destroi_dicionario(&dicionario); /* Libera memória */
    return 0;
}


 