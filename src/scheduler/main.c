#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../file_manager/manager.h"
#include "scheduler.h"
//#include "../process/process.h"

typedef enum {
  RUNNING,
  READY,
  WAITING,
  FINISHED
  } STATES;


int main(int argc, char **argv)
{
  printf("Hello T2!\n");

  InputFile *file = read_file("input.txt");

  printf("Reading file of length %i:\n", file->len);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];
    printf(
        "\tProcess %s from factory %s has init time of %s and %s bursts.\n",
        line[0], line[2], line[1], line[3]);
        int pid = i;
        int name_factory = atoi(line[1]);
        Process* process = process_init(pid, line[0], name_factory);
        printf("soy el objeto fabrica pid:%i name:%s factory:%i\n", process->pid, process->name, process->name_factory);
  }
}
