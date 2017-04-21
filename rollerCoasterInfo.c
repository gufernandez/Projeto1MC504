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

#define N_PASSENGERS 8;		//Numero de passageiros
#define N_P_CAR 4; 				//Numero de passageiros por carrinho
#define N_CAR 2;					//Numero de carrinhos

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
	/*---------------Eu mudei pro caso de N carrinhos----------------
	load(); /* enche o carrinho com C passageiros */

	//sem_post(&filaBoard);
	//sem_wait(&todosDentro);
//	filaBoard.signal ( C );
//	todosDentro.wait ();

	//run(); /* coloca carrinho em movimento */

	//unload (); /* desembarque de passageiros */

	//sem_post(&filaUnboard);
	//sem_post(&todosFora);
//	filaUnboard.signal ( C );
//	todosFora.wait ();
/*--------------------------------------------------------------------------*/
int i = *(int *) v;

sem_wait(&loadingArea[i]);						//Espera o seu carrinho chegar na zona
																			//de embarque
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
//	filaBoard.wait ();
	board (); /* embarca no carrinho */

	sem_wait(&mutex);
//	mutex.wait ();
	boarders += 1;

	if (boarders == N_P_CAR){
		sem_post(&todosDentro); /* indica que tem C passageiros a bordo */
//	  	todosDentro . signal ();
	  	boarders = 0;
	}

	sem_post(&mutex);
//	mutex.signal ();

	sem_wait(&filaUnboard);
//	filaUnboard.wait ();
	unboard (); /* desembarca passageiro */

	sem_wait(&mutex2);
//	mutex2.wait ();
	unboarders += 1;
	if (unboarders == N_P_CAR){
		sem_post(&todosFora);
//	  	todosFora.signal ();
		unboarders = 0;
	}

	sem_post(&mutex2);
//	mutex2.signal ();
}
/*Passengers wait for the car before boarding, naturally, and wait for the car to
stop before leaving. The last passenger to board signals the car and resets the
passenger counter.*/

int main(){
	pthread_t passengers[N_PASSENGERS], car[N_CAR];
	int i, id[N_PASSENGERS];

	for (i = 0; i < N_CAR; i++) {
			if (i == 0){
				sem_init(&loadingArea[i], 0, 1);		//Apenas o carrinho 1 pode embarcar
				sem_init(&unloadingArea[i], 0, 1);	//e desembarcar
			}
			sem_init(&loadingArea[i], 0, 0);
			sem_init(&unloadingArea[i], 0, 0);
	}

	sem_init(&mutex, 0, 1);
	sem_init(&mutex2, 0, 1);
	sem_init(&filaBoard, 0, N_P_CAR); // 4 passageiros num carrinho
	sem_init(&filaUnboard, 0, N_P_CAR);
	sem_init(&todosDentro, 0, 1);
	sem_init(&todosFora, 0, 1);

	for(i = 0; i < N_PASSENGERS; i++){
		id[i] = i;
		pthread_create(&passengers[i], NULL, passengerThread, (void*) &id[i]);
	}

	for(i=0; i < N_CAR; i++){
		id[i] = i;
		pthread_create(&car[i], NULL, carThread, (void*) &id[i]);
	}
	for(i = 0; i < N_PASSENGERS; i++)
		pthread_join(passenger[i], NULL);

	/*TEM QUE DAR JOIN PRA THREAD DE CAR? (EU TO NO CAPS PRA CHAMAR ATENÇÃO SÓ
    ME DESCULPA)*/

	return 0;
}
