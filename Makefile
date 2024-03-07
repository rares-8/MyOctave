#Copyright Apostol Rares-Andrei 311CAa 2022-2023
CC = gcc
CFLAGS =-std=c99 -Wall -Wextra
build: my_octave

my_octave: my_octave.c
	$(CC) $(CFLAGS) my_octave.c -o my_octave

clean:
	rm -f my_octave
