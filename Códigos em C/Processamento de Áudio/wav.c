// GRR20192786 MATHEUS PACHECO DOS SANTOS 

#include <stdio.h>
#include <stdlib.h>
#include "wav.h"

int load_wav (wav *aud, FILE *arq){
    int i;

    i = fread(&aud->riff, 4, 3, arq);   // há 12 bytes no riff
    i += fread(&aud->fmt, 4, 6, arq);   // há 24 bytes no fmt 
    i += fread(&aud->data, 4, 2, arq);  // há 8 bytes para o inicio do data ( O aúdio em si varia)
    if(i != 11) 
        return 0;

    // o block align é o numero de bytes por amostra, e o data_size o número de bytes ocupado pelas amostras!
    // setando variaveis
    aud->samples_per_channel = aud->data.size / aud->fmt.block_align;
    aud->number_of_samples = aud->samples_per_channel * aud->fmt.number_of_channels;
    
    // aloca o tamanho necessario para as amostras;
    aud->data.samples = malloc(sizeof(int16_t) * aud->number_of_samples);
    if(! aud->data.samples)
        return 0;

    i = fread(aud->data.samples, sizeof(int16_t), aud->number_of_samples, arq);  
    if (i != aud->number_of_samples) 
        return 0;

    return 1;
}

void destroi_wav (wav *aud){
    free(aud->data.samples); // libera memoria
    aud->data.samples = NULL;
}

void printwav (wav *aud, FILE* out){
    fprintf(out, "riff tag        (4 bytes): %.4s\n", aud->riff.tag);
    fprintf(out, "riff size       (4 bytes): %d\n", aud->riff.size);
    fprintf(out, "wave tag        (4 bytes): %.4s\n", aud->riff.format);
    
    fprintf(out, "form tag        (4 bytes): %.4s\n", aud->fmt.tag);
    fprintf(out, "fmt_size        (4 bytes): %d\n", aud->fmt.size);
    fprintf(out, "audio_format    (2 bytes): %d\n", aud->fmt.audio_format);
    fprintf(out, "num_channels    (2 bytes): %d\n", aud->fmt.number_of_channels);
    fprintf(out, "sample_rate     (4 bytes): %d\n", aud->fmt.sample_rate);
    fprintf(out, "byte_rate       (4 bytes): %d\n", aud->fmt.byte_rate);
    fprintf(out, "block_align     (2 bytes): %d\n", aud->fmt.block_align);
    fprintf(out, "bits_per_sample (2 bytes): %d\n", aud->fmt.bits_per_sample);

    fprintf(out, "data tag        (4 bytes): %.4s\n", aud->data.tag);
    fprintf(out, "data size       (4 bytes): %d\n", aud->data.size);
    fprintf(out, "bytes per sample         : %d\n", aud->fmt.bits_per_sample/8);
    fprintf(out, "samples per channel      : %d\n", aud->samples_per_channel);
}

int escreve_wav(wav *aud, FILE *out){
    int i;
    
    i = fwrite(&aud->riff, 4, 3, out);   // há 12 bytes no riff
    i += fwrite(&aud->fmt, 4, 6, out);   // há 24 bytes no fmt 
    i += fwrite(&aud->data, 4, 2, out);  // há 8 bytes para o inicio do data ( O aúdio em si varia)
    if(i != 11)
        return 0;

    i = fwrite(aud->data.samples, sizeof(int16_t), aud->number_of_samples, out);
    if(i != aud->number_of_samples)
        return 0;
        
    return 1;
}

int16_t trata_clipping(int sample){
    int16_t aux;

    if(sample > INT16_MAX)
        aux = INT16_MAX;
    else
        if(sample < INT16_MIN)
            aux = INT16_MIN;
        else
            aux = sample;

    return aux;
}

void troca(int16_t *vetor, int i, int j){
    int16_t aux;
    aux = vetor[i];
    vetor[i] = vetor[j];
    vetor[j] = aux;
}

int mix_wavs(wav *aud1, wav *aud2){
    int i, aux, num_soma;

    if(aud2->fmt.number_of_channels != aud1->fmt.number_of_channels)
        return 0;
    
    // trata caso onde o número de samples do aud1 é maior ou igual aud2
    if(aud1->number_of_samples > aud2->number_of_samples)
        num_soma = aud2->number_of_samples;
    else
        num_soma = aud1->number_of_samples;
    for(i = 0; i < num_soma; i++){
        aux = aud1->data.samples[i] + aud2->data.samples[i];
        aud1->data.samples[i] = trata_clipping(aux);
    }

    // trata caso onde o número de samples do aud2 é maior que o aud1
    if(aud1->number_of_samples < aud2->number_of_samples){
        
        // aloca o espaço que falta
        aux = aud1->number_of_samples + aud2->number_of_samples;
        aud1->data.samples = realloc(aud1->data.samples, sizeof(int16_t) * (aux));
        if(! aud1->data.samples)
            return 0;
        
        // atualiza informações
        aud1->riff.size += aud2->riff.size;
        aud1->data.size += aud2->data.size;
        aud1->samples_per_channel += aud2->samples_per_channel;
        for(i = aud1->number_of_samples; i < aud2->number_of_samples; i++){
            aud1->data.samples[i] = aud2->data.samples[i];
        }
        aud1->number_of_samples = aud2->number_of_samples;
    }

    return 1;
} 

int concatena_wavs(wav *aud1, wav *aud2){
    int i, j, aux;

    if(aud2->fmt.number_of_channels != aud1->fmt.number_of_channels)
        return 0;

    // realloca espaço necessário
    aux = aud1->number_of_samples + aud2->number_of_samples;
    aud1->data.samples = realloc(aud1->data.samples, sizeof(int16_t) * aux);
    if(! aud1->data.samples)
        return 0;

    // concatena
    for(i = aud1->number_of_samples, j = 0; i < aux; i++, j++)
        aud1->data.samples[i] = aud2->data.samples[j];

    // atualiza as informações do cabeçalho
    aud1->number_of_samples = aux;
    aud1->riff.size += aud2->riff.size;
    aud1->data.size += aud2->data.size;
    aud1->samples_per_channel += aud2->samples_per_channel;

    return 1;
}

