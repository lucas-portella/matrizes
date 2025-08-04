CC=gcc
FLAGS=-Wall -Wextra

all: sequencial paralelo mrand lmat
	#./sequencial > sequencial.out && ./paralelo > paralelo.out

sequencial: nmop.c
	$(CC) $(FLAGS) nmop.c -o sequencial

paralelo: nmop.c
	$(CC) $(FLAGS) -DPARALELO nmop.c -o paralelo -lpthread

mrand: mrand.c
	$(CC) $(FLAGS) mrand.c -o mrand

lmat: lmat.c
	$(CC) $(FLAGS) lmat.c -o lmat

purge: 
	rm -f sequencial* paralelo* mrand lmat
