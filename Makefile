CC = gcc -pthread
DEBUG = -g
CFLAGS = -Wall -std=c99 -c $(DEBUG)
LFLAGS = -Wall -std=c99 $(DEBUG)

sim04 : mainprog.o parMethods.o myTimer.o simulator.o 
	$(CC) $(LFLAGS) mainprog.o parMethods.o myTimer.o simulator.o -o sim04

mainprog.o : mainprog.c mainprog.h parMethods.h myTimer.h simulator.h
	$(CC) $(CFLAGS) mainprog.c

parMethods.o : parMethods.c parMethods.h
	$(CC) $(CFLAGS) parMethods.c

myTimer.o : myTimer.c myTimer.h
	$(CC) $(CFLAGS) myTimer.c

simulator.o : simulator.c simulator.h
	$(CC) $(CFLAGS) simulator.c


clean:
	\rm *.o sim04

