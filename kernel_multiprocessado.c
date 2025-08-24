#define _GNU_SOURCE // Define esta macro para habilitar extensões GNU

#include "pcb.h"
#include "kernel_multiprocessado.h"
#include "tcb.h"
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/time.h>
#include "queue.h"
#include <string.h>
// typedef PCB **Queue;
typedef struct
{
    TCB *tcb;
    Kernel *k;
} ThreadArgs;
#define LOG_INITIAL_CAPACITY 100

struct kernel
{
    char **log_buffer;
    pthread_mutex_t mutex_queue;
    pthread_mutex_t mutex_log;
    PCB **pcb_list;
    int nprocesses;
    int generator_done;
    //processo que está utilizando a CPU no momento
    PCB *current_process[2];
    int quantum;
    int log_count;
    int log_capacity;
    SchedulerType scheduler_type;
    Queue *runqueue;
};

Kernel *multi_kernel_create(SchedulerType scheduler_type, int quantum)
{
    Kernel *k = (Kernel *)calloc(1, sizeof(Kernel));
    if (!k)
    {
        perror("Failed to allocate memory for Kernel");
        return NULL;
    }
    k->pcb_list = NULL;
    k->generator_done = 0;
    // k->current_process = NULL;
    k->current_process[0] = NULL;
    k->current_process[1] = NULL;
    k->quantum = quantum;
    pthread_mutex_init(&k->mutex_log, NULL);
    pthread_mutex_init(&k->mutex_queue, NULL);
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
    k->runqueue = NULL;
    return k;
}

void multi_kernel_read_input_file(char *input_path, Kernel *k)
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
    multi_kernel_sort_by_start_time(k);

    fscanf(file, "%d", &k->scheduler_type);

    k->runqueue = queue_create(k->nprocesses);

    fclose(file);
}

void multi_kernel_sort_by_start_time(Kernel *k)
{
    if (k && k->pcb_list && k->nprocesses > 1)
    {
        qsort(k->pcb_list, k->nprocesses, sizeof(PCB *), compare_pcb_start_time);
    }
}

void *multi_routine(void *args)
{
    //struct utilizado para passar o TCB e o kernel
    ThreadArgs *arg = (ThreadArgs *)args;
    Kernel *k = arg->k;
    TCB *tcb = arg->tcb;
    free(arg);
    PCB *p = tcb_get_process(tcb);
    pthread_mutex_t *mutex = get_pcb_mutex(p);
    pthread_cond_t *cv = pcb_get_cv(p);
    // Executa até o estado do processo ser finished
    while (1)
    {
        pthread_mutex_lock(mutex);
        // Se o processo não estiver no estado running,finished ou não for o index da thread a ser executada da vez
        //a thread deve se bloquear 
         while (pcb_get_state(p) != FINISHED &&
        (pcb_get_state(p) != RUNNING || tcb_get_thread_index(tcb) != pcb_get_active_thread_index(p)))
        // while (pcb_get_state(p) != RUNNING && pcb_get_state(p) != FINISHED)
        {
            pthread_cond_wait(cv, mutex);
        }

        // se o processo finalizou, o mutex é liberado e encerra a thread/processo
        if (pcb_get_state(p) == FINISHED)
        {
            pthread_mutex_unlock(mutex);
            break;
        }

        int time_to_each_thread = get_process_len(p)/get_num_threads(p);
        
        //Se o tempo de execução restante for menor que o quantum devemos subtrair esse valor do remaining time,
        //se não subtraimos um quantum
        if (time_to_each_thread > QUANTUM)
        {
            time_to_each_thread = QUANTUM;
        }
        sub_remaining_time(p, time_to_each_thread);
        //verificacao se cada thread esta executando como uma entidade própria
        printf("[PID %d] executou thread %d por %dms, faltam %dms\n",
            my_get_pid(p),
            tcb_get_thread_index(tcb),
            time_to_each_thread,
            get_remaining_time(p));

        pthread_mutex_unlock(mutex);
        usleep(time_to_each_thread * 1000);

        pthread_mutex_lock(mutex);
        char out[10];
        int pid=-1;
        if (k->scheduler_type == FCFS)
        {
            strcpy(out, "FCFS");
        }
        else if (k->scheduler_type == RR)
        {
            strcpy(out, "RR");
        }
        else
        {
            strcpy(out, "PRIORITY");
        }
        if (get_remaining_time(p) <= 0 && pcb_get_state(p) != FINISHED)
        {
            pcb_change_state(p, FINISHED);
            //libera as threads que estiverem dentro do primeiro while
            //pois o processo ja executou todas as suas threads
            pthread_cond_broadcast(cv);
            pid=my_get_pid(p);
        }else if (pcb_get_state(p) == RUNNING) 
        {
            // Calcula o índice da próxima thread
            int num_threads = get_num_threads(p);
            int next_thread_index = (pcb_get_active_thread_index(p) + 1) % num_threads;
            pcb_set_active_thread_index(p, next_thread_index);
            //Acorda as outras threads para que a próxima na fila possa executar
            pthread_cond_broadcast(cv);
        }

        pthread_mutex_unlock(mutex);
        if(pid!=-1){
            multi_add_log_entry(k, "[%s] Processo PID %d finalizado", out,pid);
        }
    }
    free(tcb);
    return NULL;
}

