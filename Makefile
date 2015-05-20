CC=gcc

CFLAGS=-c -Wall

all: pagetable

pagetable: main.o pageFunctions.o byu_tracereader.o
	$(CC) main.o pageFunctions.o byu_tracereader.o  $(LFLAGS) -o pagetable

main.o: main.c
	$(CC) $(CFLAGS) main.c

pageFunctions.o: pageFunctions.c
	$(CC) $(CFLAGS) pageFunctions.c

byu_tracereader.o: byu_tracereader.c
	$(CC) $(CFLAGS) byu_tracereader.c

clean:
	$(RM) *.o pagetable
