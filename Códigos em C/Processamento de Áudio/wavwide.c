// GRR20192786 MATHEUS PACHECO DOS SANTOS 

#include <stdlib.h>
#include <stdio.h>
#include "wav.h" // lib que estrutura arquivos .wav e aloca os samples dinamicamente; algumas funções retornam 0 se houve erro
#include "command_line.h" // lib que estrutura parâmetros da linha de comando; algumas funções retornam 0 se houve erro

int main (int argc, char **argv){
    int i;
    int16_t diff;
    float level;
    wav aud;
    command_line lin;

    // por default: level 1.0, e, input e output, como stdin e stdout 
    if(! inicializa_options(&lin, argc, argv)) 
        return 0;
    lin.level = "1.0";
    if(! def_options(&lin)){
        perror("A linha de comando está errada ou erro ao abrir um arquivo");
        finaliza_options(&lin);
        return 0;
    }
    
    if(! load_wav(&aud, lin.inputs[0])) // usamos somente 1 input nesse programa
        perror("Arquivo corrompido/inválido");
    else{
        if(aud.fmt.number_of_channels == 2){
            
            level = atof(lin.level); // a conversão acontece pois a linha de comando é tratada como string
            if(level > 10 || level < 0)
                perror("Erro no parâmetro -l");
            else{
                // esse bloco, faz com que os canais fiquem mais abertos
                for(i = 0; i < (aud.number_of_samples - 1); i += 2){
                    diff = aud.data.samples[i + 1] - aud.data.samples[i];
                    aud.data.samples[i] = aud.data.samples[i] - level*diff;
                    aud.data.samples[i + 1] = aud.data.samples[i + 1] + level * diff;
                }

                if(! escreve_wav(&aud, lin.output))
                    perror("Erro ao escrever em uma stream");
            }
        }
        else
            perror("O filtro só funciona em estéreo de 2 canais");

        destroi_wav(&aud); // libera memória alocada dinamicamente para os samples
    }

    finaliza_options(&lin); // fecha arquivos que foram carregados, e libera memoria alocada dinâmicamente
    return 1;
}