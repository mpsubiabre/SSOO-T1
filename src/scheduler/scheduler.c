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
    //printf("ESTOY AÑADIENDO %s\n", process->name);
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

void print_linkedlist(LinkedList* linkedlist){
    for (Process* current = linkedlist ->head; current; current=current ->next)
    {
        printf("%s\n", current->name);
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
    //printf("entre a la funcion delete process con current %s , cabeza %s y cola %s\n", current-> name, linkedlist->head->name, linkedlist->tail->name);
    // es el primer nodo de la lista
    if(linkedlist->head == current)
    {
        //printf("es el primer nodo de la lista\n");
        if(linkedlist->head->next == NULL)//SOLO HAY UN NODO
        {
           //printf("solo hay un nodo\n");
           linkedlist->head = NULL; //la dejo vacia
           current->next = NULL;
           return current; 
        }
        //hay mas de un nodo
        //printf("hay mas de un nodo\n");
        
        linkedlist->head = linkedlist->head->next;
        //printf("New head %s\n", linkedlist->head->name);
        // for (Process* current = linkedlist->head; current=current->next)
        // {
            
        // }
        
        // // la cabeza ahora es el hermano de la cabeza original
        // if(linkedlist->head->next!=NULL){
        //     printf("Next %s\n", linkedlist->head->next->name);
        //     linkedlist->head->next=linkedlist->head->next->next;    
        // } 
        // else
        // {
        //     linkedlist->head->next=NULL;
        // }
         
        current->next = NULL;
        return current;      
    }
    // No es el primer nodo
    //printf("no es el primer nodo\n");
    Process* nodo = linkedlist->head;
    while (nodo->next != NULL && nodo->next!=current)
    {   
        //printf("entre al while\n");
        nodo = nodo->next;
    }
    //printf("sali del while\n");
    if(nodo->next == NULL){
        //NO esta presente en la lista

        return current;
    }
    nodo->next = nodo->next->next;
    //printf("Next %s", nodo->next->name);
    //free(current); maybe free nodo
    current->next = NULL;
    return current;
}
