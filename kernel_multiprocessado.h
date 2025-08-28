#pragma once

typedef struct kernel Kernel;
typedef int SchedulerType;
#define FCFS 1
#define RR 2
#define PRIO 3
#define QUANTUM 500

void multi_kernel_read_input_file(char * input_path,Kernel *k);
void multi_kernel_print_output_file(Kernel *k);
void multi_kernel_schedule(Kernel *k);
void multi_kernel_print(Kernel *k);
void multi_kernel_RR_schedule(Kernel *k, struct timeval *slice_time);
void multi_kernel_FCFS_schedule(Kernel *k,struct timeval *slice_time, int finished_processes);
void multi_kernel_prio_schedule(Kernel *k,struct timeval *slice_time);
Kernel *multi_kernel_create(SchedulerType scheduler_type, int quantum);
void kernel_add_process(Kernel *k, PCB *p);
void multi_kernel_destroy(Kernel *k);
void multi_kernel_sort_by_start_time(Kernel *k);

void multi_kernel_run_simulation(Kernel *k);
void multi_kernel_printa_runqueue(Kernel *k);
int multi_get_current_time(struct timeval start_time);
void multi_kernel_print_log(Kernel *k);
void multi_add_log_entry(Kernel *k, const char *message,...);
 