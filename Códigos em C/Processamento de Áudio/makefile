# GRR20192786 MATHEUS PACHECO DOS SANTOS 

CFLAGS = -Wall -Wextra # gerar "warnings" detalhados e infos de depuração
LDLIBS = -lm

objs = wav.o command_line.o
 
CODIGOS= wavcat wavecho wavinfo wavmix wavnorm  wavrev wavvol wavwide

# regra default (primeira regra)
all: $(CODIGOS)
 
# regras de ligacao
wavcat: $(objs) wavcat.o
wavecho: $(objs) wavecho.o
wavinfo: $(objs) wavinfo.o
wavmix: $(objs) wavmix.o
wavnorm: $(objs) wavnorm.o
wavrev: $(objs) wavrev.o
wavvol: $(objs) wavvol.o
wavwide: $(objs) wavwide.o
 
# regras de compilação
command_line.o:   command_line.c command_line.h
wav.o: wav.c wav.h
 
# remove arquivos temporários
clean: 
	-rm -f *~ *.o

# remove tudo o que não for o código-fonte
purge: clean
	-rm -f $(CODIGOS)

