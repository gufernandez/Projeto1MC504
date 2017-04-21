/*The roller coaster problem

O que o nosso programa deve satisfazer:

• Passengers should invoke board and unboard.
• The car should invoke load, run and unload.
• Passengers cannot board until the car has invoked load
• The car cannot depart until C passengers have boarded.
• Passengers cannot unboard until the car has invoked unload.
• Only one car can be boarding at a time.
• Multiple cars can be on the track concurrently.
• Since cars can’t pass each other, they have to unload in the same order they
boarded.
• All the threads from one carload must disembark before any of the threads from
subsequent carloads.*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define N_PASSENGERS 8		//Numero de passageiros
#define N_P_CAR 4 				//Numero de passageiros por carrinho
#define N_CAR 2					//Numero de carrinhos

sem_t mutex;									//Semaforo para controle de mutex
sem_t mutex2;									//Semaforo para controle de mutex
sem_t filaBoard;							//Semaforo da fila de embarque
sem_t filaUnboard;						//Semaforo da fila de desembarque
sem_t todosDentro;						//Semaforo que indica se o carrinho lotou
sem_t todosFora;							//Semaforo que indica se todos sairam do carrinho
sem_t loadingArea[N_CAR];			//Controle de qual carrinho está embarcando
sem_t unloadingArea[N_CAR];		//Controle de qual carrinho está desembarcando

volatile int boarders = 0;
volatile int unboarders = 0;

int next(int id_atual){		//Retorna o id do próximo carrinho em relaçao ao atual
	return (id_atual + 1)%N_CAR;
}

void* carThread(void* v){

int i = *(int *) v;

sem_wait(&loadingArea[i]);						//Espera o seu carrinho chegar na zona de embarque
load ();															//Executa o embarque
filaBoard.signal (N_P_CAR);						//Libera a N_P_CAR posiçoes na filaBoard
sem_wait(&todosDentro);								//Espera todos entrarem no carrinho
loadingArea[next (i)].signal ();			//Sinaliza para o proximo carrinho

run ();																//Realiza corrida

sem_wait(unloadingArea[i]);						//Espera poder realizar o desembarque
unload ();														//Desembarca
filaUnboard.signal (N_P_CAR);					//Libera a N_P_CAR posiçoes na filaUnboard
sem_wait(&todosFora);								  //Espera todos descerem
unloadingArea[next (i)].signal ();		//Sinaliza para o proximo carrinho
}

void* passengerThread(void* v){
	sem_wait(&filaBoard);

	board (); /* embarca no carrinho */

	sem_wait(&mutex);
	boarders += 1;

	if (boarders == N_P_CAR){
		// indica que tem C passageiros a bordo
		sem_post(&todosDentro); 
	  	boarders = 0;
	}

	sem_post(&mutex);

	sem_wait(&filaUnboard);
	// desembarca passageiro
	unboard ();

	sem_wait(&mutex2);
	unboarders += 1;
	if (unboarders == N_P_CAR){
		sem_post(&todosFora);
		unboarders = 0;
	}

	sem_post(&mutex2);
}

int main(){


	return 0;
}
