#include "scheduler.h"


LinkedList* linkedlist_init(){
    LinkedList* linkedlist = malloc(sizeof(LinkedList));
    //linkedlist->head = NULL;
    //linkedlist->tail = NULL;
    return linkedlist;
}

void append_linkedlist(LinkedList* linkedlist, Process* process){
    linkedlist->tail->next = process;
    linkedlist->tail = process;    
}

void append_first(LinkedList* linkedlist, Process* process){
    printf("apprend firrst\n");
    linkedlist->tail->next = process;
    linkedlist->tail = process;
    linkedlist->head = process;
}



