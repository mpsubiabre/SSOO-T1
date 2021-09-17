#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "../file_manager/manager.h"
#include "scheduler.h"
//#include "../process/process.h"


int Q;//cambiar a variable de input!!!!!!!!!
int j = 0;
int timer = 0;// variable global
int total_factories = 0 ; 
bool primero = false; //permite saber si es el priemro en entrar a la cola
int numero_fabricas_cola = 0;
int quantum_factories[4];
bool proceso_nuevo = false;
Process* cpu = NULL;
int finish = 0;
  
// Defining comparator function as per the requirement
//fuente: https://www.geeksforgeeks.org/c-program-sort-array-names-strings/
static int myCompare(const void* a, const void* b)
{  
    // setting up rules for comparison
    return strcmp(*(const char**)a, *(const char**)b);
}  
// Function to sort the array
void sort(char* arr[], int n)
{
    // calling qsort function to sort the array
    // with the help of Comparator
    qsort(arr, n, sizeof(const char*), myCompare);
}

void global_clock(){ 
  sleep(1);
  timer++;
}

void calcular_quantum(LinkedList* linkedlist,Process* proceso){
  int id_fabrica = proceso->id_factory;
  int f_quantum = 0;
  int ni_quantum = linkedlist->f_en_cola[id_fabrica];
  id_fabrica++;
  if (ni_quantum == 0){
    ni_quantum++;
  }
  for (int i = 0; i < 4; i++){
    if (linkedlist->f_en_cola[i]>0){
      f_quantum++;
    }
  }
  if (f_quantum < 0 ){
    
  }
  else{
    int multiplicacion;
    multiplicacion =((int) ni_quantum)*((int)f_quantum);
    quantum_factories[id_fabrica] =(int)(Q/multiplicacion); 
    
  }
}

int revisar_entrada_nuevos_procesos(Process** lista_procesos, Process** lista_procesos_entrando, int j, int len ){
  // aca de revisa si es tiempo de que llegue a la cola algun proceso
  //int len = sizeof(lista_procesos)/sizeof(Process*);
  j=0;
  for (int i = 0; i < len; i++){
    if(lista_procesos[i]->time_start == timer){
      printf("[t = %i] se ha creado el proceso %s\n", timer, lista_procesos[i]->name);
      lista_procesos_entrando[j] = lista_procesos[i];
      j++;
      proceso_nuevo = true;
    }
  }
  return j;
}

// algoritmo sacado de internet https://www.lemoda.net/c/array-sort/----------------
/* n_array is the number of elements in the array. */
#define n_array sizeof(array)/sizeof(const char *)
/* Compare the strings. */
static int compare (const void * a, const void * b)
{
    /* The pointers point to offsets into "array", so we need to
       dereference them to get at the strings. */
    return strcmp (*(const char **) a, *(const char **) b);
}//---------------------------------------------

