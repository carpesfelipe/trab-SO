#include "pcb.h"
#include "kernel.h"
struct kernel
{
    PCB ** pcb_list;
    int generator_done;
    PCB *current_process;
    int quantum;
    char ** log_buffer;
    SchedulerType scheduler_type;
    PCB ** runqueue;    
};

Kernel *kernel_create(SchedulerType scheduler_type, int quantum)
{
    Kernel *k = (Kernel *)calloc(1, sizeof(Kernel));
    if (!k) {
        perror("Failed to allocate memory for Kernel");
        return NULL;
    }
    k->pcb_list = NULL; // Initialize the PCB list
    k->generator_done = 0;
    k->current_process = NULL;
    k->quantum = quantum;
    k->log_buffer = NULL; // Initialize log buffer
    k->scheduler_type = scheduler_type;
    k->runqueue = NULL; // Initialize runqueue
    return k;
}

void kernel_read_input_file(char * input_path,Kernel *k){
    
}
void kernel_print_input_file(Kernel *k){
    
}
void kernel_schedule(Kernel *k){
    
}
void kernel_destroy(Kernel *k)
{
    if (k) {
        // Free PCB list and other resources
        free(k->pcb_list);
        free(k->log_buffer);
        free(k->runqueue);
        free(k);
    }
}