// GRR20192786 MATHEUS PACHECO DOS SANTOS 

#include <stdlib.h>
#include <stdio.h>
#include "wav.h" // lib que estrutura arquivos .wav e aloca os samples dinamicamente; algumas funções retornam 0 se houve erro
#include "command_line.h" // lib que estrutura parâmetros da linha de comando; algumas funções retornam 0 se houve erro

int main(int argc, char **argv){
    int i, aux;
    wav aud;
    command_line lin; 
    float level;

    // por default: level 1.0, e, input e output como stdin e stdout
    if(! inicializa_options(&lin, argc, argv)){ 
        perror("Erro de alocação");
        return 0;
    }
    lin.level = "1.0";
    if(! def_options(&lin)){
        perror("A linha de comando está errada ou erro ao abrir um arquivo");
        finaliza_options(&lin);
        return 0;
    }

    if(! load_wav(&aud, lin.inputs[0])) 
        perror("Arquivo corrompido/inválido");
    else {
        level = atof(lin.level); // a conversão acontece, pois a linha de comando é tratada como string
        if((level < 0.0) || (level > 10.0))
            perror("Erro parâmetro -l");
        else{
            for(i = 0; i < aud.number_of_samples; i++){
                aux = aud.data.samples[i] * level;
                aud.data.samples[i] = trata_clipping(aux);
            }

            if(! escreve_wav(&aud, lin.output))
                perror("Erro ao escrever em uma stream");
        }
        destroi_wav(&aud);  // libera memória alocada dinâmicamente
    }

    finaliza_options(&lin); // fecha arquivos que foram carregados, e libera memoria alocada dinâmicamente
    return 1;
}
