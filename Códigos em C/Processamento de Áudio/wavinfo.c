// GRR20192786 MATHEUS PACHECO DOS SANTOS 

#include <stdlib.h>
#include <stdio.h>
#include "wav.h" // lib que estrutura arquivos .wav e aloca os samples dinamicamente; algumas funções retornam 0 se houve erro
#include "command_line.h" // lib que estrutura parâmetros da linha de comando; algumas funções retornam 0 se houve erro

int main (int argc, char **argv){
    wav audio;
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

    if(! load_wav(&audio, lin.inputs[0])) // usamos somente 1 input nesse programa
        perror("Arquivo corrompido ou inválido");
    else 
        printwav(&audio, lin.output); // printa as informações do wav no stream indicado por output
    
    finaliza_options(&lin);   // fecha arquivos que foram abertos
    destroi_wav(&audio);      // libera memória alocada dinamicamente
    return 1;
}





