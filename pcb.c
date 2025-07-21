#include "pcb.h"
#define READY 1
#define RUNNING 2
#define READY 3

typedef struct { 
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
} PCB; 

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

int get_tempo_execucao(PCB *p)
{

}
int get_prio(PCB *p)
{
}
int get_qtdThreads(PCB *p)
{
}
int meu_get_pid(PCB *p)
{
}
int get_tempo_chegada(PCB *p)
{
}