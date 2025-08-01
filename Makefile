all:
	gcc -pthread -Wall -o trabSO *.c 
run:
	./trabSO casos_teste/entradas/1.txt
zip:
TARNAME = 2022102425,2022101541
FILES = *.c *.h Makefile

tarball:
	tar -czvf $(TARNAME).tar.gz $(FILES)