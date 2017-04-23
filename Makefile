CC=gcc
CFLAGS=-std=c11 -Wall -pthread
SOURCES=rc.c
EXECUTABLE=rc.x
RM=rm -f

default: all

all: executa

executa: compila
	./$(EXECUTABLE)

compila:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clear:
	$(RM) $(EXECUTABLE)