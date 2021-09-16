
// Import used global libraries
#pragma once
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.

typedef enum {
  RUNNING,
  READY,
  WAITING,
  FINISHED
  } STATES;

struct process;
typedef struct process Process;

struct process{
    int pid;
    char* name;
    int id_factory;
    STATES state;
    int time_start;
    Process* next;
    int quantum;
    int t_entrada_cpu;
    int veces_interrupcion_quantum;
    int veces_elegido_cpu;
    int puntero_rafaga;
    int numero_ai;
    int numero_rafagas;
    int delta;
    int tiempo_entrada_waiting;
    //tiempo en sistema inicio a fin
    int turnaround_time;
    // tiempo en ready y waiting
    int wainting_time;

    int rafagas[9999];
};

Process* process_init(int pid, char* name, int id_factory, int time_start);
//void process_destroy(Process* process);
