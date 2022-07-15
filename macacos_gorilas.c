// Programa não está completo,
// Sua lógica está pronta 
// mas ainda n está funcionando

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAXCANIBAIS 20
// Existe apenas um cozinheiro
int qtdPorcoes = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; /* lock pro contador*/
pthread_cond_t cond_cozinheiro = PTHREAD_COND_INITIALIZER; 
pthread_cond_t cond_canibal = PTHREAD_COND_INITIALIZER;

void *canibal(void*meuid);
void *cozinheiro(int qtdPorcoes);

// Problema dos canibais
// Canibais não podem se servir ao mesmo tempo, mas podem comer ao mesm tempo
// não podem se servir se o caldeirão estiver vazio
// só podem se servir depois que o cozinheiro terminar de fazer as questões
// se o caldeirão estiver vazio os canibais esperam dormindo
// Os cozinheiros só podem encher o caldeirão se ele estiver vazio

// Salvando no aprender para continuar em casa

void main(argc, argv)
int argc;
char *argv[];{
  int erro;
  int i, numCanibais;
  int *id;

  pthread_t tid[MAXCANIBAIS];

  if(argc != 3){
    printf("erro na chamada do programa: jantar <#canibais> <#comida>\n");
    exit(1);
  }
  
  numCanibais = atoi (argv[1]);                                     // Número de canibais
  qtdPorcoes = atoi (argv[2]);                                      // Quantidade de porções que o cozinheiro consegue preparar por vez
  printf("numero de canibais: %d -- quantidade de comida: %d\n", numCanibais, qtdPorcoes);

  if(numCanibais > MAXCANIBAIS){
    printf("o numero de canibais e' maior que o maximo permitido: %d\n", MAXCANIBAIS);
    exit(1);
  }
  
  for (i = 0; i < numCanibais; i++)  {
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tid[i], NULL, canibal, (void *) (id));

    if(erro){
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }

  cozinheiro(qtdPorcoes);
}

void * canibal (void* pi){
  
  while(1) {
    pthread_mutex_lock(&lock);
    while(qtdPorcoes != 0){
      pthread_cond_wait(&cond_cozinheiro, &lock);
    }
    // ...
    // Pegar uma porção de comida e acordar o cozinheiro se as porções acabaram

    printf("%d: vou comer a porcao que peguei\n", *(int *)(pi));
    sleep(10);
    qtdPorcoes--;
    if(qtdPorcoes == 0){
      printf("Acordando cozinheiro\n");
      pthread_cond_signal(&cond_cozinheiro);
    }
    pthread_mutex_unlock(&lock);
    sleep(1);
  }
}

void *cozinheiro (int m){
  while(1){
    pthread_mutex_lock(&lock);
    //dormir enquanto tiver comida
    while(qtdPorcoes != 0){
      //printf("\nCozinheiro %d no aguardo, existem ainda %d porções de comida\n",id, comida);
      pthread_cond_wait(&cond_cozinheiro, &lock);
    }
    printf("cozinheiro: vou cozinhar\n");
    qtdPorcoes++;
    if(qtdPorcoes >= 10){
      pthread_cond_broadcast(&cond_canibal);
    }
    pthread_mutex_unlock(&lock);
    sleep(1);
    //acordar os canibais
  }
}
