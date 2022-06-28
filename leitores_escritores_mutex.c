#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"

#define TRUE 1

#define NE 3																// Número de escritores
#define NL 10 															// Número de leitores

pthread_mutex_t lock_bd = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_nl = PTHREAD_MUTEX_INITIALIZER;

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
  
  for (i = 0; i < NL ; i++) {								// Criando leitores
	  id = (int *) malloc(sizeof(int));
    *id = i;
	 	pthread_create(&r[i], NULL, reader, (void *) (id));
	}

	for (i = 0; i< NE; i++) {									// Criando escritores
	  id = (int *) malloc(sizeof(int));
    *id = i;
		pthread_create(&w[i], NULL, writer, (void *) (id));
	}

	pthread_join(r[0],NULL);
	return 0;
}

void * reader(void *arg) {
	int i = *((int *) arg);

	while(TRUE) {															// Repete para sempre
		pthread_mutex_lock(&lock_nl);
			num_leitores++;
			if(num_leitores == 1){
				pthread_mutex_lock(&lock_bd);				// Lock em Bd
			}
    pthread_mutex_unlock(&lock_nl);

    read_data_base(i);											// acesso aos dados (Crítico)

    pthread_mutex_lock(&lock_nl);
			num_leitores--;
			if(num_leitores == 0){ 
				pthread_mutex_unlock(&lock_bd);			// Unlock em Bd
			}
    pthread_mutex_unlock(&lock_nl);

    /* região não crítica */
		use_data_read(i);
	}
  pthread_exit(0);
}

void * writer(void *arg) {
	int i = *((int *) arg);
	
	while(TRUE) {															// Repete para sempre
		think_up_data(i);        								// Região não crítica
    pthread_mutex_lock(&lock_bd);
			write_data_base(i);      							// Atualiza os dados (Crítico)
		pthread_mutex_unlock(&lock_bd);
  }
  pthread_exit(0);
}

void read_data_base(int i) {
	printf("Leitor %d está lendo os dados! Número de leitores: %d\n", i,num_leitores);
	sleep(rand() % 5);
}

void use_data_read(int i) {
	printf("Leitor %d está usando os dados lidos! Número de leitores: %d\n", i,num_leitores);
	sleep(rand() % 5);
}

void think_up_data(int i) {
	printf("Escritor %d está pensando no que escrever!\n", i);
	sleep(rand() % 5);
}

void write_data_base(int i) {
	printf("Escritor %d está escrevendo os dados! Número de leitores: %d\n", i,num_leitores);
	sleep( rand() % 5 + 15);
}
