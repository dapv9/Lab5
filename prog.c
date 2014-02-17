#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

struct Cuentas {
  int saldo;
  int id;
  sem_t semaforo;
};

void *depositar(int x,int cant);
void *retirar(int x, int cant);
void *transferir(int x,int y);
struct Cuentas *cuenta;
int numCuentas;
int timeRun;
//sem_t semaforo;

void *Hilos(void *arg){
  srand(time(NULL)); 
  int l=0;
  while(l<timeRun){
    int cuentaDepositante, cuentaDepositar;

    //Eleccion de las cuentas y monto a transferir al azar
    cuentaDepositante = (rand() % numCuentas) ;
    while(cuenta[cuentaDepositante].saldo==0){
      cuentaDepositante = (rand() % numCuentas) ;      
    }
    
    cuentaDepositar= (rand() % numCuentas) ;
  	while (cuentaDepositar==cuentaDepositante){
  		cuentaDepositar= (rand() % numCuentas) ;
  	}
    
    
    transferir(cuentaDepositante,cuentaDepositar);
    //sem_post(&semaforo);
    l++;
  }
  return 0;
}

//--------------------------------------------------------------------
// main

int main(int program, char* lista_arg[]){

  int numhilos=atoi(lista_arg[1]);
  timeRun=atoi(lista_arg[2]);
  numCuentas=atoi(lista_arg[3]);
  int valor=atoi(lista_arg[4]);
  int totalInicial= valor * numCuentas;

  if(numCuentas<2){
    printf("Necesita minimo 2 cuentas para funcionarn \n");
    return 0;
  }

  if(valor==0){
    printf("El saldo inicial no puede ser 0\n");
    return 0;
  }

//imprime valores iniciales
  printf("numero de hilos: %i\n", numhilos);
  printf("tiempo a correr: %i\n", timeRun);
  printf("numero de cuentas: %i\n", numCuentas);
  printf("saldo inicial: %d\n", valor);
  printf("total inicial cuentas :%d\n",totalInicial);
  printf("----------------------------------------------\n");
  cuenta= malloc(numCuentas * sizeof *cuenta);
  
  pthread_t hilo_id[numhilos];
  pthread_attr_t attr;


  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


  //Creacion Cuentas
  int w=0;
  while (w<numCuentas){
    cuenta[w].saldo=valor;
    cuenta[w].id=w;
    sem_init(&cuenta[w].semaforo, 0, 1);
    w++;
  }

  //Creacion Hilos
  w=0;
  while (w<numhilos){
    pthread_create(&hilo_id[w], &attr , Hilos , NULL);
    w++;
  }

  //Join de los hilos
  w=0;
  while (w<numhilos){
    pthread_join(hilo_id[w], NULL);
    w++;
  }

  //Impresion final de los valores 
  w=0;
  int total=0;
  while (w<numCuentas){
    printf("Saldo final cuenta: %i, valor :%d\n",w, cuenta[w].saldo);	
    total=total+cuenta[w].saldo;
    w++;
    sem_destroy(&cuenta[w].semaforo);
  }	

  printf("total final cuentas :%d\n", total);
  
  free(cuenta);
  pthread_attr_destroy(&attr);
  return 1;
  
}

//-------------------------------------------------------
//depositar
void* depositar(int x,int cant){
  cuenta[x].saldo=cuenta[x].saldo+cant;
  return 0;
}

//-------------------------------------------------------
//retirar
void *retirar(int x, int cant){
  cuenta[x].saldo=cuenta[x].saldo-cant;
   return 0;
}


//-------------------------------------------------------
//transferir
void *transferir(int x,int y){
  if (sem_trywait(&cuenta[x].semaforo)==0 && sem_trywait(&cuenta[y].semaforo)==0){

    int maximo= cuenta[x].saldo;
      
      int cantidad = (rand() % maximo +1) ;
      while (cantidad==0){
        cantidad = (rand() % maximo + 1) ;
      }
    retirar(x,cantidad);
    depositar(y, cantidad);
    sem_post(&cuenta[x].semaforo);
    sem_post(&cuenta[y].semaforo);
  }
  return 0;
}