void prioridad_procesos(Process *argv[], int j, LinkedList* linkedlist){
  int j_orog = j;
  if (j == 0){
    return;
  }
  if ((j == 1 && primero == false )|| linkedlist->head==NULL){
    primero = true;
    append_first(linkedlist, argv[0]);
    numero_fabricas_cola ++;
    argv[0]->turnaround_time=timer;
    linkedlist->f_en_cola[argv[0]->id_factory]++;
    linkedlist->lista_n_procesos_fabricas[argv[0]->id_factory]++;
  }
  else if( j == 1 && primero == true){
    append_linkedlist(linkedlist, argv[0]);
    numero_fabricas_cola ++;   
    argv[0]->turnaround_time=timer;
    linkedlist->f_en_cola[argv[0]->id_factory]++;
    linkedlist->lista_n_procesos_fabricas[argv[0]->id_factory]++;
  }
  else{
    int count=0;
    for (int i = 0; i < j; i++){ //WAITING PRIORIDAD
      if (argv[i]->state == WAITING){
        append_linkedlist(linkedlist, argv[i]);
        numero_fabricas_cola ++;
        argv[i]->turnaround_time=timer;
        linkedlist->f_en_cola[argv[i]->id_factory]++;
        linkedlist->lista_n_procesos_fabricas[argv[i]->id_factory]++;
        count++;
      }
    }
    j=j-count;
    if(j==0){
      return;
    }
    count=0;
    // printf("REVISO LOS REDY Y QTM\n");
    for (int i = 0; i < j; i++){ // GANO REDY T QTM
      if (argv[i]->state == READY && argv[i]->quantum == 0){
        append_linkedlist(linkedlist, argv[i]);
        numero_fabricas_cola ++;
        argv[i]->turnaround_time=timer;
        linkedlist->f_en_cola[argv[i]->id_factory]++;
        linkedlist->f_en_cola[argv[i]->id_factory]++;
        linkedlist->lista_n_procesos_fabricas[argv[i]->id_factory]++;
        count++;
      }
    }
    j=j-count;
    if(j==0){
      return;
    }
    Process* lista_same_id_fabrica_0[8];
    Process* lista_same_id_fabrica_1[8];
    Process* lista_same_id_fabrica_2[8];
    Process* lista_same_id_fabrica_3[8];
    for (int i = 0; i < j; i++){
      lista_same_id_fabrica_0[i] = NULL;
      lista_same_id_fabrica_1[i] = NULL;
      lista_same_id_fabrica_2[i] = NULL;
      lista_same_id_fabrica_3[i] = NULL;
     // lista_names_fabrica_0[i] = NULL;
      //lista_names_fabrica_1[i] = NULL;
      //lista_names_fabrica_2[i] = NULL;
      //lista_names_fabrica_3[i] = NULL;
    }
    int i_0=0;
    int i_1=0;
    int i_2=0;
    int i_3=0;
    for (int i = 0; i < j_orog; i++){
      if (argv[i]->id_factory == 0){//aca se guardan los procesos con id de fab 0
        lista_same_id_fabrica_0[i_0] = argv[i];  
 
        i_0++;     
      }
      else if (argv[i]->id_factory == 1){//aca se guardan los procesos con id de fab 1
        lista_same_id_fabrica_1[i_1] = argv[i];
        i_1++; 
      }
      else if (argv[i]->id_factory == 2){//aca se guardan los procesos con id de fab 2
        lista_same_id_fabrica_2[i_2] = argv[i]; 
        i_2++;
      }
      else{                               //aca se guardan los procesos con id de fab 3
        lista_same_id_fabrica_3[i_3] = argv[i];
        i_3++;
      }
    }
    // printf("1. REVISO LOS nombres\n");
    if (i_0>0){
      char * lista_names_fabrica_0[i_0];
      for (int i = 0; i < i_0; i++){
        lista_names_fabrica_0[i] = lista_same_id_fabrica_0[i]->name; 
      }
      qsort (lista_names_fabrica_0, i_0, sizeof (const char *), compare);
      for (int i = 0; i < i_0; i++) {
          for (int j = 0; j < i_0; j++) {
            if (lista_same_id_fabrica_0[j]->name == lista_names_fabrica_0[i]){
              if(primero == false){
                primero = true;
                append_first(linkedlist, lista_same_id_fabrica_0[j]);
                argv[i]->turnaround_time=timer;
              }
              else{
                append_linkedlist(linkedlist, lista_same_id_fabrica_0[j]);
                argv[i]->turnaround_time=timer;
              }
              numero_fabricas_cola ++;
              linkedlist->f_en_cola[lista_same_id_fabrica_0[j]->id_factory]++;
              linkedlist->lista_n_procesos_fabricas[lista_same_id_fabrica_0[j]->id_factory]++;
            }
          }
        }
      }

    if(i_1>0){
      char* lista_names_fabrica_1[i_1];
      for (int i = 0; i < i_1; i++){
        lista_names_fabrica_1[i] = lista_same_id_fabrica_1[i]->name;      
      }
      qsort (lista_names_fabrica_1, i_1, sizeof (const char *), compare);
      for (int i = 0; i < i_1; i++) {
          for (int j = 0; j < i_1; j++) {
            if (lista_same_id_fabrica_1[j]->name == lista_names_fabrica_1[i]){
              if(primero == false){
                primero = true;
                append_first(linkedlist, lista_same_id_fabrica_1[j]);
                lista_same_id_fabrica_1[j]->turnaround_time=timer;
              }
              else{
                append_linkedlist(linkedlist, lista_same_id_fabrica_1[j]);
                lista_same_id_fabrica_1[j]->turnaround_time=timer;
              }
              numero_fabricas_cola ++;
              linkedlist->f_en_cola[lista_same_id_fabrica_1[j]->id_factory]++;
              linkedlist->lista_n_procesos_fabricas[lista_same_id_fabrica_1[j]->id_factory]++;

            }
          }
       }
     }
    if(i_2>0){
      char* lista_names_fabrica_2[i_2];
      for (int i = 0; i < i_2; i++){
        lista_names_fabrica_2[i] = lista_same_id_fabrica_2[i]->name;     
      }
      qsort (lista_names_fabrica_2, i_2, sizeof (const char *), compare);
      for (int i = 0; i < i_2; i++) {
          for (int j = 0; j < i_2; j++) {
            if (lista_same_id_fabrica_2[j]->name == lista_names_fabrica_2[i]){
              if(primero == false){
                primero = true;
                append_first(linkedlist, lista_same_id_fabrica_2[j]);
                lista_same_id_fabrica_2[j]->turnaround_time=timer;

              }
              else{
                append_linkedlist(linkedlist, lista_same_id_fabrica_2[j]);
                lista_same_id_fabrica_2[j]->turnaround_time=timer;
              }
              numero_fabricas_cola ++;
              linkedlist->f_en_cola[lista_same_id_fabrica_2[j]->id_factory]++;
              linkedlist->lista_n_procesos_fabricas[lista_same_id_fabrica_2[j]->id_factory]++;
            }
          }
       }
     }
    if(i_3>0){
      char* lista_names_fabrica_3[i_3];
      for (int i = 0; i < i_3; i++){
        lista_names_fabrica_3[i] = lista_same_id_fabrica_3[i]->name;      
      }
      qsort (lista_names_fabrica_3, i_3, sizeof (const char *), compare);
      for (int i = 0; i < i_3; i++) {
          for (int j = 0; j < i_3; j++) {
            if (lista_same_id_fabrica_3[j]->name == lista_names_fabrica_3[i]){
              if(primero == false){
                primero = true;
                append_first(linkedlist, lista_same_id_fabrica_3[j]);
                lista_same_id_fabrica_3[j]->turnaround_time=timer;
              }
              else{
                append_linkedlist(linkedlist, lista_same_id_fabrica_3[j]);
                lista_same_id_fabrica_3[j]->turnaround_time=timer;
              }
              numero_fabricas_cola ++;
              linkedlist->f_en_cola[lista_same_id_fabrica_3[j]->id_factory]++;
              linkedlist->lista_n_procesos_fabricas[lista_same_id_fabrica_3[j]->id_factory]++;
            }
          }
       }
     }
  }
   for (int i = 0; i < j; i++){
    argv[i] = NULL;
  }
  proceso_nuevo = false;
  }

