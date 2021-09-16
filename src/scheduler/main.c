#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "../file_manager/manager.h"
#include "scheduler.h"
//#include "../process/process.h"


int Q = 100;//cambiar a variable de input!!!!!!!!!
int j = 0;
int timer = 0;// variable global
int total_factories = 0 ; 
bool primero = false; //permite saber si es el priemro en entrar a la cola
int numero_fabricas_cola = 0;
int quantum_factories[4];
bool proceso_nuevo = false;
Process* cpu=NULL;
LinkedList* terminados;


void global_clock(){ 
  printf("pasara una unidad de tiempo....\n");
  sleep(1);
  timer++;
  printf("paso unidad de tiempo, ahora son las %i\n", timer);
}

void calcular_quantum(LinkedList* linkedlist,Process* proceso){
  printf("----entre a calcular qum\n");
  int id_fabrica = proceso->id_factory;
  int f_quantum = 0;
  int ni_quantum = linkedlist->f_en_cola[id_fabrica];
  if (ni_quantum == 0){
    ni_quantum++;
  }
  for (int i = 0; i < 4; i++){
    if (linkedlist->f_en_cola[i]>0){
      f_quantum++;
      //printf("La fabrica %i tiene %i procesos en la cola\n", id_fabrica, linkedlist->f_en_cola[i]);
    }
  }
  if (f_quantum < 0 ){
    printf("problemas....f_quantum < 0 \n");
  }
  else{
    //printf(" variables calculo quantum:\n");
    //printf("Q: %i\n", Q);
    //printf("ni: %i\n", ni_quantum);
    //printf("f: %i\n", f_quantum);
    //int multiplicacion = (int)ni_quantum*f_quantum;
    int multiplicacion;
    multiplicacion =((int) ni_quantum)*((int)f_quantum);
    //printf("multiplicacion %i\n", multiplicacion);
    quantum_factories[id_fabrica] =(int)(Q/multiplicacion); 
    
  }
  printf("el quantum de la fabrica %i es: %i\n", id_fabrica, quantum_factories[id_fabrica]);
}

int revisar_entrada_nuevos_procesos(Process** lista_procesos, Process** lista_procesos_entrando, int j, int len ){
  // aca de revisa si es tiempo de que llegue a la cola algun proceso
  //int len = sizeof(lista_procesos)/sizeof(Process*);
  printf("len %i\n", len);
  j=0;
  for (int i = 0; i < len; i++){
    printf("time start %i || timer: %i\n", lista_procesos[i]->time_start, timer);
    if(lista_procesos[i]->time_start == timer){
      printf("[t = %i] se ha creado el proceso %s\n", timer, lista_procesos[i]->name);
      lista_procesos_entrando[j] = lista_procesos[i];
      j++;
      proceso_nuevo = true;
    }
  }
  printf("SALI DE REVISAR ENTRADA NUEVSprocesos\n");
  return j;
}


