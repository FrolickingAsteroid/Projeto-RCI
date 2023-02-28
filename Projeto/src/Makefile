# project name (gera executável com este nome)
Target=cot
CC=gcc

# flags de compilação
CFLAGS=-std=c99 -Wpedantic -Wconversion -Wall -Werror
LDFLAGS=-lm #permite linkar a biblioteca de matemática <math.h>

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=*.o)
HEADER=$(wildcard *.h)

# compile and clean
.PHONY: all clean

all: ## -> opção geral -> 'make' ou 'make all'
		CFLAGS+= -O3
		$(TARGET)

$(TARGET): $(OBJS) Makefile
		$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c $(HEADERS) Makefile
		$(CC) -c $(CFLAGS) -o $@ $<
		echo "Compiling complete :]"

clean: ## -> opção CLEAN para limpar os object files e o executável -> 'make clean'
		rm -f *.o $(TARGET)
		echo "Cleaned Executable :]"
