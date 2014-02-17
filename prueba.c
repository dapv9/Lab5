#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// main


int main(int prueba, char** lista_arg){
  FILE * Archivo;
  char* nomP=lista_arg[1];
  char* archivoVectores=lista_arg[2];
  char* c;

  char* numHilos="1";
  char* timeRun="2";
  char* numCuentas="3";
  char* valorInicial="100";
  char* numRepeticiones;

  Archivo = fopen(archivoVectores,"r+");

  if(Archivo == NULL){
      fprintf(stdout,"\nError al abrir los archivos.\n");
    return 0;
  }

  char dato[100], temp2[100];
  char *token;
  int status;
  pid_t pid, pid_hijo;

  while(feof(Archivo)==0){
    fgets(dato,100,Archivo);
    strcpy(temp2, dato);
    token= strtok(temp2, " ");
    numHilos= token;
    token= strtok(NULL, " ");
    timeRun= token;
    token= strtok(NULL, " ");
    numCuentas= token;
    token= strtok(NULL, " ");
    valorInicial= token;
    token= strtok(NULL, " ");
    numRepeticiones= token;
     pid=fork();
    if(pid<0){
      printf("Fork Failed");
      return 1;
    } else if(pid==0){
      char *temp[]= {numHilos, numHilos, timeRun, numCuentas, valorInicial, NULL};
      execv(nomP,temp);
      status= execv(nomP,temp);
    }
    wait(&status);

    printf("----------------------------------------------\n");
  }  
//imprime valores iniciales
  printf("nombre Programa: %s\n", nomP);
  printf("Nombre archivo de prueba: %s\n", archivoVectores);
  fclose(Archivo);
  return 0;
}

