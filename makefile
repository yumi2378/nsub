all: nsub
nsub: nsub.c
	gcc -Wall -o nsub nsub.c
