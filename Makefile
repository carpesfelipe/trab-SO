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
val:
	valgrind --leak-check=full -s ./trabSO /home/felipecarpes/trab-SO/casos_teste_v3/entradas/1.txt > /home/felipecarpes/trab-SO/minhas_saidas/1.txt
	valgrind --leak-check=full -s ./trabSO /home/felipecarpes/trab-SO/casos_teste_v3/entradas/2.txt > /home/felipecarpes/trab-SO/minhas_saidas/2.txt
	valgrind --leak-check=full -s ./trabSO /home/felipecarpes/trab-SO/casos_teste_v3/entradas/3.txt > /home/felipecarpes/trab-SO/minhas_saidas/3.txt
	valgrind --leak-check=full -s ./trabSO /home/felipecarpes/trab-SO/casos_teste_v3/entradas/4.txt > /home/felipecarpes/trab-SO/minhas_saidas/4.txt
	valgrind --leak-check=full -s ./trabSO /home/felipecarpes/trab-SO/casos_teste_v3/entradas/5.txt > /home/felipecarpes/trab-SO/minhas_saidas/5.txt
	valgrind --leak-check=full -s ./trabSO /home/felipecarpes/trab-SO/casos_teste_v3/entradas/6.txt > /home/felipecarpes/trab-SO/minhas_saidas/6.txt
	valgrind --leak-check=full -s ./trabSO /home/felipecarpes/trab-SO/casos_teste_v3/entradas/7.txt > /home/felipecarpes/trab-SO/minhas_saidas/7.txt
diff:
	diff 
multiprocessador:
	gcc -pthread -g -O0 -Wall -o trabSO $(SRC_COMMON) kernel_multiprocessado.c main_multi.c