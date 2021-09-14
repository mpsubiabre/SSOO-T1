#include "scheduler.h"


LinkedList* linkedlist_init(int total_factories){
    LinkedList* linkedlist = malloc(sizeof(LinkedList));
    //linkedlist->head = NULL;
    //linkedlist->tail = NULL;
    total_factories = total_factories;
    return linkedlist;
}

void append_linkedlist(LinkedList* linkedlist, Process* process){
    linkedlist->tail->next = process;
    linkedlist->tail = process;    
}

void append_first(LinkedList* linkedlist, Process* process){
    linkedlist->tail->next = process;
    linkedlist->tail = process;
    linkedlist->head = process;
}



