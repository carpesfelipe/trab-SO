#define _GNU_SOURCE // Define esta macro para habilitar extensões GNU

#include "pcb.h"
#include "kernel.h"
#include "tcb.h"
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/time.h>
#include "queue.h"

// typedef PCB **Queue;
typedef struct
{
    TCB *tcb;
    Kernel *k;
} ThreadArgs;
#define LOG_INITIAL_CAPACITY 15

struct kernel
{
    char **log_buffer;
    PCB **pcb_list;
    int nprocesses;
    int generator_done;
    PCB *current_process;
    int quantum;
    int log_count;
    int log_capacity;
    SchedulerType scheduler_type;
    Queue *runqueue;
};

Kernel *kernel_create(SchedulerType scheduler_type, int quantum)
{
    Kernel *k = (Kernel *)calloc(1, sizeof(Kernel));
    if (!k)
    {
        perror("Failed to allocate memory for Kernel");
        return NULL;
    }
    k->pcb_list = NULL;
    k->generator_done = 0;
    k->current_process = NULL;
    k->quantum = quantum;
    k->log_buffer = (char **)calloc(LOG_INITIAL_CAPACITY, sizeof(char *));
    k->log_count = 0;
    k->log_capacity = LOG_INITIAL_CAPACITY;
    if (!k->log_buffer)
    {
        perror("Failed to allocate memory for log buffer");
        free(k);
        return NULL;
    }

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
    k->nprocesses = nprocesses;

    k->pcb_list = (PCB **)calloc(nprocesses, sizeof(PCB *));

    if (!k->pcb_list)
    {
        perror("Failed to allocate memory for PCB list");
        fclose(file);
        return;
    }

    for (int i = 0; i < nprocesses; i++)
    {
        int process_len, prio, num_threads, start_time;
        fscanf(file, "%d", &process_len);
        fscanf(file, "%d", &prio);
        fscanf(file, "%d", &num_threads);
        fscanf(file, "%d", &start_time);

        k->pcb_list[i] = process_create(i + 1, process_len, prio, num_threads, start_time);
    }
    kernel_sort_by_start_time(k);

    fscanf(file, "%d", &k->scheduler_type);

    k->runqueue = queue_create(k->nprocesses);

    fclose(file);
}

void kernel_sort_by_start_time(Kernel *k)
{
    if (k && k->pcb_list && k->nprocesses > 1)
    {
        qsort(k->pcb_list, k->nprocesses, sizeof(PCB *), compare_pcb_start_time);
    }
}

void kernel_RR_schedule(Kernel *k)
{
    // Implement RR
}
void *routine(void *args)
{
    ThreadArgs *arg = (ThreadArgs *)args;
    Kernel *k = arg->k;
    TCB *tcb = arg->tcb;
    free(arg);
    PCB *p = tcb_get_process(tcb);
    pthread_mutex_t *mutex = get_pcb_mutex(p);
    pthread_cond_t *cv = pcb_get_cv(p);
    //Executa até o estado do processo ser finished
   while (1) {
        pthread_mutex_lock(mutex);
        //Se o processo não estiver no estado running ou finished, a thread deve se bloquear até estar no estado running
        //A alteração do processo de RUNNING para READY só ocorre em kerne_priority_schedule
        while (pcb_get_state(p) != RUNNING && pcb_get_state(p) != FINISHED) {
            pthread_cond_wait(cv, mutex);
        }
        //se o processo finalizou, o mutex é liberado e "encerra" a thread
        if (pcb_get_state(p) == FINISHED) {
            pthread_mutex_unlock(mutex);
            break;
        }

        int remaining = get_remaining_time(p);
        //se uma thread já alcançou o remaining_time==0 o estado ja foi alterado para finished e as threads não
        //precisam mais olhar o resto, mas voltam para verificar o estado do processo antes de encerrar
        if (remaining <= 0) {
            pthread_mutex_unlock(mutex);
            continue; 
        }

        int time_to_consume = k->quantum;
        if (remaining < time_to_consume) {
            time_to_consume = remaining;
        }
        sub_remaining_time(p, time_to_consume);
        
        printf("[PID %d] executou thread %d por %dms, faltam %dms\n", 
               my_get_pid(p), 
               tcb_get_thread_index(tcb), 
               time_to_consume, 
               get_remaining_time(p)); 

        pthread_mutex_unlock(mutex);
        usleep(time_to_consume * 1000); 
        
        pthread_mutex_lock(mutex);
        if (get_remaining_time(p) <= 0 && pcb_get_state(p) != FINISHED) {
            pcb_change_state(p, FINISHED);
            add_log_entry(k, "[FCFS] Processo PID %d finalizado", my_get_pid(p));
            pthread_cond_broadcast(cv);// Acorda outras threads para que elas vejam o FINISHED e saiam
        }
        pthread_mutex_unlock(mutex);
    }
    free(tcb);
    return NULL;
}


