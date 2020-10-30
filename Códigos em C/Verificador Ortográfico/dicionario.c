/* GRR20192786 Matheus Pacheco dos Santos */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dicionario.h"

#define flag 92000


void inicializa_dicionario(vetor_string *dicionario){
    int i, quociente;
    FILE *pont_arq;
    char palavra[SIZE];
    
    pont_arq = fopen("/usr/share/dict/brazilian", "r");

    if(! pont_arq){
        pont_arq = fopen("brazilian", "r");
        if(! pont_arq){
            printf("Erro ao abrir arquivo");
            exit(1);
        }
    }

    dicionario->strs = (string*)malloc(sizeof(string) * (flag + 1)); 
    if(! dicionario->strs){
        printf("Erro de alocação");
        fclose(pont_arq);
        exit(1);
    }
    
    dicionario->strs[0] = (string)malloc(sizeof(string) * SIZE); /* Deixa posição zero pra sentinela. */
    if (! dicionario->strs[0]){
        printf("Erro de alocação");
        free(dicionario->strs);
        dicionario->strs = NULL;
        fclose(pont_arq);
        exit(1);
    } 

    for(i = 1;fgets(palavra, SIZE, pont_arq); i++){
        
        if((i % flag) == 0){
            quociente = i / flag;
            dicionario->strs = (string *)realloc(dicionario->strs,(quociente+1) * flag * sizeof(string));
            
            if(! dicionario->strs){
                printf("Erro de alocação");
                fclose(pont_arq);
                exit(1);
            }
        }    
        palavra[strcspn(palavra, "\n")] = '\0'; /* Substitui '\n' lido por fgets, por '\0' */
        dicionario->strs[i] = strdup(palavra); /* Aloca dinâmicamente */ 
    }
    dicionario->tam = i - 1;

    fclose(pont_arq);
}

void destroi_dicionario(vetor_string *dicionario){
    int i;

    for(i = 0; i < dicionario->tam; i++){
        free(dicionario->strs[i]);
        dicionario->strs[i] = NULL;
    }
    
    free(dicionario->strs); 
    dicionario->strs = NULL;
}

void maiu_minu_nomep(char *caixa_A, char *caixa_b, char *nome_p){
    int i;
    
    for(i = 0; caixa_b[i] != '\0'; i++){
        if(isupper(caixa_b[i])){
            caixa_b[i] = tolower(caixa_b[i]);
            nome_p[i] = tolower(nome_p[i]);
        }
        else caixa_A[i] = toupper(caixa_A[i]);

    }
    nome_p[0] = caixa_A[0];
}

int busca_binaria(vetor_string *dicionario, string palavra){
    int ini, meio, fim, aux;

    ini = 1 ;
    meio = (dicionario->tam) / 2;
    fim = dicionario->tam;
                                    
    aux = strcmp(dicionario->strs[meio], palavra);
    while((aux != 0) && (ini <= fim)){
        if(aux > 0)
            fim = meio - 1;
        else
            ini = meio + 1;
        
        meio = (ini + fim) / 2;
        aux = strcmp(dicionario->strs[meio], palavra);
    }

    if(ini <= fim)
        return 1;
    else
        return 0;
}

int busca_palavra(vetor_string *dicionario, string palavra){
    char caixa_A[SIZE],  nome_p[SIZE];

    /* Dicionario reserva uma sentinela */
    strcpy(dicionario->strs[0], palavra);
    strcpy(caixa_A, palavra);
    strcpy(nome_p, palavra);
    maiu_minu_nomep(caixa_A, dicionario->strs[0], nome_p);

    if(busca_binaria(dicionario, caixa_A) || busca_binaria(dicionario, dicionario->strs[0]) || busca_binaria(dicionario, nome_p))
        return 1;
    else
        return 0;
}

