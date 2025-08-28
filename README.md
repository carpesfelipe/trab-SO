# trab-SO
Implementação de um Mini-Kernel Multithread com suporte a Escalonamento por FCFS, RR e Prioridade (Preemptiva) 

Para implementação do trabalho foi utilizada uma estrutura Kernel, para diminuir/extinguir a quantidade de variáveis globais necessárias, tornando um ambiente mais controlado.
Essa estrutura carrega informações como: Fila de prontos(Queue * runqueue); vetor de processos lidos(PCB **list); processo que ocupa a cpu no momento(PCB *current process); um log buffer(cahr **log_buffer para adicionar 
as mensagens em tempo de execução juntamente com variáveis para seu redimensionamento(int log count e int log_capacity; e por fim o tipo de escalonamento adotado(SchedulerType);
O programa cliente possui, além das funções de inicializaçã, leitura e liberação de memória, uma função chave(kernel_run_simulation) que realiza as seguintes tarefas: cria as threads de todos os 
processos(são criadas mas não executadas); move processos para a fila de prontos de acordo com seu tempo de chegada, atribui uma funçáo de escalonar que irá acordar as threads do processo(de acordo com a 
política de escalonamento) e por fim aguarda que todas as threads terminem. Todas as threads dos processos executam a mesma rotina de trabalho (função routine passada em pthread_crete). Ela bloqueia a thread aguardando
pela variável de condição(controlada pelo estado do processo e pelo índice da thread(para que duas threads não executem ao mesmo tempo, já que se trata de um monoprocessado)



https://www.cos.ufrj.br/~rfarias/cos121/filas.html
