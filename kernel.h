#pragma once

typedef struct kernel Kernel;
typedef int SchedulerType;
#define RR 1
#define FCFS 2
#define PRIO 3

void kernel_read_input_file(char * input_path,Kernel *k);
void kernel_print_input_file(Kernel *k);
void kernel_schedule(Kernel *k);