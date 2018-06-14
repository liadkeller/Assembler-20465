assembler: main.o table.o analysis.o secondPass.o binary.o
	gcc -g -ansi -Wall main.o table.o analysis.o secondPass.o binary.o -pedantic -o assembler

main.o: main.c main.h utils.h binary.h table.h
	gcc -c  -g -ansi main.c  -Wall -pedantic -o main.o

table.o: table.c utils.h table.h
	gcc -c -g -ansi table.c -Wall -pedantic -o table.o

analysis.o: analysis.c utils.h table.h
	gcc -c -g -ansi analysis.c -Wall -pedantic -o analysis.o

secondPass.o: secondPass.c utils.h binary.h table.h
	gcc -c -g -ansi secondPass.c -Wall -pedantic -o secondPass.o

binary.o: binary.c utils.h binary.h table.h
	gcc -c -g -ansi binary.c -Wall -pedantic -o binary.o

clean: main.o table.o analysis.o secondPass.o binary.o
