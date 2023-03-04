TARGET=cot
CC=gcc
CFLAGS=-std=c99 -Wpedantic -Wconversion -Wall -Werror
LDFLAGS=-lm #permite linkar a biblioteca de matemática <math.h>
SRCS=$(wildcard src/*.c)
OBJS=$(addprefix bin/, $(notdir $(SRCS:.c=.o)))
HEADERS=$(wildcard src/*.h)

# compile and clean
.PHONY: all clean

all: ## -> opção geral -> 'make' ou 'make all'
all: CFLAGS+=-O3
all: $(shell mkdir -p bin) $(TARGET)

$(TARGET): $(OBJS) Makefile
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@
	@echo "Compiling is done :]"

bin/%.o: src/%.c $(HEADERS) Makefile
	$(CC) -c $(CFLAGS) -o $@ $<
	
clean: ## -> opção CLEAN para limpar os object files e o executável -> 'make clean'
	rm -f bin/*.o $(TARGET) vgcore*
	@echo "files were cleansed"

debug: ## debug
debug: CFLAGS += -g -Wextra
debug: $(shell mkdir -p bin) $(TARGET)
