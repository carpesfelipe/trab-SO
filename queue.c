#include "queue.h"
#include "pcb.h"

struct queue {
	int size;
	PCB **data;
	int first;
	int last;
	int nItens; 
};

Queue *queue_create(Queue *q, int s){
    Queue *q = (Queue *)calloc(1, sizeof(Queue));
    if (!q) {
        perror("Failed to allocate memory for queue");
        return NULL;
    }

    q->size = s;
    q->first = 0;
    q->last = -1; // começa com -1 pq ainda nao tem nada preenchidos
    q->data = (PCB **)calloc(1, sizeof(PCB*));

    return q;
}

int queue_full(Queue *q){
    return (q->nItens == q->size);
}

int queue_empty(Queue *q){
    return (q->nItens == 0);
}

void queue_add(Queue *q, PCB *pcb){

    if(!queue_full(q)){ // so preenche se nao estiver cheio
        if(q->last == q->size-1){
            q->last = -1; //volta p -1 pra ver se a primeira posicao ja foi liberada
        }    

        q->last++;
        q->nItens++;
        q->data[q->last] = pcb;
    }
}

int queue_remove(Queue *q){
    
}
