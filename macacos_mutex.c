#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MA 10 //macacos que andam de A para B
#define MB 10 //macacos que andam de B para A

pthread_mutex_t lock_ponte = PTHREAD_MUTEX_INITIALIZER;   // lock para utilizar a ponte
pthread_mutex_t lock_monkeysAB = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_monkeysBA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_gorillas = PTHREAD_MUTEX_INITIALIZER;

// Controle qtd de mamacoss
int num_macacos_MA = 0, num_macacos_MB = 0;

void * macacoAB(void * a) {
  int i = *((int *) a);    
  while(1){

    pthread_mutex_lock(&lock_gorillas);                   // Garantir que seu truno seja exclusivo (com gorilas)
      pthread_mutex_lock(&lock_monkeysAB);                // Pegar acesso a ponte (mesma ideia do leitores e escritores)
        num_macacos_MA++;                                 // os macacos funcionam como os leitores
        if(num_macacos_MA == 1){
          pthread_mutex_lock(&lock_ponte);
        }
      pthread_mutex_unlock(&lock_monkeysAB);
    pthread_mutex_unlock(&lock_gorillas);
    
    //Procedimentos para acessar a corda
    printf("Macaco %d passado de A para B \n",i);         // Área exclusiva
    sleep(1);
    //Procedimentos para quando sair da corda

    pthread_mutex_lock(&lock_monkeysAB);                  // Devolver acesso a ponte
      num_macacos_MA--;
      if(num_macacos_MA == 0){
        pthread_mutex_unlock(&lock_ponte);
      }
    pthread_mutex_unlock(&lock_monkeysAB);
  }
  pthread_exit(0);
}

void * macacoBA(void * a) {                               // O mesmo processo para macacoAB
  int i = *((int *) a);    
  while(1){
    pthread_mutex_lock(&lock_gorillas);
      pthread_mutex_lock(&lock_monkeysBA);
        num_macacos_MB++;
        if(num_macacos_MB == 1){
          pthread_mutex_lock(&lock_ponte);
        }
      pthread_mutex_unlock(&lock_monkeysBA);
    pthread_mutex_unlock(&lock_gorillas);
    
    //Procedimentos para acessar a corda
    printf("Macaco %d passado de B para A \n",i);
    sleep(1);
    //Procedimentos para quando sair da corda

    pthread_mutex_lock(&lock_monkeysBA);
      num_macacos_MB--;
      if(num_macacos_MB == 0){
        pthread_mutex_unlock(&lock_ponte);
      }
    pthread_mutex_unlock(&lock_monkeysBA);
  }
  pthread_exit(0);
}

void * gorila(void * a){
  while(1){
    printf("Gorila deseja passar de A para B \n");
    pthread_mutex_lock(&lock_gorillas);                   // Mesmo exemplo que o escritor
      pthread_mutex_lock(&lock_ponte);                    // Pegar acesso a ponte
    pthread_mutex_unlock(&lock_gorillas);

    //Procedimentos para acessar a corda
    printf("Gorila passado de A para B \n");
    sleep(5);
    //Procedimentos para quando sair da corda

    pthread_mutex_unlock(&lock_ponte);
  }
  pthread_exit(0);
}

int main(int argc, char * argv[]){
  pthread_t macacos[MA+MB];
  int *id;
  int i = 0;

  for(i = 0; i < MA+MB; i++){
    id = (int *) malloc(sizeof(int));
    *id = i;
    if(i%2 == 0){
      if(pthread_create(&macacos[i], NULL, &macacoAB, (void*)id)){
        printf("Não pode criar a thread %d\n", i);
        return -1;
      }
    }else{
      if(pthread_create(&macacos[i], NULL, &macacoBA, (void*)id)){
        printf("Não pode criar a thread %d\n", i);
        return -1;
      }
    }
  }
  pthread_t g;
  pthread_create(&g, NULL, &gorila, NULL);

  pthread_join(macacos[0], NULL);
  return 0;
}
