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
void kernel_RR_schedule(Kernel *k);
void kernel_FCFS_schedule(Kernel *k);
void kernel_prio_schedule(Kernel *k);
Kernel *kernel_create(SchedulerType scheduler_type, int quantum);
void kernel_add_process(Kernel *k, PCB *p);
void kernel_destroy(Kernel *k);
void kernel_sort_by_start_time();