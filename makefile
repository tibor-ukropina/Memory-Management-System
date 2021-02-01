CC = gcc
CFLAGS = -Wall -ansi -pedantic

all: arraytester listtester

arraytester: arraytester.c ds_memory.o ds_array.o 
	$(CC) $(CFLAGS) arraytester.c ds_memory.o ds_array.o -o arraytester

listtester: listtester.c ds_memory.o ds_list.o 
	$(CC) $(CFLAGS) listtester.c ds_memory.o ds_list.o -o listtester

ds_memory.o: ds_memory.c ds_memory.h
	$(CC) $(CFLAGS) -c ds_memory.c -o ds_memory.o

ds_array.o: ds_array.c ds_array.h
	$(CC) $(CFLAGS) -c ds_array.c -o ds_array.o

ds_list.o: ds_list.c ds_list.h
	$(CC) $(CFLAGS) -c ds_list.c -o ds_list.o

clean:
	$(RM) ds_memory *.o *~

run:
	./main