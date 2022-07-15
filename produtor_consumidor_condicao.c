#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PR 1 //número de produtores
#define CN 1 // número de consumidores
#define N 5  //tamanho do buffer

void * produtor(void * meuid);
void * consumidor (void * meuid);

pthread_cond_t cons_cond = PTHREAD_COND_INITIALIZER;        // condição consumidor
pthread_cond_t prod_cond = PTHREAD_COND_INITIALIZER;        // condição produtor
pthread_mutex_t lock_buffer = PTHREAD_MUTEX_INITIALIZER;    // controla acesso ao buffer
int buffer = 0;

void main(argc, argv)
int argc;
char *argv[];{
  int erro;
  int i, n, m;
  int *id;

  pthread_t tid[PR];

  for (i = 0; i < PR; i++){
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tid[i], NULL, produtor, (void *) (id));

    if(erro){
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }

  pthread_t tCid[CN];

  for (i = 0; i < CN; i++){
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tCid[i], NULL, consumidor, (void *) (id));

    if(erro){
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }
  pthread_join(tid[0],NULL);
}

void * produtor (void* pi){
  int i = *((int *)pi);
  while(1){
    pthread_mutex_lock(&lock_buffer);                       // pegando acesso ao buffer
      while (buffer == N){                                  // Se o buffer estiver cheio, o produtor vai esperar
        pthread_cond_wait(&prod_cond, &lock_buffer);
      }
      buffer++;
      printf("Item inserido do buffer, tamanho = %d\n", buffer);

      if (buffer == 1) {
        pthread_cond_signal(&cons_cond);
      }
    pthread_mutex_unlock(&lock_buffer);
    sleep(rand()%(i+1));
  }
  pthread_exit(0);
}

void * consumidor (void* pi){
  int i = *((int *)pi);
  while(1){
    pthread_mutex_lock(&lock_buffer);                       // pegando acesso ao buffer
      while (buffer == 0) {                                 // Se o buffer estiver vazio, o consumidor vai esperar
        pthread_cond_wait(&cons_cond, &lock_buffer);
      }
      buffer--;
      printf("Item removido do buffer, tamanho = %d\n", buffer);

      if (buffer == N-1){
        printf("Acorda produtor! Tamanho do buffer = %d\n", buffer);
        pthread_cond_signal(&prod_cond);
      }
    pthread_mutex_unlock(&lock_buffer);
    sleep(rand()%(i+1));
  }
  pthread_exit(0);
}
