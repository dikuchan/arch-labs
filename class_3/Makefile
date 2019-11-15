CC=gcc
CFLAGS=-Wall -Wextra -fopenmp
OBJECTS=build

all: dynamic collision

dynamic: dynamic.c
	$(CC) $(CFLAGS) dynamic.c -o $(OBJECTS)/dynamic

collision: collision.c
	$(CC) $(CFLAGS) collision.c -o $(OBJECTS)/collision

clean:
	rm -r $(OBJECTS)
