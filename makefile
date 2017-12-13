all: main

main:

	gcc main.c -o predictor `pkg-config --libs --cflags opencv` `pkg-config --cflags --libs gtk+-2.0` -ldl -lm 
