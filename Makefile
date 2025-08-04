CC=gcc
FLAGS=-Wall -Wextra

all: sequencial paralelo
	./sequencial > sequencial.out && ./paralelo > paralelo.out

sequencial: nmop.c
	$(CC) $(FLAGS) nmop.c -o sequencial

paralelo: nmop.c
	$(CC) $(FLAGS) -DPARALELO nmop.c -o paralelo -lpthread

purge: 
	rm -f sequencial* paralelo*
