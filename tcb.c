/*
    * Biblioteca para manipulação de TCBs (Thread Control Blocks)
    * 
    * Esta biblioteca fornece funções para criar, manipular e destruir TCBs, além de obter informações sobre as threads associadas a um processo
    * 
    * Autor: Débora Azevedo (2022101541) e Felipe Carpes (2022102425)
*/

#include "tcb.h"

struct tcb
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
void tcb_destroy(TCB * tcb){
    free(tcb);
}
PCB * tcb_get_process(TCB * tcb){
    return tcb->pcb;
}
int tcb_get_thread_index(TCB * tcb){
    return tcb->thread_index;
}