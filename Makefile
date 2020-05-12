CC=gcc
CFLAGS=-O3

debug: inscript.c
	gcc -g -o inscript $<

release: clean inscript
	
inscript: inscript.c

clean:
	- rm inscript