#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "unistd.h"

int i = 0;

void * funcao1(void * arg){
  i = 5;
  pthread_exit(0);    
}

void * funcao2(void * arg){
  int y = 10;
  // printf("O programa executa x=y/0, antes da função 1 fazer i = 5");

  // Se chegar nesse ponto e a função1 não tiver sido executada, vai acontecer um erro (divisão por 0)
  // Se essa função2 fosse maior esse problema não aconteceria (o print ajuda a simular isso)
  // O print não é uma solução!!!
  int x = y/i;
  printf("Valor de x: %d\n",x);
  pthread_exit(0);    
}

int main(){
  printf("Condição de Corrida\n");

  pthread_t thread1, thread2;
  // Crio e coloco as threads para executar
  // A função 2 é a primeira a ser executada
  pthread_create(&thread2, NULL, funcao2, NULL);
  pthread_create(&thread1, NULL, funcao1, NULL);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  printf("Fim");

  return 0;
}