all: main

main:
	
	gcc main.c  `pkg-config --libs --cflags opencv` -ldl -lm

