#include "tcb.h"

typedef struct tcb
{
    PCB *pcb;
    int thread_index;
};
TCB *tcb_create(PCB *pcb, int thread_index)
{
    TCB *tcb = (TCB *)calloc(1, sizeof(TCB));
    if (!tcb) {
        perror("Failed to allocate memory for TCB");
        return NULL;
    }
    tcb->pcb = pcb;
    tcb->thread_index = thread_index;
    return tcb;
}