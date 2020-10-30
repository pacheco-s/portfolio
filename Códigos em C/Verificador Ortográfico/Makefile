# GRR20192786 Matheus Pacheco dos Santos

CFLAGS = -Wall -g # Flags de compilação
LDLIBS = -lm      # Bibliotecas à ligar

objects = ortografia.o dicionario.o

# Regra default
all: ortografia 

# Regras de ligação
ortografia: $(objects)

# Regras de compilação
ortografia.o: ortografia.c dicionario.h
dicionario.o: dicionario.c dicionario.h

# Remove arquivos temporários
clean: 
	-rm -f ortografia $(objects)

# Remove tudo o que não for o código-fonte
purge: clean
	-rm -f ortografia
