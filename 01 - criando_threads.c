#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "unistd.h"

void * funcao1(void * arg){
  printf("Thread 1\n");
}

void * funcao2(void * arg){
  printf("Thread 2\n");
}

int main(){
  printf("Entendendo as Threads\n");

  // Criando as threads
  pthread_t thread1, thread2;

  // Inicializando as threads (vai ser inicializada e executada)
  // Ambas as funções serão executadas paralelamente
  pthread_create(&thread1, NULL, funcao1, NULL);
  pthread_create(&thread2, NULL, funcao2, NULL);

  // Finalizar o ramo de execução de uma thread
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  // Sem ele, caso o programa finalize, as thread não serão realizadas até o fim

  printf("Fim");

  return 0;
}