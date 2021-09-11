#pragma once
#include "../process/process.h"
#include "../scheduler/scheduler.h"

typedef struct linkedlist{
    Process* inicio;
    Process* fin; 
}LinkedList;

void append_linkedlist(LinkedList* linkedlist, Process* process);


Process* process_init(int pid, char* name, int name_factory, char state);
void process_destroy(Process* process);
void iterate_linkedlist( LinkedList* linkedlist);
Process* pop_process(LinkedList* linkedlist, Process* process);
