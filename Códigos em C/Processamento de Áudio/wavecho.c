// GRR20192786 MATHEUS PACHECO DOS SANTOS 

#include <stdlib.h>
#include <stdio.h>
#include "wav.h" // lib que estrutura arquivos .wav e aloca os samples dinamicamente; algumas funções retornam 0 se houve erro
#include "command_line.h" // lib que estrutura parâmetros da linha de comando; algumas funções retornam 0 se houve erro

int main (int argc, char **argv){
    wav aud;
    float level;
    int delay, i, i_delay;
    command_line lin; 

    // por default: level 0.5, delay 1000ms, e, input e output como stdin e stdout respectivamente
    if(! inicializa_options(&lin, argc, argv)){ 
        perror("Erro de alocação");
        return 0;
    }
    lin.level = "0.5";
    lin.delay = "1000.0";
    if(! def_options(&lin)){
        perror("A linha de comando está errada ou erro ao abrir um arquivo");
        finaliza_options(&lin);
        return 0;
    }

    if(! load_wav(&aud, lin.inputs[0])) // usamos somente 1 input nesse programa
        perror("Arquivo corrompido/inválido");
    else {
        level = atof(lin.level);  // a conversão acontece, pois a linha de comando é tratada com strings
        delay = atoi(lin.delay);

        // trata as condições do level e delay
        if((level < 0) || (level > 1) || (delay < 0))
            perror("Erro nos paramentros -l e -t");
        else{
            // o indice delay é calculado dessa forma pois sample rate é taxa de amostragem
            // (amostras/seg), por canal 
            i_delay = delay * 0.001 * aud.fmt.sample_rate * aud.fmt.number_of_channels;

            for(i = i_delay; i < aud.number_of_samples; i++)
                aud.data.samples[i] +=  level*aud.data.samples[i - i_delay];

            if(! escreve_wav(&aud, lin.output))
                perror("Erro ao escrever em uma stream");
        } 
        destroi_wav(&aud); // fecha arquivos que foram carregados, e libera memória alocada dinâmicamente
    }
    
    finaliza_options(&lin);  // fecha arquivos que foram abertos
    return 1;
}