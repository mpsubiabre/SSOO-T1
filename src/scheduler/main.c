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
int quantum_factories[];
Process* cpu=NULL;
LinkedList* terminados;


void global_clock(){ 
  printf("pasara una unidad de tiempo....\n");
  sleep(5);
  timer++;
  printf("paso unidad de tiempo, ahora son las %i\n", timer);
  
}

void calcular_quantum(LinkedList* linkedlist,Process* proceso){
  printf("----entre a calcular qum\n");
  int id_fabrica = proceso->id_factory;
  printf(" variables calculo quantum:\n");
  printf("Q: %i\n", Q);
  printf("ni: %i\n", linkedlist->f_en_cola[id_fabrica]);
  printf("f: %i\n", numero_fabricas_cola);
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
      printf("[t = %i] se ha creado el proceso %s\n", timer, lista_procesos[i]->name);
      lista_procesos_entrando[j] = lista_procesos[i];
      j++;
    }
  }
  return j;
}

void prioridad_procesos(Process *argv[], int j, LinkedList* linkedlist){
  printf("entre a funcion prioridad con numero %i de procesos\n", j);
  if (j == 1 && primero == false){
    printf("primerisimo\n");
    primero = true;
    append_first(linkedlist, argv[0]);
    numero_fabricas_cola ++;
    printf("pertenece a la fabrica %i\n", argv[0]->id_factory);
    linkedlist->f_en_cola[argv[0]->id_factory]++;
  }
  else if( j == 1 && primero == true){
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
      else if (argv[i]->state == READY && argv[i]->quantum == 1){
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
  printf("Estado:%d\n",linkedlist->head->state);
  print_linkedlist(linkedlist);
  printf("cola: %s\n",linkedlist->tail->name);
  }
// cuando CPU es null
void ingresar_cpu(LinkedList* linkedlist){

  if (linkedlist->head ==NULL)
  {
    printf("No quedan elementos en la cola\n");
    return;
  }
  // Si la El primer proceso esta Ready entra a cpu
  if (linkedlist->head->state == READY)
  {
    cpu = linkedlist->head;
    printf("Entra a CPU el proceso %s\n", cpu->name);
    delete_process(linkedlist, cpu);
    printf("Se elimina de la cola el proceso\n");
    cpu->state = RUNNING;
    printf("el estado del proceso paso a %d\n", cpu->state);
    //ingresar el tiempo en que entro a cpu
    cpu->t_entrada_cpu=timer;
    calcular_quantum(linkedlist, cpu);
    return;
  }
  //El primer nodo no esta ready se busca el primero que este en ready
  Process* curr =  linkedlist->head->next;
  while (curr)
  {
    if (curr->state==READY)
    {
      cpu = linkedlist->head;
      printf("Entra a CPU el proceso %s\n", cpu->name);
      delete_process(linkedlist, cpu);
      printf("Se elimina de la cola el proceso\n");
      cpu->state = RUNNING;
      printf("el estado del proceso paso a %d\n", cpu->state);
      calcular_quantum(linkedlist);
      return;
    }
    curr=curr->next;
  }
  printf("No hay proceso en estado READY en cola\n");
}

// void waiting_to_ready(LinkedList* linkedlist){
//   Process* curr =  linkedlist->head;
//   while (curr)
//   {
//     if (curr->state==WAITING)
//     {
//       curr->state=READY;
//     }
//     curr=curr->next;
//   }
// }

// cuando CPU no es null
//void cpu_estado(Process* cpu1){
//  if (timer-cpu1->t_entrada_cpu == cpu1->timeburst[numero de interrupcion])
//  {
//    cpu->state == WAITING;
//    printf("[t = %i] ")
//    //poner al final de la cola
//    append_linkedlist(linkedlist, cpu);
//    //CPU debe volver a estar vacio
//    cpu=NULL;
//  }
//  //Se termina de ejecutar el proceso (final de la lista de rafagas)
//  else if (se termina de ejecutar ())
//  {
//     cpu->state = FINISHED;
//     append_linkedlist(terminados, cpu);
//     cpu=NULL;
//  }
//  else if (timer-cpu1->t_entrada_cpu = cpu1->quantum)
//  {
//     cpu->state = READY;
//     append_linkedlist(linkedlist, cpu);
//     // agregar el tiempo que se ocupo para las interrupciones
//     cpu=NULL;
//  }
//  else
//  {
//     //Continua running y se suma 1 al tiempo.
//  }

    
  
// }

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
    printf("\tProcess %s from factory %s has init time of %s and %s bursts.\n",line[0], line[2], line[1], line[3]);
    // int n_rafagas = atoi(line[3]);
    // printf("Numero de rafagas %i en el %s\n", n_rafagas, line[0]);
    int pid = i;
    char* name = line[0];
    int id_factory = atoi(line[2]);
    if (total_factories < id_factory){
      total_factories = id_factory; 
    }
    int time_start = atoi(line[1]);
    // printf("%s \n", line[0]);
    // printf("Time start %i \n",time_start);
    Process* process = process_init(pid, name, id_factory, time_start);
    // //aca se guardan los tiempos Ai, Bi del proceso para iterarlos luego 
    // int k=1;
    // for (int i = 0; i < n_rafagas*2-1; i++)
    // {
    //   //se emipeza a ver la linea despues de la que indica la cantidad.
    //   printf("esta es la rafaga %i con el numero %i\n", i,atoi(line[3+k]));
    //   process->rafagas[i]=atoi(line[3+k]);
    //   k++;
    // }
    // k=0;
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
    if (cpu!=NULL){
      print("Hay un proceso en CPU");
    }
    // 2. Procesos creados entran en cola y tambien el que salio de cpu
    // 3. No hay proceso en cpu
    /* 4. Se actualizan las estadisticas de los procesos. Si un proceso 
    salió de cpu, se considera como si hubiera estado en running*/
    // 5. Los procesos WAITING que terminaron su I/O Burst (Bi) pasan a READY.

    // aca de revisa si es tiempo de que llegue a la cola algun proceso
    int len = sizeof(lista_procesos)/sizeof(Process*);
    printf("------------------------review------------------\n");
    j = revisar_entrada_nuevos_procesos(lista_procesos, lista_procesos_entrando, j, len);
    printf("------------------------review------------------\n");
    
    prioridad_procesos(lista_procesos_entrando, j, linkedlist);
    for (int i = 0; i < total_factories+1; i++){
      printf("f en cola %i\n", linkedlist->f_en_cola[i]);
    }
    if (cpu==NULL)
    {
      printf("No hay nada en Cpu\n");
      ingresar_cpu(linkedlist);
    }
    global_clock();
  }

}
