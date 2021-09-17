
#include "process.h"

Process* process_init(int pid, char* name, int id_factory, int time_start){
    Process* process = malloc(sizeof(Process));
    process->pid = pid;
    process->name = name;
    process->id_factory = id_factory;
    process->state = READY;
    process->next = NULL;
    process->time_start = time_start;
    process->quantum = -1;
    process->t_entrada_cpu = 0;
    process->puntero_rafaga = 0;
    process->tiempo_entrada_waiting = 0;
    process->response_time=0;
    return process;
}

void print_proces(Process* process, int timer){
    switch (process->state)
    {
    case RUNNING:
        printf("[t = %i] El proceso %s ha pasado a RUNNING",timer, process->name);
        break;
    case READY:
        printf("[t = %i] El proceso %s ha pasado a READY",timer, process->name);
        break;
    case WAITING:
        printf("[t = %i] El proceso %s ha pasado a WAITING",timer, process->name);
        break;
    case FINISHED:
        printf("[t = %i] El proceso %s ha pasado a FINISHED",timer, process->name);
        break;
    }
}
