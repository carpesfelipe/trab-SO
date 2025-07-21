#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "pcb.h"
#include "kernel.h"

int main(int argc, char *argv[]) {
    Kernel *kernel;
    kernel_read_input_file(argv[1],kernel);
    //kernel_print(kernel);
    //kernel_schedule(kernel);
    //kernel_print_output_file(kernel);
    
}