// void kernel_RR_schedule(Kernel *k, struct timeval *slice_time)
// {
//     //se a CPU não está vazia e o processo que está lá ultrapassou o quantum, o processo é preemptado

//     if (k->current_process != NULL && get_current_time(*slice_time) >= QUANTUM)
//     {
//         pthread_mutex_t *mutex = get_pcb_mutex(k->current_process);
//         pthread_cond_t *cv = pcb_get_cv(k->current_process);
//         // processo que está na CPU é preemptado e adicionado ao final da fila de prontos caso ainda haja remaining_time
//         pthread_mutex_lock(mutex);
//         if ((pcb_get_state(k->current_process) == RUNNING) && get_remaining_time(k->current_process) > 0)
//         {
//             pcb_change_state(k->current_process, READY);
//             queue_add(k->runqueue, k->current_process);
//             k->current_process = NULL;
//             pthread_cond_broadcast(cv);
//             // CPU fica vazia
//         }
//         pthread_mutex_unlock(mutex);
        
//     }
//     if (!queue_empty(k->runqueue) && k->current_process == NULL)
//     {
//         // Pega o próximo processo da fila
//         PCB *next_process = queue_remove(k->runqueue);
//         k->current_process = next_process;
//         pthread_mutex_t *mutex = get_pcb_mutex(k->current_process);
//         pthread_cond_t *cv = pcb_get_cv(k->current_process);
//         //printf("rodando pid %d\n",my_get_pid(next_process));
//         // Uma vez que o processo está estado RUNNING ele não volta pra ready pois não há preempcao
//         add_log_entry(k, "[RR] Executando processo PID %d com quantum %dms", my_get_pid(next_process), QUANTUM);
//         pthread_mutex_lock(mutex);
//         pcb_change_state(k->current_process, RUNNING);
//         // Atualiza referencia para a fatia de tempo
//         gettimeofday(slice_time, NULL);
//         //  Sinaliza para as threads do processo começarem a executar já q o estado é RUNNING
//         pthread_cond_broadcast(cv);
//         pthread_mutex_unlock(mutex);
//     }   
// }

void multi_kernel_FCFS_schedule(Kernel *k,struct timeval *slice_time)
{
     // Verifica se a fila de prontos não está vazia e se o tempo de fatia de tempo é valido
    if (!queue_empty(k->runqueue) && multi_get_current_time(*slice_time))
    {
        // Pega o próximo processo da fila sem remover
        PCB *next_process = queue_peek(k->runqueue);
        
        // Verifica se há CPUs livres para alocar este processo.
        int allocated_cpus = 0;
        for (int i = 0; i < 2; i++) {
            // Apenas aloca o processo se a CPU estiver livre
            if (k->current_process[i] == NULL) {
                if(allocated_cpus == 0 || queue_size(k->runqueue) == 1)
                {
                    k->current_process[i] = next_process;
                    multi_add_log_entry(k, "[FCFS] Executando processo PID %d // processador %d", my_get_pid(next_process), i);
                    // printf("[PID %d] alocado na CPU %d\n", my_get_pid(next_process), i);
                    allocated_cpus++;
                }
            }
        }

        // Se o processo foi alocado em pelo menos uma CPU, remova-o da fila e sinalize as threads.
        if (allocated_cpus > 0) {
            // nao posso fazer desse jeito (ele pode precisar d eoutra dps)
            queue_remove(k->runqueue);

            pthread_mutex_t *mutex = get_pcb_mutex(next_process);
            pthread_cond_t *cv = pcb_get_cv(next_process);
            
            pthread_mutex_lock(mutex);
            pcb_change_state(next_process, RUNNING);
            pthread_cond_broadcast(cv);
            pthread_mutex_unlock(mutex);
        }

    }
}

// void kernel_prio_schedule(Kernel *k, struct timeval *slice_time)
// {
//      if (k->current_process != NULL && !queue_empty(k->runqueue) && get_current_time(*slice_time) >= QUANTUM)
//     {
//         PCB * next_process=queue_remove_min(k->runqueue,0);
//         if(is_priority_p1_over_p2(next_process,k->current_process)){
//             pthread_mutex_t* mutex=get_pcb_mutex(k->current_process);
            
//             //Verifica se o processo tem maior prioridade e se ele ainda possui remaining_time
            
//             pthread_mutex_lock(mutex);
//             if(pcb_get_state(k->current_process)==RUNNING && get_remaining_time(k->current_process) > 0){
             
