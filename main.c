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