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
#include <curses.h>
#include <unistd.h>
#include "interface.h"

//Migué para teste by:Helu
#define L1 9
#define L2 18
#define EMB2 getmaxx(stdscr)/2-5
#define DES2 getmaxx(stdscr)/2-6
//Migué para teste by:Helu

#define N_PASSENGERS 20	// Numero de passageiros
#define N_P_CAR 4				// Numero de passageiros por carrinho
#define N_CARS 5				// Numero de carrinhos
#define L_CARRO (N_P_CAR+N_P_CAR%2)		//y = 1/5*x + 12/5
#define P_EMBARQUE EMB+L_CARRO
#define P_DESEMBARQUE DES+L_CARRO
#define TEMPO 100000

sem_t mutex_1;
sem_t mutex_2;
sem_t board_queue;
sem_t unboard_queue;
sem_t all_aboard;
sem_t all_ashore;
sem_t loading_area[N_CARS];
sem_t unloading_area[N_CARS];
sem_t sem_write;
sem_t sem_line;

carrinho car[N_CARS];

volatile int refreshers;
volatile int writing;

int boarders;
int unboarders;

void refreshAll()
{
	while(1){
		usleep(TEMPO);
		if(refreshers){
			while(refreshers);
			return;
		}
		else if(!refreshers){
			refreshers = 1;
			while(writing){
				sem_wait(&sem_write);
				sem_post(&sem_write);
				usleep(TEMPO);
			}
			refresh();
			refreshers = 0;
			return;
		}
	}
}
//Funcao que desenha as threads na fila de espera da montanha russa

void printFila(int id)
{
  sem_wait(&sem_write); //Trocar mutex_3 pelo nome do semaforo que esta sendo utilizado para desenhar a animacao
	writing++;
  move(L1-2, EMB2+(3*id)); //L1 é a linha em que se encontra o trilho. É importante definir para cada trilho.
                          //EMB eh a coluna base onde se encontra a primeira letra do terminal de EMBARQUE
  printw("%d", id);
  sem_post(&sem_write);
	writing--;
	refreshAll();
}

//Funcao que faz as threads sairem da fila e entrarem no brinquedo
void board2(int id)
{
  sem_wait(&sem_write);
	writing++;
  move(L1-2, EMB2+(3*id));
  printw("  ");
  sem_post(&sem_write);
	writing--;
	refreshAll();
}

//Funcao que faz as threads sairem do brinquedo para o terminal de DESEMBARQUE
void unboard2(int id)
{
  sem_wait(&sem_write);
	writing++;
  move(L2-2, DES2+(3*id)); //DES eh a coluna base onde se encontra a primeira letra do terminal de DESEMBARQUE
  printw("%d", id);
  sem_post(&sem_write);
	writing--;
	refreshAll();
}

//Funcao que faz as threads sairem do terminal de DESEMBARQUE
void leave(int id)
{
  sem_wait(&sem_write);
	writing++;
  move(L2-2, DES2+(3*id));
  printw("  ");
  sem_post(&sem_write);
	writing--;
	refreshAll();
}

//Retorna o id do carrinho que está embarcando
int boardCar()
{
	for (int i=0; i<N_CARS;i++){
		if (car[i].boarding)
			return i;
	}
	return -1;
}

//Retorna o id do carrinho que está desembarcando
int unboardCar()
{
	for (int i=0; i<N_CARS;i++){
		if (car[i].unboarding)
			return i;
	}
	return -1;
}

// Essa funcao representa o carrinho correndo
void run(int id)
{
	for (int i=P_EMBARQUE; i<getmaxx(stdscr)+L_CARRO+2; i++){
		sem_wait(&sem_write);
		writing++;
		printCar(LE, i, L_CARRO, &car[id]);
		sem_post(&sem_write);
		writing--;
		refreshAll();
	}
}

// Essa funcao representa o carrinho pronto pros passageiros entrarem
void load(int id)
{
	for (int i=0; i<P_EMBARQUE; i++){
		sem_wait(&sem_write);
		writing++;
		printCar(LE, i, L_CARRO, &car[id]);
		sem_post(&sem_write);
		writing--;
		refreshAll();
	}
}

