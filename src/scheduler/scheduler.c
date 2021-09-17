#include "scheduler.h"


LinkedList* linkedlist_init(int total_factories){
    LinkedList* linkedlist = malloc(sizeof(LinkedList));
    //linkedlist->head = NULL;
    //linkedlist->tail = NULL;
    linkedlist-> total_factories = total_factories;
    for (int i = 0; i < 4; i++){
        linkedlist -> lista_n_procesos_fabricas[i] = 0;
    }
    return linkedlist;
}

void append_linkedlist(LinkedList* linkedlist, Process* process){
    process->next = NULL;
    linkedlist->tail->next = process;
    linkedlist->tail = process;    
}

void append_first(LinkedList* linkedlist, Process* process){
    process->next = NULL;
    linkedlist->head = process;
    //linkedlist->tail->next = NULL;
    linkedlist->tail = process;
}

void print_linkedlist(LinkedList* linkedlist, FILE* output){
    for (Process* current = linkedlist ->head; current; current=current ->next)
    {
        fprintf(output, "%s,%i,%i,%i,%i,%i\n", current->name, current->veces_elegido_cpu, current->veces_interrupcion_quantum,current->turnaround_time,current->response_time,current->wainting_time);
    }
}

void destroy_linkedlist(LinkedList* linkedlist)
{
    //liberar los procesos
    if (linkedlist->head)
    {
        Process* curr = linkedlist->head->next;
        Process* prev = linkedlist->head;
        while (curr)
        {
            free(prev);
            prev = curr;
            curr = curr ->next;
        }
        free(prev);
    }
    free(linkedlist);
}

Process* delete_process_pachi(LinkedList* linkedlist ,Process* current){
    if(linkedlist->head == current)
    {
        if(linkedlist->head->next == NULL)//SOLO HAY UN NODO
        {
           linkedlist->head = NULL; //la dejo vacia
           current->next = NULL;
           return current; 
        }
        //hay mas de un nodo
        linkedlist->head = linkedlist->head->next;
        current->next = NULL;
        return current;      
    }
    // No es el primer nodo
    Process* nodo = linkedlist->head;
    while (nodo->next != NULL && nodo->next!=current)
    {   
        nodo = nodo->next;
    }
    if(nodo->next == NULL){
        //NO esta presente en la lista

        return current;
    }
    nodo->next = nodo->next->next;
    current->next = NULL;
    return current;
}
