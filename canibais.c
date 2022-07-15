#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAXCANIBAIS 20
// Existe apenas um cozinheiro


void *canibal(void*meuid);
void *cozinheiro(int m);

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
  int i, numCanibais, qtdPorcoes;
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
    // ...
    // Pegar uma porção de comida e acordar o cozinheiro se as porções acabaram
     
    printf("%d: vou comer a porcao que peguei\n", *(int *)(pi));
    sleep(10);
  }
  
}

void *cozinheiro (int m){
 
  while(1){
     
    ...//dormir enquanto tiver comida
    printf("cozinheiro: vou cozinhar\n");
    sleep(20);
    ...//acordar os canibais
   }

}
