CC = gcc
CFLAGS = -Wall -Wextra -O3


.PHONY = all


all: build exec clean

build:
	$(CC) $(CFLAGS) main.c -o main -lpthread


exec: build
	@./main


clean: exec 
	rm ./main