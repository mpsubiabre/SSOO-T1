#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "../file_manager/manager.h"
#include "scheduler.h"
//#include "../process/process.h"

typedef enum {
  RUNNING,
  READY,
  WAITING,
  FINISHED
  } STATES;
int Q = 100;//cambiar a variable de input!!!!!!!!!
int j = 0;
int timer = 0;// variable global
int total_factories = 0 ; 
bool primero = false; //permite saber si es el priemro en entrar a la cola
int numero_fabricas_cola = 0;
int quantum_factories[];

void global_clock(){ 
  printf("pasara una unidad de tiempo....\n");
  sleep(5);
  timer++;
  printf("paso unidad de tiempo, ahora son las %i\n", timer);
  
}

void calcular_quantum(LinkedList* linkedlist){
  printf("----entre a calcular qum\n");
  for (int i = 0; i < total_factories+1; i++){
    printf(" variables calculo quantum:\n");
    printf("Q: %i\n", Q);
    printf("ni: %i\n", linkedlist->f_en_cola[i]);
    printf("f: %i\n", numero_fabricas_cola);
    if (numero_fabricas_cola > 0 && linkedlist->f_en_cola[i] > 0){
      printf("entre al if en calculo quatntum\n");
      quantum_factories[i] =(int)(Q/(linkedlist->f_en_cola[i]*numero_fabricas_cola)); 
      printf(" el quantum de la fabrica %i es: %i\n", i, quantum_factories[i]);
    }
  }
}

int revisar_entrada_nuevos_procesos(Process** lista_procesos, Process** lista_procesos_entrando, int j, int len ){
  // aca de revisa si es tiempo de que llegue a la cola algun proceso
  //int len = sizeof(lista_procesos)/sizeof(Process*);
  printf("len %i\n", len);
  j=0;
  for (int i = 0; i < len; i++){
    printf("time start %i\n", lista_procesos[i]->time_start);
    printf("timer: %i\n", timer);
    if(lista_procesos[i]->time_start == timer){
      printf("entre con j = %i\n", j);
      printf("-----SE CREO UN PROCESO NUEVO-----\n");
      lista_procesos_entrando[j] = lista_procesos[i];
      j++;
    }
  }
  return j;
}

void prioridad_procesos(Process *argv[], int j, LinkedList* linkedlist){
  printf("entre a funcion prioridad con numero %i de procesos\n", j);

  if (j == 1 & primero == false){
    printf("primerisimo\n");
    primero = true;
    append_first(linkedlist, argv[0]);
    numero_fabricas_cola ++;
    printf("pertenece a la fabrica %i\n", argv[0]->id_factory);
    linkedlist->f_en_cola[argv[0]->id_factory]++;
  }
  else if( j == 1 & primero == true){
    printf("entro uno noma\n");
    append_linkedlist(linkedlist, argv[0]);
    numero_fabricas_cola ++;   
    printf("pertenece a la fabrica %i\n", argv[0]->id_factory);
    linkedlist->f_en_cola[argv[0]->id_factory]++;
  }
  else{
    printf("desempate\n");
    for (int i = 0; i < j; i++){
      if (argv[i]->state == WAITING){
        printf("gano waiting\n");
        append_linkedlist(linkedlist, argv[i]);
        numero_fabricas_cola ++;
        printf("pertenece a la fabrica %i\n", argv[i]->id_factory);
        linkedlist->f_en_cola[argv[i]->id_factory]++;
      }
      else if (argv[i]->state == READY & argv[i]->quantum == 1){
        printf("gano redy y qtm\n");
        append_linkedlist(linkedlist, argv[i]);
        numero_fabricas_cola ++;
        linkedlist->f_en_cola[argv[i]->id_factory]++;
        printf("pertenece a la fabrica %i\n", argv[i]->id_factory);
        linkedlist->f_en_cola[argv[i]->id_factory]++;
      }
      else{
        printf("llego un nuevo proceso\n");
        //condiciones menor numero de fabrica o menos nombre
        if (primero == false){
          primero = true;
          append_first(linkedlist, argv[i]);
          numero_fabricas_cola ++;
          printf("pertenece a la fabrica %i\n", argv[i]->id_factory);
          linkedlist->f_en_cola[argv[i]->id_factory]++;
        }
        else{
          append_linkedlist(linkedlist, argv[i]);
          numero_fabricas_cola ++;
          printf("pertenece a la fabrica %i\n", argv[i]->id_factory);
          linkedlist->f_en_cola[argv[i]->id_factory]++;
        }
      }
    }
    
  }  
  printf("inicio:%s\n",linkedlist->head->name);
  printf("cola: %s\n",linkedlist->tail->name);
  }


int main(int argc, char **argv)
{
  printf("Hello T2!\n");

  InputFile *file = read_file("input.txt");

  printf("Reading file of length %i:\n", file->len);

  Process* lista_procesos[file->len];
  Process* lista_procesos_entrando[file->len];

  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];
    printf(
        "\tProcess %s from factory %s has init time of %s and %s bursts.\n",
        line[0], line[2], line[1], line[3]);
        int pid = i;
        char* name = line[0];
        int id_factory = atoi(line[2]);
        if (total_factories < id_factory){
          total_factories = id_factory; 
        }
        int time_start = atoi(line[1]);
        Process* process = process_init(pid, name, id_factory, time_start);
        //aca se guardan los procesos para iterarlos luego
        lista_procesos[i] = process;

        //printf("soy el objeto fabrica pid:%i name:%s factory:%i\n", process->pid, process->name, process->name_factory);
  }

  LinkedList * linkedlist = linkedlist_init(total_factories);
  //se inicializa con cero
  for (int i = 0; i < total_factories+1; i++){
    linkedlist->f_en_cola[i]=0;
    quantum_factories[i]=0;
  }
  
  while(true){
    
  // aca de revisa si es tiempo de que llegue a la cola algun proceso
  int len = sizeof(lista_procesos)/sizeof(Process*);
  printf("------------------------review------------------\n");
  j = revisar_entrada_nuevos_procesos(lista_procesos, lista_procesos_entrando, j, len);
  printf("------------------------review------------------\n");
  
  prioridad_procesos(lista_procesos_entrando, j, linkedlist);
  for (int i = 0; i < total_factories+1; i++){
    printf("f en cola %i\n", linkedlist->f_en_cola[i]);
  }
  calcular_quantum(linkedlist);
  global_clock();
}
}
