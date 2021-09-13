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

int j = 0;
int timer = 0;
bool primero = false;
void global_clock(){
  printf("pasara una unidad de tiempo....\n");
  sleep(1);
  printf("paso unidad de tiempo, ahora son las %i\n", timer);
  timer++;
}

void prioridad_procesos(Process *argv[], int j, LinkedList* linkedlist){
  //int len = sizeof(argv)/sizeof(Process*);'
  printf("funcion prioridad procesos %i \n", j);
  if (j == 1 & primero == false){
    printf("primerisimo\n");
    primero = true;
    append_first(linkedlist, argv[0]);
  }
  else if( j == 1 & primero == true){
    printf("entro uno\n");
    append_linkedlist(linkedlist, argv[0]);   
  }
  else{
    printf("desempate\n");
    for (int i = 0; i < j; i++){
      if (argv[i]->state == WAITING){
        printf("gano waiting\n");
        append_linkedlist(linkedlist, argv[i]);
      }
      else if (argv[i]->state == READY & argv[i]->quantum == 1){
        printf("gano redy y qtm\n");
        append_linkedlist(linkedlist, argv[i]);
      }
      else{
        printf("llego uno nuevo\n");
        //condiciones menor numero de fabrica o menos nombre
        if (primero == false){
          primero = true;
          append_first(linkedlist, argv[i]);
        }
        else{
          append_linkedlist(linkedlist, argv[i]);
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
        int time_start = atoi(line[1]);
        Process* process = process_init(pid, line[0], id_factory, time_start);
        //aca se guardan los procesos para iterarlos luego
        lista_procesos[i] = process;

        //printf("soy el objeto fabrica pid:%i name:%s factory:%i\n", process->pid, process->name, process->name_factory);
  }

  LinkedList * linkedlist = linkedlist_init();
  
  printf("inicio:%s\n",linkedlist->head->name);
  printf("cola: %s\n",linkedlist->tail->name);
  // aca de revisa si es tiempo de que llegue a la cola algun proceso
  int len = sizeof(lista_procesos)/sizeof(Process*);
  for (int i = 0; i < len; i++){
    printf("time start %i\n", lista_procesos[i]->time_start);
    printf("timer: %i\n", timer);
    if(lista_procesos[i]->time_start == timer){
      //printf("entre con j = %i\n", j);
      lista_procesos_entrando[j]=lista_procesos[i];
      j++;
    }
  }
  prioridad_procesos(lista_procesos_entrando, j, linkedlist);


}
