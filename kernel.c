#include "pcb.h"
#include "kernel.h"

typedef PCB** List;
typedef PCB** Queue;
struct kernel
{
    //verificar qual estrutura utilizar
    List pcb_list;
    int generator_done;
    PCB *current_process;
    int quantum;
    char ** log_buffer;
    SchedulerType scheduler_type;
    //verificar qual estrutura utilizar
    Queue runqueue;    
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
    FILE *file = fopen(input_path, "r");
    if (!file) {
        perror("Failed to open input file");
        return;
    }
    int nprocesses = 0;
    fscanf(file, "%d", &nprocesses);
    k=kernel_create(0, QUANTUM); // Create kernel with default values
    List process_list;
    //process_list= list_create();
    for(int i=0;i<nprocesses;i++){
        int process_len, prio, num_threads, start_time;
        fscanf(file,"%d",&process_len);
        fscanf(file,"%d",&prio);
        fscanf(file,"%d",&num_threads);
        fscanf(file,"%d",&start_time);
        PCB *p=process_create(i, process_len, prio, num_threads, start_time);
        print_process(p);
        //kernel_add_process(k, p);
    }
    fscanf(file,"%d",&k->scheduler_type);
}
void kernel_add_process(Kernel *k, PCB *p) {
    if (!k || !p) return;
    // Add process to the PCB list
    //list_add(k->pcb_list, p);
}

void kernel_print(Kernel *k) {
    if (!k) return;
    printf("Kernel State:\n");
    printf("Scheduler Type: %d\n", k->scheduler_type);
    printf("Quantum: %d\n", k->quantum);
    printf("Processes:\n");
    // for (int i = 0; i < list_size(k->pcb_list); i++) {
    //     PCB *p = list_get(k->pcb_list, i);
    //     print_process(p);
    // }

}
void kernel_print_output_file(Kernel *k){
    
}

void kernel_schedule(Kernel *k){
    if(k->scheduler_type==RR){
        round_robin_schedule(k);
    }else if(k->scheduler_type==FCFS){
        FCFS_schedule(k);
    }else{
        prio_schedule(k);
    }
}

void round_robin_schedule(Kernel *k) {
    // Implement Round Robin scheduling logic
}
void FCFS_schedule(Kernel *k) {
    // Implement First-Come, First-Served scheduling logic
}
void prio_schedule(Kernel *k) {
    
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