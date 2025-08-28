/*
    * Biblioteca para manipulacao do Kernel para um sistema monoprocessado (apenas uma cpu)
    * 
    * Esta biblioteca fornece funções para criar, manipular e destruir o Kernel, além de obter informações sobre os processos
    * 
    * Autor: Débora Azevedo (2022101541) e Felipe Carpes (2022102425)
*/

#pragma once

// Definição da estrutura do Kernel
typedef struct kernel Kernel;
// Definição do tipo de escalonador
typedef int SchedulerType;

// Definição dos tipos de escalonadores
#define FCFS 1
#define RR 2
#define PRIO 3
#define QUANTUM 500

// Funcao para ler o arquivo de entrada
void kernel_read_input_file(char * input_path,Kernel *k);

// Funcao para executar o escalonamento RR
void kernel_RR_schedule(Kernel *k,struct timeval *slice_time);

// Funcao para executar o escalonamento FCFS
void kernel_FCFS_schedule(Kernel *k,struct timeval *slice_time);

// Funcao para executar o escalonamento por prioridade
void kernel_prio_schedule(Kernel *k,struct timeval *slice_time);

// Funcao para criar o Kernel
Kernel *kernel_create(SchedulerType scheduler_type, int quantum);

// Funcao para destruir o Kernel
void kernel_destroy(Kernel *k);

// Funcao para ordenar os processos pelo tempo de chegada
void kernel_sort_by_start_time(Kernel *k);

// Funcao para executar a simulacao do Kernel
void kernel_run_simulation(Kernel *k);

// Funcao para imprimir a fila de prontos
void kernel_printa_runqueue(Kernel *k);

// Funcao para obter o tempo atual
int get_current_time(struct timeval start_time);

// Funcao para imprimir o log do Kernel
void kernel_print_log(Kernel *k);

// Funcao para adicionar uma entrada ao log do Kernel
void add_log_entry(Kernel *k, const char *message,...);
