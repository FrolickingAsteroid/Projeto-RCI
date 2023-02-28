TARGET=cot
CC=gcc
CFLAGS=-std=c99 -Wpedantic -Wconversion -Wall -Werror
LDFLAGS=-lm #permite linkar a biblioteca de matemática <math.h>
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=*.o)
HEADERS=$(wildcard *.h)

# compile and clean
.PHONY: all clean

all: ## -> opção geral -> 'make' ou 'make all'
all: CFLAGS+=-O3
all: $(TARGET)

$(TARGET): $(OBJS) Makefile
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@
	@echo "Compiling is done :]"

%.o: %.c $(HEADERS) Makefile
	$(CC) -c $(CFLAGS) -o $@ $<
	
clean: ## -> opção CLEAN para limpar os object files e o executável -> 'make clean'
	rm -f *.o $(TARGET)
	@echo "files were cleansed"