// cuando CPU es null
Process* ingresar_cpu(LinkedList* linkedlist, Process* cpu){
  if (linkedlist->head==NULL)
  {
    cpu = NULL;
    return cpu;
  }
  
  // Si la El primer proceso esta Ready entra a cpu
  if (linkedlist->head->state == READY)
  {
    cpu = delete_process_pachi(linkedlist,linkedlist->head);
    int id_fabrica_current = cpu -> id_factory;
    linkedlist->f_en_cola[id_fabrica_current]--;
    linkedlist->lista_n_procesos_fabricas[cpu->id_factory]--;

    if(linkedlist->head == NULL){
      primero=false;
    }
    cpu->veces_elegido_cpu++;
    cpu->state = RUNNING;
    printf("[t = %i] El proceso %s ha pasado a estad RUNNING\n", timer, cpu->name);
    //ingresar el tiempo en que entro a cpu
    cpu->t_entrada_cpu=timer;
    if(cpu->response_time==0){
      cpu->response_time=timer-cpu->turnaround_time;
    }
    calcular_quantum(linkedlist, cpu);
    return cpu;
  }
  //El primer nodo no esta ready se busca el primero que este en ready
  Process* curr =  linkedlist->head->next;
  while (curr)
  {
    if (curr->state==READY)
    {
      cpu = delete_process_pachi(linkedlist, curr);
      int id_fabrica_current = cpu -> id_factory;
      linkedlist->f_en_cola[id_fabrica_current]--;
      linkedlist->lista_n_procesos_fabricas[cpu->id_factory]--;
      cpu->state = RUNNING;
      if(cpu->response_time==0){
      cpu->response_time=timer-cpu->turnaround_time;}
      cpu->veces_elegido_cpu++;
      printf("[t = %i] El proceso %s ha pasado a estad RUNNING\n", timer, cpu->name);
      cpu->t_entrada_cpu=timer;
      calcular_quantum(linkedlist, cpu);
      return cpu;
    }
    curr=curr->next;
  }
  // printf("No hay proceso en estado READY en cola\n");
  cpu = NULL;
  return cpu;
}

