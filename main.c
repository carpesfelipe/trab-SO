/*
    * Programa principal para simulacao de um Mini-Kernel Multithread com suporte a Escalonamento por FCFS, RR e Prioridade (Preemptiva)
    * 
    * Este programa cria um kernel, le um arquivo de entrada com processos, simula a execucao dos processos
    * e finalmente libera a memoria alocada.
    * 
    * Autor: Débora Azevedo (2022101541) e Felipe Carpes (2022102425)
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "kernel.h"

int main(int argc, char *argv[]) {
    
    Kernel *kernel=kernel_create();

    kernel_read_input_file(argv[1],kernel);

    kernel_run_simulation(kernel);

    kernel_destroy(kernel);

    return 0;

}