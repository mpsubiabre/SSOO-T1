#pragma once
#include "process.h"

typedef struct linkedlist{
    Process* head;
    Process* tail; 
}LinkedList;




LinkedList* linkedlist_init();
void append_linkedlist(LinkedList* linkedlist, Process* process);

//void process_destroy(Process* process);
//void iterate_linkedlist( LinkedList* linkedlist);
//Process* pop_process(LinkedList* linkedlist, Process* process);
