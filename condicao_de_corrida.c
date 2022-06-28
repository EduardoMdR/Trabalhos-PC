#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "unistd.h"


int main(){
  printf("Condição de Corrida\n");

  pthread_t thread1, thread2;

  printf("Fim");

  return 0;
}