// Essa funcao representa o carrinho pronto pros passageiros sairem
// (em um problema real poderia fazer algum processamento aqui)
void unload(int id)
{
	for (int i=0; i<P_DESEMBARQUE; i++){
		sem_wait(&sem_write);
		writing++;
		printCar(LD, i, L_CARRO, &car[id]);
		sem_post(&sem_write);
		writing--;
		refreshAll();
	}
}

// Essa funcao representa a transferencia do carrinho do desembarque para embarque
// (em um problema real poderia fazer algum processamento aqui)
void transfer(int id)
{
	for (int i=P_DESEMBARQUE; i<getmaxx(stdscr)+L_CARRO+2; i++){
		sem_wait(&sem_write);
		writing++;
		printCar(LD, i, L_CARRO, &car[id]);
		sem_post(&sem_write);
		writing--;
		refreshAll();
	}
}
// Essa funcao representa o passageiro entrando no carrinho
// (em um problema real poderia fazer algum processamento aqui)
void board(int id)
{
	int carro = boardCar();
	int seat = 0;

	sem_wait(&sem_line);
	if (carro<0)
		printw("Nenhum carro embarcando");
	else{
		for (int i=0;i<N_P_CAR && seat == 0;i++){
			if (car[carro].passageiros[i] == -1){
				car[carro].passageiros[i] = id;
				seat = 1;
			}
		}
	}
	sem_post(&sem_line);

	sleep(1);
	sem_wait(&sem_write);
	writing++;
	printCar(LE, P_EMBARQUE, L_CARRO, &car[carro]);
	sem_post(&sem_write);
	writing--;
	refreshAll();
}

// Essa funcao representa o passageiro saindo do carrinho
// (em um problema real poderia fazer algum processamento aqui)
void unboard(int id)
{
	int carro = unboardCar();
	int out = 0;
	sem_wait(&sem_line);
	if (carro<0)
		printw("Nenhum carro embarcando");
	else{
		for (int i=0;i<N_P_CAR && out == 0;i++){
			if (car[carro].passageiros[i] == id){
				car[carro].passageiros[i] = -1;
				out = 1;
			}
		}
	}
	sem_post(&sem_line);

	sem_wait(&sem_write);
	writing++;
	printCar(LD, P_DESEMBARQUE, L_CARRO, &car[carro]);
	sem_post(&sem_write);
	writing--;
	refreshAll();
}

//Retorna o id do próximo carrinho em relaçao ao atual
int next(int id_atual)
{
	return (id_atual + 1)%N_CARS;
}

void* car_thread(void* v)
{
	while(1){
		int id = *(int *) v;

		sem_wait(&loading_area[id]);

		load(id);
		car[id].boarding = 1;
		// Fazendo os passageiros entrarem
		for (int i = 0; i < N_P_CAR; i++)
		{
			sem_post(&board_queue);
		}

		// Todo mundo entrou?
		sem_wait(&all_aboard);
		car[id].boarding = 0;

		// Pista pronta pro proximo carrinho
		sem_post(&loading_area[next(id)]);

		// UHUUU
		run(id);

		sem_wait(&unloading_area[id]);

		//printf("(c3) car_thread %d unloading\n", id);
		unload(id);
		car[id].unboarding = 1;

		// Fazendo os passageiros sairem
		for (int i = 0; i < N_P_CAR; i++)
		{
			sem_post(&unboard_queue);
		}

		// Todo mundo saiu?
		sem_wait(&all_ashore);
		car[id].unboarding = 0;

		// Pista pronta pro proximo carrinho
		sem_post(&unloading_area[next(id)]);

		transfer(id);

	}

	return NULL;
}

