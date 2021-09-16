#pragma once
#include "process.h"

typedef struct linkedlist{
    Process* head;
    Process* tail; 
    // Cantidad 
    int lista_n_procesos_fabricas[4];
    int total_factories;
    //cantidad de procesos de una fabrica en cola 
    //Ejemplo f_en_cola[1]=4 "Hay cuatro procesos en la fabrica id 1"
    int f_en_cola[];    
}LinkedList;




LinkedList* linkedlist_init(int total_factories);
void append_linkedlist(LinkedList* linkedlist, Process* process);
void append_first(LinkedList* linkedlist, Process* process);
void print_linkedlist(LinkedList* linkedlist);
void destroy_linkedlist(LinkedList* linkedlist);
//Esta funcion elimina un proceso
void delete_process(LinkedList* linkedlist ,Process* process);
Process* delete_process_pachi(LinkedList* linkedlist ,Process* process);
//void iterate_linkedlist( LinkedList* linkedlist);
//Process* pop_process(LinkedList* linkedlist, Process* process);
