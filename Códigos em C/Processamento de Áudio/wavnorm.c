// GRR20192786 MATHEUS PACHECO DOS SANTOS 

#include <stdlib.h>
#include <stdio.h>
#include "wav.h" // lib que estrutura arquivos .wav e aloca os samples dinamicamente; algumas funções retornam 0 se houve erro
#include "command_line.h" // lib que estrutura parâmetros da linha de comando; algumas funções retornam 0 se houve erro

int main (int argc, char **argv){
    int16_t maior;
    int i;
    float norma;
    wav aud;
    command_line lin; 

    // por default: input e output como stdin e stdout respectivamente
    if(! inicializa_options(&lin, argc, argv)){ 
        perror("Erro de alocação");
        return 0;
    }
    if(! def_options(&lin)){
        perror("A linha de comando está errada ou erro ao abrir um arquivo");
        finaliza_options(&lin);
        return 0;
    }

    if(! load_wav(&aud, lin.inputs[0])){  // usamos somente 1 input nesse programa
        perror("Arquivo comrrompido/inválido");
    }
    else{
        // acho maior valor nas amostras 
        maior = aud.data.samples[0];
        for(i = 1; i < aud.number_of_samples; i++)
            if(maior < aud.data.samples[i])
                maior = aud.data.samples[i];

        // a norma é definida de forma que para que a normalização do maior valor seja o 
        // valor máximo, preservando a proporção e não ocasionando overflow. Usamos float por precisão
        norma = (float)INT16_MAX / maior; 

        for(i = 0; i < aud.number_of_samples; i++)
            aud.data.samples[i] = trata_clipping(aud.data.samples[i] * norma);
        

        if(! escreve_wav(&aud, lin.output)) 
            perror("Erro de escrita na stream");
        destroi_wav(&aud);     // libera memória alocada dinamicamente
    }

    finaliza_options(&lin);    // fecha arquivos que foram abertos
    return 1 ;  
}
