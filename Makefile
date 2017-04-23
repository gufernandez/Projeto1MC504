all: executa

executa: biblioteca compila
	./rc

compila:
	gcc rc.c -o rc interface.o -lcurses -pthread

biblioteca:
	gcc -o interface.o -c interface.c

clear:
	rm rc interface.o
