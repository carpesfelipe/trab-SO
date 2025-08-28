/*
    * Biblioteca para  manipulacao de PCB (Process Control Blocks)
    * 
    * Esta biblioteca fornece funções para criar, manipular e destruir PCBs, além de obter informações sobre os processos
    * 
    * Autor: Débora Azevedo (2022101541) e Felipe Carpes (2022102425)
*/

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

// Definição dos estados do processo
#define READY 1
#define RUNNING 2
#define FINISHED 3

// Definição da estrutura PCB
typedef struct pcb PCB;
// Definição do estado do processo
typedef int ProcessState;

// Funcao para criar um novo processo
PCB *process_create(int pid, int tempoExecucao, int prio, int qtdThreads, int tempoChegada);

// Funcao para executar um processo
void process_run(PCB * p);

// Funcao para obter o PID de um processo
int my_get_pid(PCB * p);

// Funcao para imprimir informações sobre um processo
void print_process(PCB *p);

// Funcao para obter o estado de um processo (retorna um inteiro que corresponde a um estado, seguindo a definicao em pcb.h)
int pcb_get_state(PCB * p);

// Funcao para obter o tempo restante de um processo
int get_remaining_time(PCB *p);

// Funcao para obter o tamanho de um processo
int get_process_len(PCB* p);

// Funcao para obter a prioridade de um processo
int get_priority(PCB *p);

// Funcao para obter o numero de threads de um processo
int get_num_threads(PCB *p);

// Funcao para obter o tempo de início de um processo
int get_start_time(PCB *p);

// Funcao para obter os IDs das threads de um processo
pthread_t * get_threads_ids(PCB* p);

// Funcao para destruir um processo
void process_destroy(PCB *p);

// Função para subtrair o tempo restante de um processo
void sub_remaining_time(PCB * p,int time);

// Funcao para mudar o estado de um processo
void pcb_change_state(PCB * p,ProcessState state);

// Funcao para comparar o tempo de início de dois processos
int compare_pcb_start_time(const void *a, const void *b);

// Funcao para obter a variavel de condicao de um processo
pthread_cond_t *pcb_get_cv(PCB * p);

// Funcao para obter o mutex de um processo
pthread_mutex_t * get_pcb_mutex(PCB *p);

// Funcao para verificar se a prioridade de p1 (processo1) e maior que a de p2(processo2)
int is_priority_p1_over_p2(PCB * p1,PCB * p2 );

// Funcao para obter o indice da thread ativa de um processo
int pcb_get_active_thread_index(PCB* p);

// Funcao para definir o indice da thread ativa de um processo
void pcb_set_active_thread_index(PCB* p, int index);