void prioridad_procesos(Process *argv[], int j, LinkedList* linkedlist){

  printf("entre a funcion prioridad con numero %i de procesos\n", j);
  if (j == 0){
    return;
  }
  printf("J ES :%i\n", j);
  for (int i = 0; i < j; i++){
    printf("lista_procesos entrando antes : \n");
    printf("%s\n", argv[i]->name);
  }
  if ((j == 1 && primero == false)|| linkedlist->head==NULL){
    printf("primerisimo\n");
    primero = true;
    append_first(linkedlist, argv[0]);
    numero_fabricas_cola ++;
    printf("pertenece a la fabrica %i\n", argv[0]->id_factory);
    linkedlist->f_en_cola[argv[0]->id_factory]++;
    linkedlist->lista_n_procesos_fabricas[argv[0]->id_factory]++;
  }
  else if( j == 1 && primero == true){
   // printf("entro uno noma\n");
    append_linkedlist(linkedlist, argv[0]);
    numero_fabricas_cola ++;   
    printf("pertenece a la fabrica %i\n", argv[0]->id_factory);
    linkedlist->f_en_cola[argv[0]->id_factory]++;
    linkedlist->lista_n_procesos_fabricas[argv[0]->id_factory]++;
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
        linkedlist->lista_n_procesos_fabricas[argv[i]->id_factory]++;
      }
      else if (argv[i]->state == READY && argv[i]->quantum == 1){
        printf("gano redy y qtm\n");
        append_linkedlist(linkedlist, argv[i]);
        numero_fabricas_cola ++;
        linkedlist->f_en_cola[argv[i]->id_factory]++;
        printf("pertenece a la fabrica %i\n", argv[i]->id_factory);
        linkedlist->f_en_cola[argv[i]->id_factory]++;
        linkedlist->lista_n_procesos_fabricas[argv[i]->id_factory]++;
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
          linkedlist->lista_n_procesos_fabricas[argv[i]->id_factory]++;
        }
        else{
          append_linkedlist(linkedlist, argv[i]);
          numero_fabricas_cola ++;
          printf("pertenece a la fabrica %i\n", argv[i]->id_factory);
          linkedlist->f_en_cola[argv[i]->id_factory]++;
          linkedlist->lista_n_procesos_fabricas[argv[i]->id_factory]++;
        }
      }
    }
    
  } 
  
  printf("inicio:%s\n",linkedlist->head->name);
  printf("Estado:%d\n",linkedlist->head->state);
  //print_linkedlist(linkedlist);
  printf("cola: %s\n",linkedlist->tail->name);
  printf(" PRINTEANDO J : %i\n", j);
  for (int i = 0; i < j; i++){
    printf("lista_procesos entrando antes : \n");
    printf("%s", argv[i]->name);
    argv[i] = NULL;
    printf("lista_procesos entrando despues : \n");
  }
  proceso_nuevo = false;

  }

// cuando CPU es null
Process* ingresar_cpu(LinkedList* linkedlist, Process* cpu){
  cpu->tiempo_entrada_waiting=0;
  if (linkedlist->head ==NULL)
  {
    printf("No quedan elementos en la cola\n");
    cpu = NULL;
    return cpu;
  }
  // Si la El primer proceso esta Ready entra a cpu
  if (linkedlist->head->state == READY)
  {
    printf("usare la nueva funcion delete!!!!!!!!!!\n");
    cpu = delete_process_pachi(linkedlist,linkedlist->head);
    printf("Entra a CPU el proceso %s\n", cpu->name);
    int id_fabrica_current = cpu -> id_factory;
    linkedlist->lista_n_procesos_fabricas[cpu->id_factory]--;

    printf("Se elimina de la cola el proceso\n");
    if(linkedlist->head == NULL){
      printf("NO quedan elementos\n");
    }
    cpu->state = RUNNING;
    printf("el estado del proceso paso a %d\n", cpu->state);
    //ingresar el tiempo en que entro a cpu
    cpu->t_entrada_cpu=timer;
    calcular_quantum(linkedlist, cpu);
    cpu->next = NULL;
    return cpu;
  }
  //El primer nodo no esta ready se busca el primero que este en ready
  Process* curr =  linkedlist->head->next;
  while (curr)
  {
    if (curr->state==READY)
    {
      printf("usare la nueva funcion delete!!!!!!!!!!\n");
      cpu = delete_process_pachi(linkedlist, curr);
      printf("Entra a CPU el proceso %s\n", cpu->name);
      int id_fabrica_current = cpu -> id_factory;
      linkedlist->lista_n_procesos_fabricas[cpu->id_factory]--;
      printf("Se elimina de la cola el proceso\n");
      cpu->state = RUNNING;
      printf("el estado del proceso paso a %d\n", cpu->state);
      cpu->t_entrada_cpu=timer;
      calcular_quantum(linkedlist, cpu);
      cpu->next = NULL;
      return cpu;
    }
    curr=curr->next;
  }
  printf("No hay proceso en estado READY en cola\n");
  cpu = NULL;
  return cpu;
}

