cc = gcc

main: main.c
	cc -o main main.c -lraylib -lm

clean: main
	rm -r main
