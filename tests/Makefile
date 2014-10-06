CC = gcc -g
CFLAGS = -c -Wall -Werror
LDFLAGS = -lcrypto

SOURCES = run.c

OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf run.o
