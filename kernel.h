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

// Executa algoritmo de FCFS, executando um processo inteiro antes de olhar o próximo
void kernel_FCFS_schedule(Kernel *k,struct timeval *slice_time);

// Executa algoritmo de prioridade preemptiva acordando as threads quando necessário
void kernel_prio_schedule(Kernel *k,struct timeval *slice_time);

// Inicializa variáveis do kernel
Kernel *kernel_create();

// Libera recursos alocados para o kernel
void kernel_destroy(Kernel *k);

// Funcao para ordenar os processos pelo tempo de chegada
void kernel_sort_by_start_time(Kernel *k);

// Insere processos na fila de prontos de acordo com o seu tempo de chegada, definindo qual algoritmo
//de escalonamento será executado
void kernel_run_simulation(Kernel *k);

// Funcao para imprimir a fila de prontos
void kernel_print_runqueue(Kernel *k);

//Dado um valor de tempo inicial, retorna o tempo que se passou em ms
int get_current_time(struct timeval start_time);

// Funcao para imprimir o log do Kernel em um arquivo
void kernel_print_log(Kernel *k);

// Funcao para adicionar uma entrada ao log do Kernel
void add_log_entry(Kernel *k, const char *message,...);
