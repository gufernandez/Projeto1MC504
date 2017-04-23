#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#define LE 9
#define LD 18
#define EMB getmaxx(stdscr)/2-5
#define DES getmaxx(stdscr)/2-6

struct Carrinho{
   int *passageiros;
   int boarding;
   int unboarding;
};

typedef struct Carrinho carrinho;

void printCar(int linha, int coluna, int largura, carrinho *car);
void printRail();
void printTitulo();

#endif
