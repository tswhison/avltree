CC       ?= gcc
CPPFLAGS ?=
CFLAGS   ?= -std=gnu99 -ggdb3 -O0 -Wall -Werror
LDFLAGS  ?=

all: libavl.so main

libavl.so: avl.h avl.c avl_insert.c avl_remove.c avl_util.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -fPIC -o avl.o avl.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -fPIC -o avl_insert.o avl_insert.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -fPIC -o avl_remove.o avl_remove.c
	$(CC) -shared -o libavl.so avl.o avl_insert.o avl_remove.o

main: main.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o main main.c -L$(PWD) -lavl

clean:
	$(RM) libavl.so avl.o avl_insert.o avl_remove.o main
	$(RM) -r cov mem

.PHONY: all clean
