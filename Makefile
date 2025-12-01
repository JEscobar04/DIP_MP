CC = gcc
CFLAGS ?= -Wall -ansi
TARGETS = PhotoLab.o Image.o FileIO.o DIPs.o Test.o
RM = rm -f

ifeq ($(DEBUG), 1)
	CFLAGS += -g
endif

build : src/Image src/FileIO src/DIPs src/Test src/PhotoLab
	$(CC) $(TARGETS) -o PhotoLab

Image : src/Image.c src/Image.h
	$(CC) -c $(CFLAGS) src/Image.c -o Image.o

FileIO : src/FileIO.c src/FileIO.h
	$(CC) -c $(CFLAGS) src/FileIO.c -o FileIO.o

DIPs : src/DIPs.c src/DIPs.h
	$(CC) -c $(CFLAGS) src/DIPs.c -o DIPs.o

Test : src/Test.c src/Test.h
	$(CC) -c $(CFLAGS) src/Test.c -o Test.o

PhotoLab : src/PhotoLab.c
	$(CC) -c $(CFLAGS) src/PhotoLab.c -o PhotoLab.o
.PHONY : PhotoLab

clean : 
	$(RM) $(TARGETS)

.PHONY : clean
