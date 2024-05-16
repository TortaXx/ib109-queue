CC=gcc
CFLAGS=-I.
DEPS = hellomake.h


run: build
	@./main

build:
	@$(CC) -pthread -g -lm lf_queue.c mutex_queue.c main.c -o main

# %.o: %.c $(DEPS)
# 	$(CC) -c -o $@ $< $(CFLAGS)

# hellomake: hellomake.o hellofunc.o 
# 	$(CC) -o hellomake hellomake.o hellofunc.o 
