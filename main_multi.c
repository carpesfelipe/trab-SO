#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "pcb.h"
// #include "kernel.h"
#include "queue.h"

#include "kernel_multiprocessado.h"

int main(int argc, char *argv[]) {
    Kernel *kernel=multi_kernel_create(FCFS,500);
    //processos criados e inicializados
    multi_kernel_read_input_file(argv[1],kernel);

    //Loop para simular o tempo e mover os processos para a runqueue
    multi_kernel_run_simulation(kernel);

    //libera a memoria alocada
    multi_kernel_destroy(kernel);

    return 0;

}