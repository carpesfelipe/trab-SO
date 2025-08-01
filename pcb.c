#include "pcb.h"
#include "tcb.h"
struct pcb{ 
    int pid; 
    int process_len; 
    int remaining_time; 
    int priority; 
    int num_threads; 
    int start_time; 
    ProcessState state; 
    pthread_mutex_t mutex; 
    pthread_cond_t cv; 
    pthread_t *thread_ids; 
}; 

PCB *process_create(int pid, int process_len, int prio, int num_threads, int start_time)
{
    PCB *p = (PCB *)calloc(1, sizeof(PCB));
    if (!p) {
        perror("Failed to allocate memory for PCB");
        return NULL;
    }
    p->pid = pid;
    p->process_len = process_len;
    p->remaining_time = process_len;
    p->priority = prio;
    p->num_threads = num_threads;
    p->start_time = start_time;
    p->state = READY; // Initial state
    pthread_mutex_init(&p->mutex, NULL);
    pthread_cond_init(&p->cv, NULL);
    p->thread_ids = (pthread_t *)calloc(num_threads, sizeof(pthread_t));
    if (!p->thread_ids) {
        perror("Failed to allocate memory for thread IDs");
        free(p);
        return NULL;
    }
    return p;
}

void print_process(PCB *p)
{
    if (!p) return;
    printf("Process ID: %d\n", p->pid);
    printf("Process Length: %d\n", p->process_len);
    printf("Remaining Time: %d\n", p->remaining_time);
    printf("Priority: %d\n", p->priority);
    printf("Number of Threads: %d\n", p->num_threads);
    printf("Start Time: %d\n", p->start_time);
    printf("State: %d\n", p->state);
}
void process_destroy(PCB *p)
{
    if (!p) return;
    pthread_mutex_destroy(&p->mutex);
    pthread_cond_destroy(&p->cv);
    free(p->thread_ids);
    free(p);
}
int get_remaining_time(PCB *p)
{
    return p->remaining_time;
}
int get_priority(PCB *p)
{
        return p->priority;

}
int get_num_threads(PCB *p)
{
        return p->num_threads;

}
int my_get_pid(PCB *p)
{
        return p->pid;

}
int get_start_time(PCB *p)
{
        return p->start_time;

}
pthread_t * get_threads_ids(PCB* p){
    return p->thread_ids;
}
