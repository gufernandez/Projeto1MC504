#include <curses.h>
#include <stdio.h>
#include "interface.h"
/*Índice da curses:
    y,x: coordenadas de uma posição na tela (y -> linha e x -> coluna)
    getmaxx: pega o maior número de x, ou seja, quantas colunas há na tela (largura), o mesmo vale pra y
    initscr: começa uma tela da curses (a tela stdscr)
    refresh: aplica as mudanças na tela
    printw: printf da curses
    getcurx: pega a posição x atual do cursor, o mesmo vale pra y
    move(y,x): move o cursor para a posição y,x
*/

#define LE 9
#define LD 18
#define EMB getmaxx(stdscr)/2-5
#define DES getmaxx(stdscr)/2-6

void printTitulo(){
  int i;

  move(0,0);
  for (i = 0; i < getmaxx(stdscr); i++){
    printw("=");
  }
  move(2,getmaxx(stdscr)/2-7);  //Aqui estamos movendo o cursor para a linha 2 coluna 1.
  printw("MONTANHA RUSSA");

  move(4,0);
  for (i = 0; i < getmaxx(stdscr); i++){
    printw("=");
  }
  move(LD-3,DES);
  printw("*DESEMBARQUE*");
  move(LE-3,EMB);
  printw("*EMBARQUE*");
  move(0,0);

}

void printRail(){
  int i;

  /*LE*/
  move(LE-1,0);
  for (i = 0; i < getmaxx(stdscr); i++){
    printw(" ");
  }
  move(LE,0);
  for (i = 0; i < getmaxx(stdscr); i++){
    printw("-");
  }
  move(LE+1,0);
  for (i = 0; i < getmaxx(stdscr); i++){
    printw("-");
  }
  move(LE+2,0);
  for (i = 0; i < getmaxx(stdscr); i++){
    printw(" ");
  }
  /*LD*/
  move(LD-1,0);
  for (i = 0; i < getmaxx(stdscr); i++){
    printw(" ");
  }
  move(LD,0);
  for (i = 0; i < getmaxx(stdscr); i++){
    printw("-");
  }
  move(LD+1,0);
  for (i = 0; i < getmaxx(stdscr); i++){
    printw("-");
  }
  move(LD+2,0);
  for (i = 0; i < getmaxx(stdscr); i++){
    printw(" ");
  }
  move(0,0);

}

void printCar(int linha, int coluna, int largura, carrinho *car){
  int i = 0;
  int passageiros = 0;
  int j = 0;
  int curX = coluna+1;
  int max = getmaxx(stdscr);

  //FIX DE BUGS
  move(LE+2,0);
  for (i = 0; i < getmaxx(stdscr); i++){
    printw(" ");
  }
  move(LE,0);
  printw("-");
  move(LE+1,0);
  printw("-");
  //FIM DO FIX

  while(car->passageiros[i++] >= 0){
    passageiros++;
  }

  move(linha-1, --curX);
  /*-----------FRENTE-----------*/
  if ((curX <= max) && (curX >= 0))
    printw(" ");

  for (i=0; i<largura;i++){
    //printw("%d %d",getcury(stdscr), getcurx(stdscr));
    move(linha-1, --curX);
    if ((curX <= max) && (curX >= 0))
      printw("_");
  }
  move(linha-1,--curX);
  if ((curX <= max) && (curX >= 0))
    printw(" ");

  move(linha-1,--curX);
  if ((curX <= max) && (curX >= 0))
    printw(" ");

  curX = coluna+1;
  move(linha, --curX);
  /*-----------Fileira 1-----------*/
  if ((curX <= max) && (curX >= 0))
    printw(")");

  for (i=0; i<largura;i++){
    move(linha,--curX);

    if ((curX <= max) && (curX >= 0)){
      if ((i % 2 == 1) && (j<=passageiros) && (car->passageiros[j]>=0)){
        printw("%d", car->passageiros[j++]);
      }else
        printw(" ");
    }
  }
  move(linha,--curX);
  if ((curX <= max) && (curX >= 0))
    printw("|");

  move(linha,--curX);
  if ((curX <= max) && (curX >= 0))
    printw("-");

  curX = coluna+1;
  move(linha+1, --curX);
  /*-----------Fileira 2-----------*/
  if ((curX <= max) && (curX >= 0))
    printw(")");

  for (i=0; i<largura;i++){
    move(linha+1,--curX);
    if ((curX <= max) && (curX >= 0)){
      if ((i % 2 == 1) && (j<=passageiros) && (car->passageiros[j]>=0)){
        printw("%d",car->passageiros[j++]);
      }else
        printw("_");
    }
  }

  move(linha+1,--curX);
  if ((curX <= max) && (curX >= 0))
      printw("|");

  move(linha+1,--curX);
  if ((curX <= max) && (curX >= 0))
    printw("-");

}
