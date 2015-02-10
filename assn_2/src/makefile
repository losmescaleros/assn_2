# Executable
EXEC = assn_2
# Compiler
CC = g++
# Compiler options
CFLAGS = -c -Wall

all: main

main: main.o
	$(CC) -o $(EXEC) main.o

main.o: main.c
	$(CC) $(CFLAGS) main.c