//                 // processo que está na CPU é preemptado e adicionado ao final da fila de prontos caso ainda haja remaining_time
//                 pcb_change_state(k->current_process, READY);
//                 queue_add(k->runqueue, k->current_process);
//                 // CPU agora é do processo de maior prioridade
//                 k->current_process = NULL;
                
//             }
//             pthread_mutex_unlock(mutex);
//         }
        
//     }
//     if (k->current_process == NULL && !queue_empty(k->runqueue))
//     {
        
//         PCB *next_process = queue_remove_min(k->runqueue,1);
//         k->current_process = next_process;
//         pthread_mutex_t *mutex = get_pcb_mutex(k->current_process);
//         pthread_cond_t *cv = pcb_get_cv(k->current_process);
//         // Uma vez que o processo está estado RUNNING ele não volta pra ready pois não há preempcao
//         add_log_entry(k, "[PRIORITY] Executando processo PID %d prioridade %d", my_get_pid(next_process),get_priority(next_process));
//         pthread_mutex_lock(mutex);
//         pcb_change_state(k->current_process, RUNNING);
//         // Sinaliza para as threads do processo começarem a executar
//         gettimeofday(slice_time, NULL);
//         pthread_cond_broadcast(cv);
//         pthread_mutex_unlock(mutex);
//     }
// }

void multi_kernel_run_simulation(Kernel *k)
{
    int process_index = 0;
    int finished_processes = 0;
    //contador para tempo de chegada dos processos
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
            pthread_create(&threads_ids[j], NULL, &multi_routine, args);
        }
    }
    //contador de fatia de tempo
    struct timeval slice_time;
    // Enquanto todos os processos não estiverem no estado finalizado
    gettimeofday(&slice_time, NULL);

    while (finished_processes < k->nprocesses)
    {
        // adiciona processos na fila de prontos enquanto todos os processos não foram adicionados e
        // o tempo alcançou o start_time do processo
        if (process_index < k->nprocesses && get_start_time(k->pcb_list[process_index]) <= multi_get_current_time(start_time))
        {
            PCB *p = k->pcb_list[process_index];
            queue_add(k->runqueue, p);
            process_index++;
        }
        
        if (k->scheduler_type == FCFS)
        {
            multi_kernel_FCFS_schedule(k,&slice_time);
        }
        // else if (k->scheduler_type == RR)
        // {
            
        //     kernel_RR_schedule(k, &slice_time);
        // }
        // else
        // {
        //     kernel_prio_schedule(k,&slice_time);
        // }
        // verifica se o processo atual terminou
        for(int i=0; i<2; i++){
            if (k->current_process[i] != NULL)
            {
                // libera a CPU
                pthread_mutex_t *mutex = get_pcb_mutex(k->current_process[i]);
                pthread_mutex_lock(mutex);
                if (pcb_get_state(k->current_process[i]) == FINISHED)
                {
                    k->current_process[i] = NULL;
                    finished_processes++;
                }
                pthread_mutex_unlock(mutex);
            }
        }
        
        // Simula a passagem de tempo para que o loop não ocorra várias vezes sem acontecer nada
        usleep(100);
        
    }
    // Espera todas as threads terminarem antes de sair
    for (int i = 0; i < k->nprocesses; i++)
    {
        PCB *p = k->pcb_list[i];
        int num_threads = get_num_threads(p);
        pthread_t *threads_ids = get_threads_ids(p);
        for (int j = 0; j < num_threads; j++)
        {
            pthread_join(threads_ids[j], NULL);
        }
    }
    multi_kernel_print_log(k);

    printf("Escalonador terminou execução de todos processos\n");
}
void multi_kernel_destroy(Kernel *k)
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

void multi_kernel_printa_runqueue(Kernel *k)
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

void multi_add_log_entry(Kernel *k, const char *format, ...)
{
    if (!k)
        return;

    // Redimensiona o buffer se estiver cheio
    pthread_mutex_lock(&k->mutex_log);

    if (k->log_count >= k->log_capacity)
    {
        k->log_capacity *= 2;
        k->log_buffer = (char **)realloc(k->log_buffer, k->log_capacity * sizeof(char *));
        if (!k->log_buffer)
        {
            perror("Failed to reallocate log buffer");
            exit(EXIT_FAILURE);
            pthread_mutex_unlock(&k->mutex_log);
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
    pthread_mutex_unlock(&k->mutex_log);
}

void multi_kernel_print_log(Kernel *k)
{
    if (!k || !k->log_buffer)
        return;

    for (int i = 0; i < k->log_count; i++)
    {
        printf("%s\n", k->log_buffer[i]);
    }
}

int multi_get_current_time(struct timeval start_time)
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    long elapsed_time_ms = (current_time.tv_sec - start_time.tv_sec) * 1000 +
                           (current_time.tv_usec - start_time.tv_usec) / 1000;
    return (int)elapsed_time_ms;
}