void waiting_to_ready(LinkedList* linkedlist){
  Process* curr =  linkedlist->head;
  while (curr)
  {
    if (curr->state==WAITING)
    { 
      curr->wainting_time++;    
      int tiempo_en_waiting = timer- curr->tiempo_entrada_waiting;
      int bi = curr->rafagas[curr->puntero_rafaga];
      if(tiempo_en_waiting == bi ){
        curr->state=READY;
        curr->tiempo_entrada_waiting=0;
        printf("[t = %i] El proceso %s ha pasado a estado READY\n", timer, curr->name);
      } 
    }
    if (curr->state==WAITING || curr->state==READY){
      cpu->wainting_time++;
    }
    curr=curr->next;
  }
}

// cuando CPU no es null
void cpu_estado(Process* cpu){
  printf("[t = %i] Esta el %s esta RUNNING\n", timer, cpu->name);
  int tiempo_en_cpu = timer - cpu->t_entrada_cpu;
  cpu->next = NULL;
  if(tiempo_en_cpu == cpu->rafagas[cpu->puntero_rafaga] || cpu->delta == tiempo_en_cpu){//te interrumes poque se acabo ai
    if (cpu->puntero_rafaga + 1 == cpu->numero_rafagas){// es el ultimo ai por lo tanto termina
      printf("[t = %i] el proceso %s ha pasado a estado FINISH\n", timer, cpu->name);
      cpu->state = FINISHED;
      int turn = cpu->turnaround_time;
      cpu->turnaround_time=timer-turn;
      printf("[t = %i] Saldra de cpu_Estado el %s en estado %i\n", timer, cpu->name, cpu->state);
      finish++;
      return;
    }
    cpu->puntero_rafaga++;
    cpu->state = WAITING;
    cpu->wainting_time++;
    cpu->tiempo_entrada_waiting=timer;
    printf("[t = %i] el proceso %s ha pasado a estado WAITING \n", timer, cpu->name);
  }
  //SE CONSUME QUANTUM
  else if(tiempo_en_cpu == cpu->quantum){
    cpu->state = READY;
    cpu->wainting_time++;
    cpu->veces_interrupcion_quantum++;
    //append_linkedlist(linkedlist, cpu);
    int delta = cpu->rafagas[cpu->puntero_rafaga] - cpu->quantum;
    cpu->delta = delta;
    cpu ->veces_interrupcion_quantum++;//agregar el tiempo que se ocupo para las interrupciones
    printf("[t = %i] el proceso %s ha pasado a estado READY\n", timer, cpu->name);
  }
  else{
    printf("[t = %i] el proceso %s esta en estado RUNNING\n", timer, cpu->name);
  }
}


int main(int argc, char **argv)
{
  printf("Hello T2!\n");

  InputFile *file = read_file("input.txt");
  FILE* outputfile = fopen("output_file.csv","w+");

  Process* lista_procesos[file->len];
  Process* lista_procesos_entrando[file->len];
  printf("Ingrese el valor de Q\n");
  scanf("%d", &Q);
  //int Q= atoi(Q);
  printf("El valor de Q es %i\n",Q);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];
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
    lista_procesos[i] = process;
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
      cpu_estado(cpu);
    }
    // 2. Procesos creados entran en cola y tambien el que salio de cpu
    // aca de revisa si es tiempo de que llegue a la cola algun proceso
    int len = sizeof(lista_procesos)/sizeof(Process*);
    j = revisar_entrada_nuevos_procesos(lista_procesos, lista_procesos_entrando, j, len);
    if (cpu!=NULL)
    {
      if(cpu->state == WAITING || cpu->state == READY)
      {
        Process* nuevo = cpu;
        //nuevo->next=NULL;
        if(proceso_nuevo){
          lista_procesos_entrando[j+1]=nuevo;
          j++;
        }
        else
        {
          lista_procesos_entrando[j]=nuevo;
          j++;
        }
        cpu=NULL;
      }
      else if (cpu->state == FINISHED)
      {
        if(terminados->head==NULL){
          append_first(terminados,cpu);
          cpu=NULL;
        }
        else
        {
          append_linkedlist(terminados, cpu);
          cpu=NULL;
        }
      }
      
    }
    prioridad_procesos(lista_procesos_entrando, j, linkedlist);
    j=0;
    if (cpu==NULL)
    {
      cpu = ingresar_cpu(linkedlist, cpu);
    }
    /* 4. Se actualizan las estadisticas de los procesos. Si un proceso 
    salió de cpu, se considera como si hubiera estado en running*/
    print_linkedlist(linkedlist, outputfile);
    // 5. Los procesos WAITING que terminaron su I/O Burst (Bi) pasan a READY.
    waiting_to_ready(linkedlist);
    if(finish==len){
      break;
    }
    global_clock();
  } 
  fclose(outputfile);
}



