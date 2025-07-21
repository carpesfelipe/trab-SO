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
void print_process(PCB *p);
int get_tempo_execucao(PCB *p);
int get_prio(PCB *p);
int get_qtdThreads(PCB *p);
int meu_get_pid(PCB *p);
int get_tempo_chegada(PCB *p);

