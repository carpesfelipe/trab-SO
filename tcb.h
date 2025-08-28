/*
    * Biblioteca para manipulação de TCBs (Thread Control Blocks)
    * 
    * Esta biblioteca fornece funções para criar, manipular e destruir TCBs, além de obter informações sobre as threads associadas a um processo
    * 
    * Autor: Débora Azevedo (2022101541) e Felipe Carpes (2022102425)
*/

#pragma once
#include "pcb.h"

typedef struct tcb TCB;

// essa funcao cria um novo TCB
TCB *tcb_create(PCB *pcb, int thread_index);

// essa funcao retorna o indice da thread
int tcb_get_thread_index(TCB *tcb);

// essa funcao retorna o PCB associado ao TCB
PCB *tcb_get_process(TCB *tcb);