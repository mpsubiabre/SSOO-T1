typedef struct process Process;
struct process{
    int pid;
    char* name;
    int name_factory;
    char state;
    Process* next;
};

Process* process_init(int pid, char* name, int name_factory, char state);
void process_destroy(Process* process);
