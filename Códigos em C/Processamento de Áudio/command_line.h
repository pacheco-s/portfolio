// GRR20192786 MATHEUS PACHECO DOS SANTOS 

// Lib feita para definir opções de linha de comando
#ifndef __COMMAND_LINE__
#define __COMMAND_LINE__

typedef struct command_line {
    FILE **inputs;
    FILE *output;

    int num_inputs;
    int argc;

    char **argv;
    char *level;
    char *delay;

} command_line;

int inicializa_options(command_line *lin, int argc, char **argv);
// Define por default, input e output como stdin e stdout, respectivamente
// e num_inputs como 1, e as demais strings como NULL. Retorna 1 se houve 
// sucesso e 0 caso contrário.

void finaliza_options(command_line *lin);
// Fecha arquivos possivelmente abertos e limpa memória que foi alocada
// dinâmicamente.

int def_options(command_line *lin);
// Define opções da linha de comando, dentre elas: -i input, -o ouput
// -l level -t time. No caso de input e output ele abre o arquivo, se houve 
// erro  retorna 0, caso contrário retorna 1. Indicadores sem seus adicionais
// são considerados erros.

int def_diversos_inputs(command_line *lin);
// Função para abrir diversos inputs, retorna 0 se houve erro e 1 caso contrário

#endif