void kernel_FCFS_schedule(Kernel *k) {
    //se nao tem nenhum processo na CPU e a fila de prontos nao esta vazia
    if (k->current_process == NULL && !queue_empty(k->runqueue)) {
        
        // Pega o próximo processo da fila
        PCB *next_process = queue_remove(k->runqueue);
        k->current_process = next_process;        
        pthread_mutex_t *mutex = get_pcb_mutex(k->current_process);
        pthread_cond_t *cv = pcb_get_cv(k->current_process);
        //Uma vez que o processo está estado RUNNING ele não volta pra ready pois não há preempcao
        pthread_mutex_lock(mutex);
        pcb_change_state(k->current_process, RUNNING);
        // Sinaliza para as threads do processo começarem a executar
        pthread_cond_broadcast(cv); 
        pthread_mutex_unlock(mutex);
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
        if (k->pcb_list)
        {
            for (int i = 0; i < k->nprocesses; i++)
            {
                process_destroy(k->pcb_list[i]);
            }
            free(k->pcb_list);
        }

        if (k->log_buffer)
        {
            for (int i = 0; i < k->log_count; i++)
            {
                free(k->log_buffer[i]);
            }
            free(k->log_buffer);
        }

        queue_destroy(k->runqueue);

        // free(k->log_buffer);
        free(k);
    }
}

void kernel_run_simulation(Kernel *k)
{
    int process_index = 0;
    int finished_processes = 0;
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    //todas as threads são criadas mas só executam quando sinalizado pelo escalonador
    for (int i = 0; i < k->nprocesses; i++)
    {
        PCB *p = k->pcb_list[i];
        int num_threads = get_num_threads(p);
        pthread_t *threads_ids = get_threads_ids(p);
        for (int j = 0; j < num_threads; j++)
        {
            ThreadArgs *args = calloc(1, sizeof(ThreadArgs));
            args->k = k;
            args->tcb = tcb_create(p, j);
            pthread_create(&threads_ids[j], NULL, &routine, args);
        }
    }

    //Enquanto todos os processos não estiverem no estaod finalizado
    while (finished_processes < k->nprocesses) {
        //adiciona processos na fila de prontos enquanto todos os processos não foram adicionados e 
        //o tempo alcançou o start_time do processo
        while (process_index < k->nprocesses && get_start_time(k->pcb_list[process_index]) <= get_current_time(start_time)) {
            PCB *p = k->pcb_list[process_index];
            queue_add(k->runqueue, p);
            process_index++;
        }

        //verifica se o processo atual terminou
        if (k->current_process != NULL && pcb_get_state(k->current_process) == FINISHED) {
            k->current_process = NULL; 
            finished_processes++;
        }

        if (k->scheduler_type == FCFS) {
            kernel_FCFS_schedule(k);
        } else if (k->scheduler_type == RR) {
            // kernel_RR_schedule(k);
        } else {
            // kernel_prio_schedule(k);
        }
        //Simula a passagem de tempo para que o loop não ocorra várias vezes sem acontecer nada
        usleep(10000); 
    }
    // Espera todas as threads terminarem antes de sair
    for (int i = 0; i < k->nprocesses; i++) {
        PCB* p = k->pcb_list[i];
        int num_threads = get_num_threads(p);
        pthread_t* threads_ids = get_threads_ids(p);
        for (int j = 0; j < num_threads; j++) {
            pthread_join(threads_ids[j], NULL);
        }
    }
    kernel_print_log(k);

    printf("Escalonador terminou execução de todos processos\n");
}

void kernel_printa_runqueue(Kernel *k)
{
    if (k && k->runqueue)
    {
        queue_print(k->runqueue);
    }
    else
    {
        printf("Runqueue is not initialized.\n");
    }
}

void add_log_entry(Kernel *k, const char *format, ...)
{
    if (!k)
        return;

    // Redimensiona o buffer se estiver cheio
    if (k->log_count >= k->log_capacity)
    {
        k->log_capacity *= 2;
        k->log_buffer = (char **)realloc(k->log_buffer, k->log_capacity * sizeof(char *));
        if (!k->log_buffer)
        {
            perror("Failed to reallocate log buffer");
            exit(EXIT_FAILURE);
        }
    }

    // Formata a string de log e a adiciona ao buffer
    va_list args;
    va_start(args, format);
    char *log_entry;
    vasprintf(&log_entry, format, args);
    va_end(args);

    if (log_entry)
    {
        k->log_buffer[k->log_count++] = log_entry;
    }
}

void kernel_print_log(Kernel *k)
{
    if (!k || !k->log_buffer)
        return;

    for (int i = 0; i < k->log_count; i++)
    {
        printf("%s\n", k->log_buffer[i]);
    }
}

int get_current_time(struct timeval start_time)
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    long elapsed_time_ms = (current_time.tv_sec - start_time.tv_sec) * 1000 +
                           (current_time.tv_usec - start_time.tv_usec) / 1000;
    return (int)elapsed_time_ms;
}