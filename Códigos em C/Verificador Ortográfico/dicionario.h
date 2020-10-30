/* GRR20192786 Matheus Pacheco dos Santos */

#ifndef __DICIONARIO__
#define __DICIONARIO__

#define SIZE 50

typedef  char *string;

typedef struct vetor_string
{
    string *strs;
    int tam;
} vetor_string;

void inicializa_dicionario (vetor_string *dicionario);
/* Carrega dicionário na memória RAM, usando o realloc. Com um  *
 * flag de 92000.                                               */

void maiu_minu_nomep(char *caixa_A, char *caixa_b, char *nome_p);
/* Converte caixa_A para caixa alta, nome_p para nome próprio e *
 * converte caixa_b para caixa baixa.                           */

int busca_binaria(vetor_string *dicionario, string palavra);
/* Algortimo clássico de busca binária, retorna 1 se achou, 0   *
 * caso contrário.                                              */

int busca_palavra(vetor_string *dicionario, string palavra);
/* Busca a string palavra no dicinonário,  usando  o  algoritmo *
 * clássico de busca binária, retorna 1 se encontrar e, 0 caso  *
 * contrário. OBS.: Usa os três casos, caixa alta, caixa baixa  *
 * nome próprio.                                                */

void destroi_dicionario(vetor_string *dicionario);
/* Função para desalocar as memórias alocadas dinâmicamete.     */

#endif