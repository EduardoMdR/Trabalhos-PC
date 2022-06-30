// Nesse programa se temos diversas thread que tentam acessar e aumentar o contador, 
// porém pode acontecer o caso de que mais de uma thread pega o mesmo valor do contador,
// e ambas podem salvar ao mesmo tempo sobrescrevendo a outra thread. Temos uma condição de corrida.
// Para resolver esse problema é utilizado o lock.

#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "unistd.h"

#define N 4
#define X 100000

int contador = 0;
int contador_unsafe = 0;
int i = 0;

// Criando um Lock
pthread_mutex_t lock_contador = PTHREAD_MUTEX_INITIALIZER;

void * incrementar(void *arg){
  int id = *((int *) arg); 
  printf("Criou um pthread com id = %d \n",id);

  for(i = 0; i < X; i++){
    // Quando uma thread acessar a linha 26, nenhuma outra pode acessar
    pthread_mutex_lock(&lock_contador);
      contador++;
    pthread_mutex_unlock(&lock_contador);
    // Ao liberar o acesso, uma nova thread vai executar a linha 26
    contador_unsafe++;
  }

  pthread_exit(0);
}

int main(){
  printf("Utilizando Locks\n");

  pthread_t thread[N];
  int * id;

  // Criando N thread, mandando executar incrementar(passando id)
  for(i = 0; i < N; i++){
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&thread[i], NULL, incrementar, (void *) (id));
  }

  for(i = 0; i < N; i++){
    pthread_join(thread[i], NULL);
  }

  // Apenas uma thread executou o comando contador++ por vez
  printf("Valor final do contador (utilizando lock) = %d\n", contador);
  
  // Não houve controle de quantas threads poderiam fazer o comando contador++ por vez
  printf("Valor final do contador_unsafe = %d\n", contador_unsafe);
  return 0;
}