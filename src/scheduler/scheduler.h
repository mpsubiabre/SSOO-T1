#pragma once
#include "process.h"

typedef struct linkedlist{
    Process* head;
    Process* tail; 
    int total_factories;
    int f_en_cola[];    
}LinkedList;




LinkedList* linkedlist_init(int total_factories);
void append_linkedlist(LinkedList* linkedlist, Process* process);
void append_first(LinkedList* linkedlist, Process* process);

//void process_destroy(Process* process);
//void iterate_linkedlist( LinkedList* linkedlist);
//Process* pop_process(LinkedList* linkedlist, Process* process);
