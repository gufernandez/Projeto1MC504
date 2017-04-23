
//Funcao que desenha as threads na fila de espera da montanha russa
void printFila(int id){
  sem_wait(&mutex_3); //Trocar mutex_3 pelo nome do semaforo que esta sendo utilizado para desenhar a animacao
  move(L1-1, EMB+(2*id)); //L1 é a linha em que se encontra o trilho. É importante definir para cada trilho.
                          //EMB eh a coluna base onde se encontra a primeira letra do terminal de EMBARQUE
  printw("%d", id);
  usleep(1000000);
  move(0,0);
  refresh();
  sem_post(&mutex_3);
}

//Funcao que faz as threads sairem da fila e entrarem no brinquedo
void board(int id)
{
  sem_wait(&mutex_3);
  move(L1-1, EMB+(2*id));
  printw(" ");
  usleep(1000000);
  move(0,0);
  refresh();
  sem_post(&mutex_3);
}

//Funcao que faz as threads sairem do brinquedo para o terminal de DESEMBARQUE
void unboard(int id)
{
  sem_wait(&mutex_3);
  move(L1-1, DES+(2*id)); //DES eh a coluna base onde se encontra a primeira letra do terminal de DESEMBARQUE
  printw("%d", id);
  usleep(1000000);
  move(0,0);
  refresh();
  sem_post(&mutex_3);
}

//Funcao que faz as threads sairem do terminal de DESEMBARQUE
void leave(int id){
  sem_wait(&mutex_3);
  move(L1-1, DES+(2*id));
  printw(" ");
  usleep(1000000);
  move(0,0);
  refresh();
  sem_post(&mutex_3);
}
