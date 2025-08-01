#include "pcb.h"
#include "kernel.h"
#include <unistd.h>
#include <time.h>
typedef PCB **List;
typedef PCB **Queue;
struct kernel
{
    // verificar qual estrutura utilizar
    List pcb_list;
    int generator_done;
    PCB *current_process;
    int quantum;
    char **log_buffer;
    SchedulerType scheduler_type;
    // verificar qual estrutura utilizar
    Queue runqueue;
};

Kernel *kernel_create(SchedulerType scheduler_type, int quantum)
{
    Kernel *k = (Kernel *)calloc(1, sizeof(Kernel));
    if (!k)
    {
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

void kernel_read_input_file(char *input_path, Kernel *k)
{
    FILE *file = fopen(input_path, "r");
    if (!file)
    {
        perror("Failed to open input file");
        return;
    }
    int nprocesses = 0;
    fscanf(file, "%d", &nprocesses);
    //TODO:Criar lista de processos
    //List process_list;
    // process_list= list_create();
    for (int i = 0; i < nprocesses; i++)
    {
        int process_len, prio, num_threads, start_time;
        fscanf(file, "%d", &process_len);
        fscanf(file, "%d", &prio);
        fscanf(file, "%d", &num_threads);
        fscanf(file, "%d", &start_time);
        PCB *p = process_create(i, process_len, prio, num_threads, start_time);
        print_process(p);
        // TODO
        // kernel_add_process(k, p);
    }
    // kernel_sort_by_start_time(k)
    //
    fscanf(file, "%d", &k->scheduler_type);
}
void kernel_add_process(Kernel *k, PCB *p)
{
    if (!k || !p)
        return;
    // Add process to the PCB list
    // list_add(k->pcb_list, p);
}
void kernel_sort_by_start_time(Kernel *k)
{
}

void kernel_print(Kernel *k)
{
    if (!k)
        return;
    printf("Kernel State:\n");
    printf("Scheduler Type: %d\n", k->scheduler_type);
    printf("Quantum: %d\n", k->quantum);
    printf("Processes:\n");
    // for (int i = 0; i < list_size(k->pcb_list); i++) {
    //     PCB *p = list_get(k->pcb_list, i);
    //     print_process(p);
    // }
}
void kernel_print_output_file(Kernel *k)
{
}

void kernel_schedule(Kernel *k)
{
    //int time=0;
    if (k->scheduler_type == RR)
    {
        kernel_RR_schedule(k);
    }
    else if (k->scheduler_type == FCFS)
    {
        kernel_FCFS_schedule(k);
    }
    else
    {
        kernel_prio_schedule(k);
    }
}

void kernel_RR_schedule(Kernel *k)
{
    // Implement RR
}
void *routine(void * args){
    pthread_mutex_lock(args);
    usleep(500000);//500ms
    
    
    pthread_mutex_unlock(args);
    printf("executando thread");
    return NULL;
}
void kernel_FCFS_schedule(Kernel *k)
{
    //TODO: criar funcao de retornar tamanho da lista de processos
    //int qtt_processes=get_size(k->pcb_list);
    int qtt_processes=3;
    for(int i=0;i<qtt_processes;i++){
        //printf("Executando processo processo PID %d",my_get_pid(k->runqueue[i]));
        
        pthread_t * threads_ids=get_threads_ids(k->pcb_list[i]);
        for(int i=0;i<get_num_threads(k->pcb_list[i]);i++){
            pthread_create(&threads_ids[i],NULL,&routine,NULL);
        }
        for(int i=0;i<get_num_threads(k->pcb_list[i]);i++){
            pthread_join(threads_ids[i],NULL);
        }
    }
}


void kernel_prio_schedule(Kernel *k)
{
    // Implement Priority Scheduling
}
void kernel_destroy(Kernel *k)
{
    if (k)
    {
        // Free PCB list and other resources
        free(k->pcb_list);
        free(k->log_buffer);
        free(k->runqueue);
        free(k);
    }
}