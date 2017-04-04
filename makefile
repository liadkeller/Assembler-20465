assembler: main.o table.o analysis.o 
	gcc -g -ansi -Wall main.o table.o analysis.o -pedantic -o assembler


main.o: main.c main.h
	gcc -c  -g -ansi main.c  -Wall -pedantic -o main.o

table.o: table.c table.h utils.h
	gcc -c -g -ansi table.c -Wall -pedantic -o table.o


analysis.o: analysis.c utils.h table.h
	gcc -c -g -ansi analysis.c -Wall -pedantic -o analysis.o
