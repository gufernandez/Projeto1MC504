CC=gcc
CFLAGS=-std=c11 -Wall -pthread -lncurses
SOURCES=rc.c
EXECUTABLE=rc.x
RM=rm -f

default: all

all: executa

executa: biblioteca compila
	./rc

compila:
	gcc rc.c -o rc interface.o -lcurses -pthread

biblioteca:
	gcc -o interface.o -c interface.c

clear:
	rm rc interface.o
