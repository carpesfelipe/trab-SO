#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#define READY 1
#define RUNNING 2
#define FINISHED 3

typedef struct pcb PCB;
typedef int ProcessState;
PCB *process_create(int pid, int tempoExecucao, int prio, int qtdThreads, int tempoChegada);
void process_run(PCB * p);
int my_get_pid(PCB * p);
void print_process(PCB *p);
int get_remaining_time(PCB *p);
int get_process_len(PCB* p);
int get_prio(PCB *p);
int get_num_threads(PCB *p);
int get_start_time(PCB *p);
pthread_t * get_threads_ids(PCB* p);
void process_destroy(PCB *p);
void sub_remaining_time(PCB * p,int time);
void pcb_change_state(PCB * p);
int compare_pcb_start_time(const void *a, const void *b);
pthread_mutex_t * get_pcb_mutex(PCB *p);

