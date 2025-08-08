#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "pcb.h"

struct queue {
	int size;
	PCB **data;
	int first;
	int last;
	int nItens; 
};

Queue *queue_create(int s){
    Queue *q = (Queue *)calloc(1, sizeof(Queue));
    if (!q) {
        perror("Failed to allocate memory for queue");
        return NULL;
    }

    q->size = s;
    q->first = 0;
    q->last = -1; // começa com -1 pq ainda nao tem nada preenchidos
    q->nItens = 0;
    q->data = (PCB **)calloc(s, sizeof(PCB*));
    if (!q->data) {
        free(q);
        return NULL;
    }

    return q;
}

int queue_full(Queue *q){
    return (q->nItens == q->size);
}

int queue_empty(Queue *q){
    return (q->nItens == 0);
}

void queue_add(Queue *q, PCB *pcb){

    if (queue_full(q)){
        printf("Fila está cheia, não pode adicionar novo elemento\n");
        return;
    }

    // q->last++;
    q->last = (q->last + 1) % q->size; // Lógica circular com módulo
    q->nItens++;
    q->data[q->last] = pcb;
}

PCB *queue_remove(Queue *q){
    if (queue_empty(q)){
        printf("Fila está vazia, não pode remover elemento\n");
        // return -1;
    }

    PCB *temp = q->data[q->first];

    q->first = (q->first + 1) % q->size; // Lógica circular com módulo
    q->nItens--;
    return temp;
}

PCB *queue_remove_arbitrary(Queue *q, int k){
    if (queue_empty(q) || k<0 || k>=q->size){
        printf("Impossível remover elemeto no idx: %d", k);
    }

    //for(int i=k; i<q->size-1; i++ ){
    //    PCB atual = 
    //}
}

void queue_print(Queue *q) {
    if(queue_empty(q)) {
        printf("Queue esta vazia.\n");
        return;
    }

    printf("Queue:\n");
    int indice_atual = q->first;
    for (int i = 0; i < q->nItens; i++)
    {
        printf("n[%d]: \n", indice_atual);
        print_process(q->data[indice_atual]);
        indice_atual = (indice_atual + 1) % q->size; // Lógica circular com módulo
    }
}

void queue_destroy(Queue *q) {
    if (!q) return;
    
    free(q->data);
    free(q);
}
