// GRR20192786 MATHEUS PACHECO DOS SANTOS 
// Nesta lib, as funções que podem ocorrer erro retorna 0 quando ocorre 1 caso contrário

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "command_line.h"

int inicializa_options(command_line *lin, int argc, char **argv){
    // define os defaults
    lin->inputs = malloc(sizeof(FILE));
    if(! lin->inputs)
        return 0;
    lin->inputs[0] = stdin;
    lin->output = stdout;
    lin->level = NULL;
    lin->delay = NULL;
    lin->argc = argc;
    lin->argv = argv;
    lin->num_inputs = 1;

    return 1;
}

void finaliza_options(command_line *lin){
    int i;

    // fecha arquivos abertos e libera memoria que foi alocada dinamicamente
    if(lin->inputs[0] != stdin)
        fclose(lin->inputs[0]);
    for(i = 1; i < lin->num_inputs ; i++)
            fclose(lin->inputs[i]);
    free(lin->inputs);

    if(lin->output != stdout)
        fclose(lin->output);
}

int def_options(command_line *lin){
    int option;

    while ((option = getopt(lin->argc, lin->argv, ":i:o:l:t:")) != -1){
        switch(option){
            case ':':
            return 0;

            case 'i':
            lin->inputs[0] = fopen(optarg, "r+");
            if(! lin->inputs[0])
                return 0;
            break;

            case 'o':
            lin->output = fopen(optarg, "w");
            if(! lin->output)
                return 0;
            break;

            case 'l':
            lin->level = optarg;
            break;

            case 't':
            lin->delay = optarg;
            break;

            default:
            break;
        }
    }

    return 1;
}

int def_diversos_inputs(command_line *lin){
    int i, j;
    
    lin->num_inputs = lin->argc - optind; // optind, marca aonde começa strings que estão sem indicadores na linha de comando 
    lin->inputs = realloc(lin->inputs, sizeof(FILE) * (lin->num_inputs));
    if(! lin->inputs){
        lin->num_inputs = 1; // volta pro default
        return 0;
    }

    // abre inputs
    for(i = optind, j = 0; i < lin->argc; i++, j++){
        lin->inputs[j] = fopen(lin->argv[i], "r");
        if(! lin->inputs[j]){
            lin->num_inputs = j - 1; // recebe o número de alocações corretas 
            return 0;
        }
    }

    return 1;
} 


