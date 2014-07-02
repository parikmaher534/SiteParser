CC=gcc
CFLAGS=-c -Wall 
#-Wno-pointer-sign
LDFLAGS= -lcrypto -I/usr/include/libxml2 -lxml2
SOURCES=main.c
HEADERS=main.h
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@


