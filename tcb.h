#pragma once
#include "pcb.h"

typedef struct tcb TCB;

TCB *tcb_create(PCB *pcb, int thread_index);
int tcb_get_thread_index(TCB *tcb);
PCB *tcb_get_process(TCB *tcb);