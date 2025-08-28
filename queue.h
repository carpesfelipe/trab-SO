/*
    * Biblioteca para manipulação de filas circulares
    * 
    * Esta biblioteca fornece funções para criar, manipular e destruir filas, além de verificar se estão cheias ou vazias, adicionar e remover elementos, e imprimir os elementos da fila.
    * 
    * Autor: Débora Azevedo (2022101541) e Felipe Carpes (2022102425)
*/

#pragma once
#include "pcb.h"

typedef struct queue Queue;

// cria uma fila 
Queue *queue_create(int s);

// verifica se a fila está cheia
int queue_full(Queue *q);

// verifica se a fila está vazia
int queue_empty(Queue *q);

// adiciona um elemento na fila
void queue_add(Queue *q, PCB *pcb);

// remove um elemento da fila
PCB *queue_remove(Queue *q);

// remove um elemento arbitrario da fila
PCB *queue_remove_arbitrary(Queue *q, int k);

// imprime os elementos da fila //SE QUISER, AINDA PRECISA FUNCAO P PRINTAR PCB
void queue_print(Queue *q);

//retorna o primeiro elemento da fila
PCB * queue_get_first(Queue* q);

//retorna o menor elemento, caso o remove=0 e remove o item da fila caso contrario
PCB * queue_remove_min(Queue * q,int remove);

// destroi a fila e libera a memoria alocada 
void queue_destroy(Queue *q);

// retorna o primeiro elemento da fila sem remove-lo
PCB *queue_peek(Queue *q);

// retorna o tamanho da fila
int queue_size(Queue *q);

// retorna o elemento na posição index da fila sem remove-lo
PCB *queue_peek_at(Queue *q, int index);