void waiting_to_ready(LinkedList* linkedlist){
  Process* curr =  linkedlist->head;
  while (curr)
  {
    if (curr->next == NULL){
      printf("el siguiente es nulo\n");
    }
    else{ printf("el siguiente es %s", curr->next->name);}
    if (curr->state==WAITING)
    {     
      printf("\tEl procesos %s esta en espera\n", curr->name);     
      curr->wainting_time++;
      printf("\tEl tiempo en espera aumento en uno\n");      
      int tiempo_en_waiting = timer- curr->tiempo_entrada_waiting;
      printf("\tEl tiempo en waiting es %i\n", tiempo_en_waiting);
      printf("\t El puntero de la rafaga es: %i", curr->puntero_rafaga);
      int bi = curr->rafagas[curr->puntero_rafaga-1];
      printf("\tEl tiempo bi es %i\n", bi);
      if(tiempo_en_waiting == bi ){
        printf("\tLos tiempos son iguales\n");
        curr->state=READY;
        curr->tiempo_entrada_waiting=0;
        printf("\tEl proceso cambio a ready\n");
      } 
    }
    curr=curr->next;
  }
}

// cuando CPU no es null
void cpu_estado(Process* cpu){
  printf("[t = %i] Esta el %s en la CPU en estado %i\n", timer, cpu->name, cpu->state);
  int tiempo_en_cpu = timer - cpu->t_entrada_cpu;
  printf("Tiempo en CPU %i\n", tiempo_en_cpu); 
  if(tiempo_en_cpu == cpu->rafagas[cpu->puntero_rafaga] || cpu->delta == tiempo_en_cpu){//te interrumes poque se acabo ai
    if (cpu->puntero_rafaga + 1 == cpu->numero_rafagas){// es el ultimo ai por lo tanto termina
      printf("[t = %i] %s PASO A ESTADO FINISH\n", timer, cpu->name);
      cpu->state = FINISHED;
      append_linkedlist(terminados, cpu);
      printf("[t = %i] Saldra de cpu_Estado el %s en estado %i\n", timer, cpu->name, cpu->state);
      return;
    }
    cpu->puntero_rafaga++;
    cpu->state = WAITING;
    cpu->tiempo_entrada_waiting=timer;
    printf("[t = %i] %s PASO A ESTADO WAITING\n", timer, cpu->name);
    printf("[t = %i] Saldra de cpu_Estado el %s en estado %i\n", timer, cpu->name, cpu->state);
  }
  //SE CONSUME QUANTUM
  else if(tiempo_en_cpu == cpu->quantum){
    cpu->state = READY;
    //append_linkedlist(linkedlist, cpu);
    int delta = cpu->rafagas[cpu->puntero_rafaga] - cpu->quantum;
    cpu->delta = delta;
    cpu ->veces_interrupcion_quantum++;//agregar el tiempo que se ocupo para las interrupciones
    printf("[t = %i] %s INTERRUPCION!!! (SE CONSUME QUANTUM)\n", timer, cpu->name);
    printf("[t = %i] %s PASO A ESTADO READY\n", timer, cpu->name);
    printf("[t = %i] Saldra de cpu_Estado el %s en estado %i\n", timer, cpu->name, cpu->state);
     //cpu=NULL;

  }
  else{
    printf("[t = %i] %s SIGUE EN RUNNING\n", timer, cpu->name);
    printf("[t = %i] Saldra de cpu_Estado el %s en estado %i\n", timer, cpu->name, cpu->state);
  }
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
    //printf("\tProcess %s from factory %s has init time of %s and %s bursts.\n",line[0], line[2], line[1], line[3]);
    // int n_rafagas = atoi(line[3]);
    // printf("Numero de rafagas %i en el %s\n", n_rafagas, line[0]);
    int pid = i;
    char* name = line[0];
    int id_factory = atoi(line[2]);    
    if (total_factories < id_factory){
      total_factories = id_factory; 
    }    
    int time_start = atoi(line[1]);
    int recorrer = atoi(line[3]) *2 +3;
    Process* process = process_init(pid, name, id_factory, time_start);
    for (int i = 4; i < recorrer; i++){
      int k = i-4;
      process->rafagas[k]= atoi(line[i]);
    }
    process->numero_ai = atoi(line[3]);
    process->numero_rafagas = atoi(line[3]) *2 -1;
    //for (int i = 0; i < recorrer-4; i++){
      //printf("rafaga: %i\n", process->rafagas[i]);
    //}
    lista_procesos[i] = process;
    printf("soy el objeto fabrica pid:%i name:%s factory:%i\n", process->pid, process->name, process->id_factory);
  }

  LinkedList * linkedlist = linkedlist_init(total_factories);
  LinkedList * terminados = linkedlist_init(total_factories);
  //se inicializa con cero
  for (int i = 0; i < total_factories+1; i++){
    linkedlist->f_en_cola[i]=0;
    quantum_factories[i]=0;
  }

  while(true){
    // 1. Revisar si hay algo en cpu
    if (cpu != NULL)
    {
      printf("actualmente en la cpu esta el %s\n", cpu->name);
      cpu_estado(cpu);
    }
    // 2. Procesos creados entran en cola y tambien el que salio de cpu
    // aca de revisa si es tiempo de que llegue a la cola algun proceso
    int len = sizeof(lista_procesos)/sizeof(Process*);
    //printf("------------------------review------------------\n");
    j = revisar_entrada_nuevos_procesos(lista_procesos, lista_procesos_entrando, j, len);
    //printf("------------------------review------------------\n");
    if (cpu!=NULL)
    {
      if(cpu->state == WAITING || cpu->state == READY)
      {
        Process* nuevo = cpu;
        nuevo->next=NULL;
        if(proceso_nuevo){
          printf("NO es nulo cpu\n");
          lista_procesos_entrando[j+1]=nuevo;
          j++;
        }
        else
        {
          printf("J0 es %i\n", j);
          //j=1;
          printf("ENTRE AL ELSE \n");
          printf("nombre nuevo es %s\n", nuevo->name);
          lista_procesos_entrando[j]=nuevo;
          j++;
          //proceso_nuevo = false;
        }
        //printf("Name de la proceso en cpu %s \n",lista_procesos_entrando[j]->name);
        printf("se añadio a lista entrando el proceso que salio de cpu\n");
        
        printf("CPU ahora esta vacio\n");
        cpu=NULL;
      }
    }
    prioridad_procesos(lista_procesos_entrando, j, linkedlist);
    for (int i = 0; i < total_factories+1; i++){
      printf("f en cola %i\n", linkedlist->f_en_cola[i]);
    }
    // 3. No hay proceso en cpu
    printf("HAbrá algo en cpu?");
    if (cpu==NULL)
    {
      printf("No hay nada en Cpu\n");
      printf("HEAD ES %s\n",linkedlist->head->name);
      printf("Cabeza de la lista ligada %s y su estado %i\n", linkedlist->head->name, linkedlist->head->state);
      if(linkedlist->head->next!=NULL){
        printf("siguietne lista ligada  %s\n", linkedlist->head->next->name);
      }
      else{
        printf("No hay siguiente\n");
      }
      cpu = ingresar_cpu(linkedlist, cpu);
    }
    /* 4. Se actualizan las estadisticas de los procesos. Si un proceso 
    salió de cpu, se considera como si hubiera estado en running*/
    
    // 5. Los procesos WAITING que terminaron su I/O Burst (Bi) pasan a READY.
    waiting_to_ready(linkedlist);
        
    global_clock();
  }
}
