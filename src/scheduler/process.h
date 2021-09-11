
// Import used global libraries
#pragma once
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.

struct process;
typedef struct process Process;

struct process{
    int pid;
    char* name;
    int name_factory;
    char* state;
    Process* next;
};

Process* process_init(int pid, char* name, int name_factory);
//void process_destroy(Process* process);
