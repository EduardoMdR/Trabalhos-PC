// A solução do problema consiste em direcionar as tarefas para o writer e reader
// e isso aontece pois o lock_tarefa varia em quem vai executar,
// Quando a tarefa pertence ao Escritor, impedi que crie novos leitores (num_leitores++)
	// Ou seja, num_leitores vai chegar em zero e vai liberar o bd
// Quando a tarefa pertence ao Leitor, seu fluxo segue normalmente
	// E o escritor fica travado

#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"

#define TRUE 1

#define NE 10																	// Número de escritores
#define NL 20 																// Número de leitores

pthread_mutex_t lock_bd = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_nl = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_tarefa = PTHREAD_MUTEX_INITIALIZER;
// Responsável pelo escalonamento de tarefas

int num_leitores = 0;

void * reader(void *arg);
void * writer(void *arg);
void read_data_base();
void use_data_read();
void think_up_data();
void write_data_base();

int main() {

	pthread_t r[NL], w[NE];
	int i;
  int *id;
  
  for (i = 0; i < NL ; i++) {									// Criando leitores
	  id = (int *) malloc(sizeof(int));
    *id = i;
	 	pthread_create(&r[i], NULL, reader, (void *) (id));
	}

	for (i = 0; i< NE; i++) {										// Criando escritores
	  id = (int *) malloc(sizeof(int));
    *id = i;
		pthread_create(&w[i], NULL, writer, (void *) (id));
	}

	pthread_join(r[0],NULL);
	return 0;
}

void * reader(void *arg) {
	int i = *((int *) arg);

	while(TRUE) {																// Repete para sempre
		pthread_mutex_lock(&lock_tarefa);					// Atribuo tarefa ao leitor
			pthread_mutex_lock(&lock_nl);						// So atribuo leitores (mum_leitores++) se a tarefa não for da escrita
				num_leitores++;
			pthread_mutex_unlock(&lock_tarefa);
				if(num_leitores == 1){
					pthread_mutex_lock(&lock_bd);				// Lock em Bd
				}
			pthread_mutex_unlock(&lock_nl);
		pthread_mutex_unlock(&lock_tarefa);

    read_data_base(i);												// Acesso aos dados (Crítico)

    pthread_mutex_lock(&lock_nl);
			num_leitores--;
			if(num_leitores == 0){
				pthread_mutex_unlock(&lock_bd);				// Unlock em Bd
			}
    pthread_mutex_unlock(&lock_nl);

		use_data_read(i);													// Região não crítica
	}
  pthread_exit(0);
}

void * writer(void *arg) {
	int i = *((int *) arg);
	
	while(TRUE) {																// Repete para sempre
		think_up_data(i);        									// Região não crítica
		pthread_mutex_lock(&lock_tarefa);					// Atribuo tarefa ao escritor
			pthread_mutex_lock(&lock_bd);
				write_data_base(i);      							// Atualiza os dados (Crítico)
			pthread_mutex_unlock(&lock_bd);
		pthread_mutex_unlock(&lock_tarefa);
  }
  pthread_exit(0);
}

void read_data_base(int i) {
	printf("Leitor %d esta lendo os dados! Numero de leitores: %d\n", i,num_leitores);
	sleep(rand() % 5);
}

void use_data_read(int i) {
	printf("Leitor %d esta usando os dados lidos! Numero de leitores: %d\n", i,num_leitores);
	sleep(rand() % 5);
}

void think_up_data(int i) {
	printf("Escritor %d esta pensando no que escrever!\n", i);
	sleep(rand() % 5);
}

void write_data_base(int i) {
	printf("Escritor %d esta escrevendo os dados! Numero de leitores: %d\n", i,num_leitores);
	sleep( rand() % 5 + 5);
}
