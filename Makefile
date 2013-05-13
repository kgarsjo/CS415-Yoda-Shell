CC=gcc
CCFLAGS=-g -Wall
SYSC_PATH=syscall/

all: shell.o splash.o sysCallTest.o
	$(CC) shell.o -o yoda-sh
	$(CC) splash.o -o splash
	$(CC) sysCallTest.o -o sysCallTest

shell.o: shell.c
	$(CC) $(CCFLAGS) -c shell.c 

splash.o: splash.c
	$(CC) $(CCFLAGS) -c splash.c

sysCallTest.o:
	$(CC) $(CCFLAGS) -c $(SYSC_PATH)sysCallTest.c

clean:
	rm *.o
	rm yoda-sh
	rm splash
	rm sysCallTest
