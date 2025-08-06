CC=gcc
FLAGS=-Wall -Wextra

all: sequencial paralelo mrand run.sh
	./run.sh

sequencial: nmop.c
	$(CC) $(FLAGS) nmop.c -o sequencial

paralelo: nmop.c
	$(CC) $(FLAGS) -DPARALELO nmop.c -o paralelo -lpthread

mrand: mrand.c
	$(CC) $(FLAGS) mrand.c -o mrand

purge: 
	rm -f sequencial* paralelo* mrand *.txt