void* passenger_thread(void* v)
{
	int id = *(int *) v;

	while(1){
		//printf("(p1) pass_thread %d waiting on board_queue\n", id);
		printFila(id);
		sem_wait(&board_queue);

		//printf("(p2) pass_thread %d boarding\n", id);
		board2(id);
		board(id);

		//		Prende o mutex_1 para verificar se este eh o ultimo passageiro
		//	a entrar no carrinho sem que 2 facam ao mesmo tempo
		sem_wait(&mutex_1);

		boarders++;

		if(boarders == N_P_CAR)
		{
			//printf("(p nice) All threads aboard!\n\n");
			sem_post(&all_aboard);
			boarders = 0;
		}
		// solta o mutex
		sem_post(&mutex_1);

		sem_wait(&unboard_queue);

		//printf("(p3) pass_thread %d unboarding\n", id);
		unboard2(id);
		unboard(id);

		//		Prende o mutex_2 para verificar se este eh o ultimo passageiro
		//	a sair do carrinho sem que 2 facam ao mesmo tempo
		sem_wait(&mutex_2);

		unboarders++;
		if(unboarders == N_P_CAR)
		{
			//printf("(p oh no) All threads ashore!\n\n");
			sem_post(&all_ashore);
			unboarders = 0;
		}
		// solta o mutex
		sem_post(&mutex_2);

		leave(id);

	}

	return NULL;
}

int main()
{
	//
	// INICIALIZANDO VARIAVEIS E SEMAFOROS
	//
  int i = 0;
	// Inicializando os mutexes
	sem_init(&mutex_1, 0, 1);
	sem_init(&mutex_2, 0, 1);

	// Inicializando as filas de espera
	sem_init(&board_queue, 0, 0);
	sem_init(&unboard_queue, 0, 0);

	// Inicializando os semaforos de todos abordo e todos saindo
	sem_init(&all_aboard, 0, 0);
	sem_init(&all_ashore, 0, 0);

	// Inicializando semaforo de escrita
	sem_init(&sem_write,0,1);

	sem_init(&sem_line,0,1);

	refreshers = 0;
	writing = 0;
	// Inicializando o numero de passageiros entrando e saindo
	boarders = 0;
	unboarders = 0;

	// Inicializando a area de chegada dos passageiros
	for (i = 0; i < N_CARS; ++i)
	{
			sem_init(&loading_area[i], 0, 0);
	}
	// Area de chegada livre para o carrinho 0!
	sem_post(&loading_area[0]);

	// Inicializando a area de saida dos passageiros
	for (i = 0; i < N_CARS; ++i)
	{
			sem_init(&unloading_area[i], 0, 0);
	}
	// Area de saida livre para o carrinho 0!
	sem_post(&unloading_area[0]);

	// Inicializando os structures de car
	for (i=0; i<N_CARS;i++){
		car[i].passageiros = malloc ((N_P_CAR+1) * sizeof(int));
		for (int j=0;j<N_P_CAR;j++)
			car[i].passageiros[j] = -1;
		car[i].passageiros[N_P_CAR] = -2;
		car[i].boarding = 0;
		car[i].unboarding = 0;
	}

  // Inicializando a tela
	initscr();
	noecho();
	printTitulo();
	printRail();
	//
	// CRIANDO THREADS
	//

	// Criando as threads dos passageiros
	pthread_t passenger[N_PASSENGERS];
	int id_passenger[N_PASSENGERS];
	for (int i = 0; i < N_PASSENGERS; ++i)
	{
		id_passenger[i] = i;
		pthread_create(&passenger[i], NULL, passenger_thread, (void*) &id_passenger[i]);
	}
	// Criando as threads dos carrinhos
	pthread_t car[N_CARS];
	int id_car[N_CARS];
	for (int i = 0; i < N_CARS; ++i)
	{
		id_car[i] = i;
		pthread_create(&car[i], NULL, car_thread, (void*) &id_car[i]);
	}

	//
	// JOIN DAS THREADS
	//

	// finalizando as threads dos passageiros
	for (int i = 0; i < N_PASSENGERS; ++i)
	{
		pthread_join(passenger[i], NULL);
	}

	// finalizando as threads dos carrinhos
	for (int i = 0; i < N_CARS; ++i)
	{
		pthread_join(car[i], NULL);
	}
	getch();      //Fica esperando que o usuário aperte alguma tecla

	endwin();
	/*Ta dando erro, não sei porque:
	"request for member ‘passageiros’ in something not a structure or union"
	for (i=0; i<N_CARS;i++){
		free(car[i].passageiros);
	}*/

	return 0;
}
