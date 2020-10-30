// GRR20192786 MATHEUS PACHECO DOS SANTOS 

#include <stdlib.h>
#include <stdio.h>
#include "wav.h" // lib que estrutura arquivos .wav e aloca os samples dinamicamente; algumas funções retornam 0 se houve erro
#include "command_line.h" // lib que estrutura parâmetros da linha de comando; algumas funções retornam 0 se houve erro

int main (int argc, char **argv){
    int i;
    wav *auds;
    command_line lin; 

    // por default, input e output são sdtin e stdout
    if(! inicializa_options(&lin, argc, argv)){ 
        perror("Erro de alocação");
        return 0;
    }
    if(! def_options(&lin)){
        perror("A linha de comando está errada ou erro ao abrir um arquivo");
        finaliza_options(&lin);
        return 0;
    }
    if(! def_diversos_inputs(&lin)){  // essa é função especial do command_line, define diversos inputs, não considera stdin como um deles
        perror("Erro de alocação ou de abertura de arquivo");
        finaliza_options(&lin);       
        return 0;                                 
    }  
                                
    if(lin.num_inputs < 2)
        perror("Há apenas 1 ou nenhum input");
    else {
        auds = malloc(sizeof(wav) * lin.num_inputs);
        if(! auds){
            perror("Erro de alocação");
            finaliza_options(&lin);
            return 0;
        }

        if(! load_wav(&auds[0], lin.inputs[0])){  // carrega primeiro arquivo da concatenação
            perror("Há arquivo corrompido/inválido");
            free(auds);
            auds = NULL;
            finaliza_options(&lin);
            return 0;
        }
        
        // concatena wavs
        for(i = 1; i < (lin.num_inputs) ; i++){
            if(! load_wav(&auds[i], lin.inputs[i])){
                perror("Há arquivo corrompido/inválido");
                destroi_wav(&auds[i - 1]);
                finaliza_options(&lin);
                free(auds);
                auds = NULL;
                return 0;
            }
            else{
                if(! concatena_wavs(&auds[i - 1], &auds[i])){ // esta função concatena o 2° elemento no 1° 
                    perror("Erro de alocação ou os audios tem números de canais distintos");
                    destroi_wav(&auds[i - 1]);
                    destroi_wav(&auds[i]);
                    finaliza_options(&lin);
                    free(auds);
                    auds = NULL;
                    return 0;
                } 
                destroi_wav(&auds[i]); // libera memoria alocada dinamicamente
                auds[i] = auds[i - 1]; 
            }
        }
        
        if(! escreve_wav(&auds[lin.num_inputs - 1], lin.output))
            perror("Erro ao escrever em uma stream");
        
        // libera memorias alocadas 
        destroi_wav(&auds[lin.num_inputs - 1]);
        free(auds);
        auds = NULL;     
    }  
    
    finaliza_options(&lin); // fecha arquivos que foram carregados, e libera memoria alocada dinâmicamente
    return 1;
}

