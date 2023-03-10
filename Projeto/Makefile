TARGET=cot
CC=gcc
CFLAGS=-std=gnu99 -Wpedantic -Wconversion -Wall -Werror
LDFLAGS=-lm #permite linkar a biblioteca de matemática <math.h>
SRCS=$(wildcard src/*.c) $(wildcard src/*/*.c)
OBJS=$(addprefix bin/, $(notdir $(SRCS:.c=.o)))
HEADERS=$(wildcard src/*/*.h)

# Reset
Color_Off=\033[0m
# Regular Colors
Red=\033[0;31m
Green=\033[0;32m

SRC_DIRS := $(sort $(dir $(SRCS)))

VPATH := $(SRC_DIRS)

# compile and clean
.PHONY: all clean

all: ## -> opção geral -> 'make' ou 'make all'
all: CFLAGS+=-O3
all: $(shell mkdir -p bin) $(TARGET)

$(TARGET): $(OBJS) Makefile
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@
	@echo -e "${Green}🗹 Finished compiling with success${Color_Off}"

bin/%.o: %.c $(HEADERS) Makefile
	$(CC) -c $(CFLAGS) -o $@ $<
	
clean: ## -> opção CLEAN para limpar os object files e o executável -> 'make clean'
	rm -f bin/*.o $(TARGET) vgcore*
	@echo -e "${Red}🗹 Executables were cleansed${Color_Off}"

debug: ## debug
debug: CFLAGS += -g -Wextra
debug: $(shell mkdir -p bin) $(TARGET)
