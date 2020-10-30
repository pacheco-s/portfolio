// GRR20192786 MATHEUS PACHECO DOS SANTOS 

#include <stdlib.h>
#include <stdio.h>
#include "wav.h" // lib que estrutura arquivos .wav e aloca os samples dinamicamente; algumas funções retornam 0 se houve erro
#include "command_line.h" // lib que estrutura parâmetros da linha de comando; algumas funções retornam 0 se houve erro

int main (int argc, char **argv){
    int i, j, k, i_ultimo_bloco;
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

    if(! load_wav(&aud, lin.inputs[0])) // usamos somente 1 input nesse programa
        perror("Arquivo corrompido/inválido");
    else{
        // A lógica usada foi a seguinte, considere 2 canais e a sequencia
        // 01 23 45 67, queremos 67 45 23 01, avançando de 2 em 2 canais podemos
        // usar como indices para fazer a troca o indice do bloco atual e, a 
        // formula para o outro indice: o valor absoluto da difereça do  indice 
        // do bloco atual e indice do inicio do ultimo bloco,  por exemplo 
        // abs(0 - 6) = 6, abs(2 - 6) = 4, agr podemos fazer a troca dos blocos

        // i_ultimo_bloco é o indice do primeiro canal no ultimo sample por canal
        i_ultimo_bloco = aud.number_of_samples - aud.fmt.number_of_channels;
    
        for(i = 0; i < aud.samples_per_channel; i += aud.fmt.number_of_channels){
            j = abs(i - i_ultimo_bloco);
            for(k = 0;  k < aud.fmt.number_of_channels; k++, j++) // contador k usado para trocar os canais
                troca(aud.data.samples, i + k, j);               
        }

        if(! escreve_wav(&aud, lin.output))
            perror("Erro ao escrever em uma stream");
        destroi_wav(&aud);         // libera memória alocada dinamicamente
    }

    finaliza_options(&lin);    // fecha arquivos que foram abertos
    return 1;
}