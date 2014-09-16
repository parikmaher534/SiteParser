CC = gcc
CFLAGS = -c -Wall 
LDFLAGS = -lcrypto

SOURCES = run.c
HEADERS = 

OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = run

all: $(SOURCES) $(EXECUTABLE) clean

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf run.o
