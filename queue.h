/*
    * Biblioteca para manipulação de filas circulares
    * 
    * Esta biblioteca fornece funções para criar, manipular e destruir filas, além de verificar se estão cheias ou vazias, adicionar e remover elementos, e imprimir os elementos da fila.
    * 
    * Autor: Débora Azevedo e Felipe Carpes
*/

#pragma once

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
int queue_remove(Queue *q);

// imprime os elementos da fila
void queue_print(Queue *q);

// destrói a fila e libera a memória alocada
void queue_destroy(Queue *q);