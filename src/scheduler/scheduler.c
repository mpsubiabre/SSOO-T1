#include "scheduler.h"

void append_linkedlist(LinkedList* linkedlist, Process* process){
    linkedlist->fin->next = process;
    linkedlist->fin = process;
}

