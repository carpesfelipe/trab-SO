# all:
# 	gcc -pthread -Wall -o trabSO *.c 
run:
	./trabSO casos_teste/entradas/1.txt
zip:
TARNAME = 2022102425,2022101541
FILES = *.c *.h Makefile

tarball:
	tar -czvf $(TARNAME).tar.gz $(FILES)

# Define os arquivos de código fonte que são comuns aos dois projetos
SRC_COMMON = pcb.c tcb.c queue.c

monoprocessador:
	gcc -pthread -Wall -o trabSO $(SRC_COMMON) kernel.c main.c

multiprocessador:
	gcc -pthread -Wall -o trabSO $(SRC_COMMON) kernel_multiprocessado.c main_multi.c