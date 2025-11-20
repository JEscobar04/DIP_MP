CC = gcc
CFLAGS ?= -Wall -ansi
TARGETS = PhotoLab.o Image.o FileIO.o DIPs.o Test.o
RM = rm -f

ifeq ($(DEBUG), 1)
	CFLAGS += -g
endif

build : Image FileIO DIPs Test PhotoLab
	$(CC) $(TARGETS) -o PhotoLab

Image : Image.c Image.h
	$(CC) -c $(CFLAGS) Image.c -o Image.o

FileIO : FileIO.c FileIO.h
	$(CC) -c $(CFLAGS) FileIO.c -o FileIO.o

DIPs : DIPs.c DIPs.h
	$(CC) -c $(CFLAGS) DIPs.c -o DIPs.o

Test : Test.c Test.h
	$(CC) -c $(CFLAGS) Test.c -o Test.o

PhotoLab : PhotoLab.c
	$(CC) -c $(CFLAGS) PhotoLab.c -o PhotoLab.o
.PHONY : PhotoLab

clean : 
	$(RM) $(TARGETS)

.PHONY : clean
