CC       ?= gcc
CPPFLAGS ?=
CFLAGS   ?= -std=gnu99 -g -O0 -Wall -Werror --coverage -fprofile-arcs -ftest-coverage
LDFLAGS  ?= -lgcov

all: libavl.so main

libavl.so: avl.h avl.c avl_insert.c avl_remove.c avl_util.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -fPIC -o avl.o avl.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -fPIC -o avl_insert.o avl_insert.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -fPIC -o avl_remove.o avl_remove.c
	$(CC) -shared -o libavl.so avl.o avl_insert.o avl_remove.o $(LDFLAGS)

main: main.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o main main.c -L$(PWD) -lavl $(LDFLAGS)

clean:
	$(RM) libavl.so avl.o avl_insert.o avl_remove.o main *.gcno

.PHONY: all clean
