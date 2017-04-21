#include <curses.h> //Incluiremos a biblioteca ao nosso sistema
#include <stdio.h>
#include <unistd.h>

#define SCREEN_SIZE 134
#define L1 9
#define EMB 85
#define DES 36

struct Carrinho{
   int id;
   int passageiros;
};

void printCar(int coluna, struct Carrinho carrinho);
void printRail();
void printTitulo();

int main(){
  int i;
  struct Carrinho car1,car2;
  car1.passageiros = 5;
  car1.id = 1;
  car2.passageiros = 6;
  car2.id = 2;

  noecho();
  curs_set(0);

  initscr();   /*Esta função  inicializa a ncurses. Para todos os programas
                 devemos sempre inicializar a ncurses e depois finalizar, como
                veremos adiante. */
  printTitulo();
  printRail();
  refresh();
  sleep(1);
  for (i=0;i<50;i++){
    printRail();
    printCar(EMB+i,car1);
    printCar(DES+i,car2);
    //sleep(1);
    usleep(100000);

    refresh();    //Atualiza a tela
  }
  getch();      //Fica esperando que o usuário aperte alguma tecla

  endwin();
  return 0;
}
void printTitulo(){
  int i;
  move(0,0);
  for (i = 0; i < SCREEN_SIZE; i++){
    printw("=");
  }
  move(2,60);  //Aqui estamos movendo o cursor para a linha 2 coluna 1.
  printw("MONTANHA RUSSA");

  move(4,0);
  for (i = 0; i < SCREEN_SIZE; i++){
    printw("=");
  }
  move(6,DES);
  printw("*DESEMBARQUE*");
  move(6,EMB);
  printw("*EMBARQUE*");
  move(0,0);
}
void printRail(){
  int i;

  move(L1-1,0);
  for (i = 0; i < SCREEN_SIZE; i++){
    printw(" ");
  }
  move(L1,0);
  for (i = 0; i < SCREEN_SIZE; i++){
    printw("-");
  }
  move(L1+1,0);
  for (i = 0; i < SCREEN_SIZE; i++){
    printw("-");
  }
  move(L1+2,0);
  for (i = 0; i < SCREEN_SIZE; i++){
    printw(" ");
  }
  move(0,0);
}
void printCar(int coluna, struct Carrinho carrinho){
  int largura = carrinho.passageiros + carrinho.passageiros%2;
  int i;

  move(L1-1, coluna);
  /*-----------FRENTE-----------*/
  if (getcury(stdscr) == L1-1)
    printw(" ");

  for (i=0; i<largura;i++){
    //printw("%d %d",getcury(stdscr), getcurx(stdscr));
    if ((getcury(stdscr) == L1-1))
      printw("_");
  }

  if ((getcury(stdscr) == L1-1))
    printw("%d", carrinho.id);

  move(L1, coluna);
  /*-----------Fileira 1-----------*/
  if (getcury(stdscr) == L1)
    printw("|");

  for (i=0; i<largura;i++){
    if (getcury(stdscr) == L1){
      if (i % 2 == 0){
        printw("o");
      }else
        printw(" ");
    }
  }

  if (getcury(stdscr) == L1)
    printw(")");

  move(L1+1, coluna);
  /*-----------Fileira 2-----------*/
  if (getcury(stdscr) == L1+1)
    printw("|");

  for (i=0; i<largura;i++){
    if (getcury(stdscr) == L1+1){
      if ((i == 0) && (carrinho.passageiros%2 == 1)){
        printw(" ");
      }
      else if (i % 2 == 0){
        printw("o");
      }else
        printw("_");
    }
  }

  if (getcury(stdscr) == L1+1)
      printw(")");

  move(0,0);
}
