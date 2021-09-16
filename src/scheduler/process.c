
#include "process.h"

Process* process_init(int pid, char* name, int id_factory, int time_start){
    Process* process = malloc(sizeof(Process));
    process->pid = pid;
    process->name = name;
    process->id_factory = id_factory;
    process->state = READY;
    process->next = NULL;
    process->time_start = time_start;
    process->quantum = 0;
    process->t_entrada_cpu = 0;
    process->puntero_rafaga = 0;
    return process;
}