#pragma once

typedef struct kernel Kernel;
typedef int SchedulerType;
#define FCFS 1
#define RR 2
#define PRIO 3
#define QUANTUM 500

void kernel_read_input_file(char * input_path,Kernel *k);
void kernel_print_output_file(Kernel *k);
void kernel_schedule(Kernel *k);
void kernel_print(Kernel *k);
void round_robin_schedule(Kernel *k);
void FCFS_schedule(Kernel *k);
void prio_schedule(Kernel *k);
Kernel *kernel_create(SchedulerType scheduler_type, int quantum);
void kernel_add_process(Kernel *k, PCB *p);
void kernel_destroy(Kernel *k);