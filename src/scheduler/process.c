
#include "process.h"


Process* process_init(int pid, char* name, int name_factory){
    Process* process = malloc(sizeof(Process));
    process->pid = pid;
    process->name = name;
    process->name_factory = name_factory;
    process->state = NULL;
    process->next = NULL;
    return process;
}