CC = gcc
CFLAGS = -Wall -pedantic -g -std=c99
MAIN = main
OBJS = main.o toolkit.o array_list.o iarray_list.o farray_list.o county_info.o operator_handler.o

all : $(MAIN)

$(MAIN) : $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

main.o : main.c util/toolkit.h util/array_list/array_list.h util/array_list/iarray_list.h util/array_list/farray_list.h operator_handler.h county_info.h
	$(CC) $(CFLAGS) -c main.c

toolkit.o : util/toolkit.c
	$(CC) $(CFLAGS) -c util/toolkit.c

county_info.o : county_info.c
	$(CC) $(CFLAGS) -c county_info.c

operator_handler.o : operator_handler.c
	$(CC) $(CFLAGS) -c operator_handler.c

array_list.o : util/array_list/array_list.c
	$(CC) $(CFLAGS) -c util/array_list/array_list.c

iarray_list.o : util/array_list/iarray_list.c
	$(CC) $(CFLAGS) -c util/array_list/iarray_list.c

farray_list.o : util/array_list/farray_list.c
	$(CC) $(CFLAGS) -c util/array_list/farray_list.c

clean :
	rm *.o $(MAIN) core*