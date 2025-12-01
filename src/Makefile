CC = gcc
CFLAGS ?= -Wall
TARGETS = dip_mp_tasks dip_mp_for dip_seq Image.o FileIO.o DIPs.o utils.o
RM = rm -f
DEBUG = 0

ifeq ($(DEBUG), 1)
	CFLAGS += -g
endif

all : $(TARGETS)
.PHONY : all

dip_seq : dip_seq.c Image.o FileIO.o DIPs.o utils.o
	$(CC) $(CFLAGS) -fopenmp dip_seq.c Image.o FileIO.o DIPs.o utils.o -o dip_seq

dip_mp_for : dip_mp_for.c Image.o FileIO.o DIPs.o utils.o
	$(CC) $(CFLAGS) -fopenmp dip_mp_for.c Image.o FileIO.o DIPs.o utils.o -o dip_mp_for

dip_mp_tasks : dip_mp_tasks.c Image.o FileIO.o DIPs.o utils.o
	$(CC) $(CFLAGS) -fopenmp dip_mp_tasks.c Image.o FileIO.o DIPs.o utils.o -o dip_mp_tasks

Image.o : Image.c Image.h
	$(CC) -c $(CFLAGS) Image.c -o Image.o

FileIO.o : FileIO.c FileIO.h
	$(CC) -c $(CFLAGS) FileIO.c -o FileIO.o

DIPs.o : DIPs.c DIPs.h
	$(CC) -c $(CFLAGS) DIPs.c -o DIPs.o

Test.o : Test.c Test.h
	$(CC) -c $(CFLAGS) Test.c -o Test.o

utils.o : utils.c utils.h
	$(CC) -c $(CFLAGS) -fopenmp utils.c -o utils.o

clean : 
	$(RM) $(TARGETS)

.PHONY : clean
