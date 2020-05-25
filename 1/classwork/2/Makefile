CC=gcc
CFLAGS=-Wall -Wextra -fopenmp
OBJECTS=build

all: integration

integration: integration.c
	$(CC) $(CFLAGS) integration.c -o $(OBJECTS)/integration

clean:
	rm -r $(OBJECTS)