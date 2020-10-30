// GRR20192786 MATHEUS PACHECO DOS SANTOS 
// lib feita com intuito de estrutrar e manipular um arquivo .wav

#ifndef __WAV__
#define __WAV__

#include <inttypes.h>

typedef struct chunck_1 {
    char tag[4];                  // constante, identifica o tipo de arquivo 
    uint32_t size;                // tamanho do arquivo em bytes, sem considerar chunk_id e chunk_size 
    char format[4];               // constante, define o formato do restante do conteúdo
} chunck_1;

typedef struct chunck_2 {
    char tag[4];	              // constante, cabeçalho do chunk 
    uint32_t size;                // tamanho deste chunk
    uint16_t audio_format;        // codificação utilizada
    uint16_t number_of_channels;  // número de canais de áudio
    uint32_t sample_rate;         // Taxa de amostragem (amostras/seg)
    uint32_t byte_rate;           // taxa de bytes por segundo
    uint16_t block_align;         // número de bytes por amostra (soma todos os canais)
    uint16_t bits_per_sample;     // bits por amostra, por canal 
} chunck_2;

typedef struct chunck_3 {
    char tag[4];                  // constante, cabeçalho do chunk
    uint32_t size;                // espaço ocupado pelas amostras, em bytes
    int16_t  *samples;            // amostras de áudio 
} chunck_3;


typedef struct wav {       
    chunck_1 riff;                // cabeçalho, Resource Interchange File Format
    chunck_2 fmt;                 // informações sobre o formato do áudio
    chunck_3 data;                // amostras do(s) canal(is) de áudio 
    int samples_per_channel;      // numero de amostras por canal
    int number_of_samples;        // numero de amostras
} wav;


int load_wav (wav *audio, FILE *arq);
// Recebe um arquivo do .wav codificação PCM 16 bits com sinal, e armazena seus 
// respectivos chunks na estrutura de dados, e tambem guarda valores úteis como
// samples per channel. Retorna 1 se nao houve erro e, 0 caso contrário.

void destroi_wav (wav *aud);
// Libera memoria alocada dinamicamenta para as amostras de audio.

void printwav (wav *aud, FILE* out);
// Printa as informações do wav no stream indicado por out 

int escreve_wav(wav *aud, FILE *out);
// Escreve chunks do wav no stream indicado por out, se houve sucesso retorna 1, 
// caso contrário retorna 0

int16_t trata_clipping(int sample);
// Verifica se acontece clipping no sample indicado, se for cliping negativo retorna 
// o INT16_MIN, se for clipping positivo retorna INT16_MAX, por fim se não ocorrer
// cliping retorna o próprio sample. 

int concatena_wavs(wav *aud1, wav *aud2);
// Concatena aud2 no aud1. Retorna 1 se houve sucesso e 0 caso contrário.

int mix_wavs(wav *aud1, wav *aud2);
// Soma asmotras do aud1 e aud2 e armazena no aud1; cuida do caso onde o número de amostras
// do aud1 e aud2 é difernete. Retorna 1 se houve sucessso e 0 caso contrário.

void troca(int16_t *vetor, int i, int j);
// Troca os valores do vetor no indice i e j, útil para reverter canais.

#endif
