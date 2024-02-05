CC = gcc
CFLAGS = -std=c99 -g -Wall
LFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

compile:
	@$(CC) -o EstimatePi main.c $(CFLAGS) $(LFLAGS)
