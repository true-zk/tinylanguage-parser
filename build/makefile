#
# makefile for TINY
# Borland C Version
# K. Louden 2/3/98
#
# modified at Oct. 29, 2022 in SJTU
#

CC = gcc

CFLAGS =

oS = main.o util.o lexer.o parse.o

main.o: ../main.c ../globals.h ../util.h ../lexer.h ../parse.h
	$(CC) $(CFLAGS) -c ../main.c

lexer.o: ../lexer.c ../lexer.h ../util.h ../globals.h
	$(CC) $(CFLAGS) -c ../lexer.c

util.o: ../util.c ../util.h ../globals.h
	$(CC) $(CFLAGS) -c ../util.c

parse.o: ../parse.c ../parse.h ../lexer.h ../globals.h ../util.h
	$(CC) $(CFLAGS) -c ../parse.c

clean:
	-rm main.o
	-rm util.o
	-rm lexer.o
	-rm parse.o

rmexe:
	-rm bin/parser

parse: $(oS)
	$(CC) $(CFLAGS) -o bin/parser $(oS)

testp:
	-bin/parser test/sample.tny

