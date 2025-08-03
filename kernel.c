#define _GNU_SOURCE // Define esta macro para habilitar extensões GNU

#include "pcb.h"
#include "kernel.h"
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"

typedef PCB **List;
// typedef PCB **Queue;

#define LOG_INITIAL_CAPACITY 15

struct kernel
{
    // verificar qual estrutura utilizar
    PCB **pcb_list;
    int nprocesses;

    int generator_done;
    PCB *current_process;
    int quantum;
    char **log_buffer;
    int log_count;
    int log_capacity;

    SchedulerType scheduler_type;
    // verificar qual estrutura utilizar
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
    k->pcb_list = NULL; // Initialize the PCB list
    k->generator_done = 0;
    k->current_process = NULL;
    k->quantum = quantum;
    // k->log_buffer = NULL; // Initialize log buffer
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

        k->pcb_list[i] = process_create(i, process_len, prio, num_threads, start_time);
        // print_process(k->pcb_list[i]);
    }
    kernel_sort_by_start_time(k);

    // printf("Processos ordenados pelo start time:\n");
    // for(int i=0; i < nprocesses; i++) {
    //     print_process(k->pcb_list[i]);
    // }

    fscanf(file, "%d", &k->scheduler_type);

    k->runqueue = queue_create(k->nprocesses);

    fclose(file);
}

// void kernel_add_process(Kernel *k, PCB *p)
// {
//     if (!k || !p)
//         return;
//     // Add process to the PCB list
//     // list_add(k->pcb_list, p);
// }
void kernel_sort_by_start_time(Kernel *k)
{
    if (k && k->pcb_list && k->nprocesses > 1) {
        qsort(k->pcb_list, k->nprocesses, sizeof(PCB *), compare_pcb_start_time);
    }
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
        // printf("Scheduling using Round Robin\n");
        kernel_RR_schedule(k);
    }
    else if (k->scheduler_type == FCFS)
    {
        // printf("Scheduling using First-Come, First-Served\n");
        kernel_FCFS_schedule(k);
    }
    else
    {
        // printf("Scheduling using Priority Scheduling\n");
        kernel_prio_schedule(k);
    }
}

void kernel_RR_schedule(Kernel *k)
{
    // Implement RR
}
void *routine(void * args){
    if(!args) {
        printf("No arguments provided to routine.\n");
        return NULL;
    }

    pthread_mutex_lock(args);
    usleep(500000);//500ms
    
    
    pthread_mutex_unlock(args);
    // printf("executando thread");
    return NULL;
}
// void kernel_FCFS_schedule(Kernel *k)
// {
//     printf("Iniciando escalonamento FCFS...\n");

//     //TODO: criar funcao de retornar tamanho da lista de processos
//     int qtt_processes=get_size(k->pcb_list);
//     // int qtt_processes=3;
//     for(int i=0;i<qtt_processes;i++){
//         //printf("Executando processo processo PID %d",my_get_pid(k->runqueue[i]));
        
//         pthread_t * threads_ids=get_threads_ids(k->pcb_list[i]);
//         for(int i=0;i<get_num_threads(k->pcb_list[i]);i++){
//             pthread_create(&threads_ids[i],NULL,&routine,NULL);
//         }
//         for(int i=0;i<get_num_threads(k->pcb_list[i]);i++){
//             pthread_join(threads_ids[i],NULL);
//         }
//     }
// }

void kernel_FCFS_schedule(Kernel *k){    
    // O escalonador deve atuar enquanto houver processos na fila de prontos
    while (!queue_empty(k->runqueue))
    {
        // pega o primeiro processo da fila de prontos
        PCB *current_process = queue_remove(k->runqueue);
        if (current_process == NULL) {
            printf("Fila de prontos vazia. Encerrando escalonador.\n");
            break;
        }

        int pid = my_get_pid(current_process);
        int num_threads = get_num_threads(current_process);

        // printf("[FCFS] Executando processo PID %d\n", pid+1);
        add_log_entry(k, "[FCFS] Executando processo PID %d", pid + 1);

        // Aloca o array para os IDs das threads dentro do PCB.
        pthread_t *threads_ids = get_threads_ids(current_process); // Obtém o array de threads do PCB
        if (!threads_ids) {
            perror("Failed to get thread_ids from PCB");
            continue;
        }

        // Obtém o mutex do PCB para passar para as threads
        pthread_mutex_t *mutex = get_pcb_mutex(current_process); 
        if (!mutex) {
            perror("Failed to get mutex from PCB");
            continue;
        }

        // Cria as threads do processo
        for (int i = 0; i < num_threads; i++){
            pthread_create(&threads_ids[i], NULL, &routine, (void *)mutex);
        }
        
        // Espera todas as threads do processo terminarem.
        for (int i = 0; i < num_threads; i++){
            pthread_join(threads_ids[i], NULL);
            // printf("[FCFS] Thread %d do processo PID %d finalizada\n", i + 1, pid + 1);
        }

        // printf("[FCFS] Processo PID %d finalizado\n", pid + 1);
        add_log_entry(k, "[FCFS] Processo PID %d finalizado", pid + 1);

        // pthread_mutex_unlock(mutex);
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
        if (k->pcb_list) {
            for (int i = 0; i < k->nprocesses; i++) {
                process_destroy(k->pcb_list[i]);
            }
            free(k->pcb_list);
        }

        if (k->log_buffer) {
            for (int i = 0; i < k->log_count; i++) {
                free(k->log_buffer[i]);
            }
            free(k->log_buffer);
        }

        queue_destroy(k->runqueue);

        // free(k->log_buffer);
        free(k);
    }
}

void kernel_run_simulation(Kernel *k){ 
    int current_time = 0;
    int process_index = 0;
    // int finished_processes = 0;

    while (process_index < k->nprocesses || !queue_empty(k->runqueue)) {
        // Itera sobre a lista de processos ordenada e adiciona os que chegaram à fila de prontos
        while (process_index < k->nprocesses && get_start_time(k->pcb_list[process_index]) <= current_time) {
            queue_add(k->runqueue, k->pcb_list[process_index]);
            process_index++;
        }

        // Se a fila de prontos não estiver vazia, o escalonador age
        if (!queue_empty(k->runqueue)) {
            kernel_schedule(k);
        }

        // SIMULA incremento do tempo
        current_time++;

    }
    kernel_print_log(k);
    printf("Escalonador terminou execução de todos processos\n");
}

void kernel_printa_runqueue(Kernel *k) {
    if (k && k->runqueue) {
        queue_print(k->runqueue);
    } else {
        printf("Runqueue is not initialized.\n");
    }
}

void add_log_entry(Kernel *k, const char *format, ...) {
    if (!k) return;

    // Redimensiona o buffer se estiver cheio
    if (k->log_count >= k->log_capacity) {
        k->log_capacity *= 2;
        k->log_buffer = (char **)realloc(k->log_buffer, k->log_capacity * sizeof(char *));
        if (!k->log_buffer) {
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

    if (log_entry) {
        k->log_buffer[k->log_count++] = log_entry;
    }
}

void kernel_print_log(Kernel *k) {
    if (!k || !k->log_buffer) return;

    for (int i = 0; i < k->log_count; i++) {
        printf("%s\n", k->log_buffer[i]);
    }
}