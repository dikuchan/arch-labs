CC=gcc
CFLAGS=-Wall -Wextra -fopenmp
OBJECTS=build

all: hello parhello directives data for

hello: hello.c
	$(CC) hello.c -o $(OBJECTS)/hello

parhello: hello.c
	$(CC) $(CFLAGS) hello.c -o $(OBJECTS)/parhello

directives: directives.c
	$(CC) $(CFLAGS) directives.c -o $(OBJECTS)/directives

data: data.c
	$(CC) $(CFLAGS) data.c -o $(OBJECTS)/data

for: for.c
	$(CC) $(CFLAGS) for.c -o $(OBJECTS)/for

clean:
	rm -r $(OBJECTS)
