/*
    * Programa principal para simulacao de um sistema operacional monoprocessado
    * 
    * Este programa cria um kernel, le um arquivo de entrada com processos, simula a execucao dos processos
    * e finalmente libera a memoria alocada.
    * 
    * Autor: Débora Azevedo (2022101541) e Felipe Carpes (2022102425)
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "pcb.h"
#include "kernel.h"
#include "queue.h"

#include "queue.h"

int main(int argc, char *argv[]) {
    Kernel *kernel=kernel_create(FCFS,500);
    //processos criados e inicializados
    kernel_read_input_file(argv[1],kernel);

    //Loop para simular o tempo e mover os processos para a runqueue
    kernel_run_simulation(kernel);

    // kernel_schedule(kernel);
    //kernel_print_output_file(kernel);

    kernel_destroy(kernel);

    return 0;

}