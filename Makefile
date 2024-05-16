CC=gcc
CFLAGS=-I.
DEPS = hellomake.h


run: build
	@./main

build:
	@$(CC) -pthread -g -lm lf_queue.c mutex_queue.c main.c -o main

