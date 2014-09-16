#Application variables
CC = gcc
CFLAGS = -c -Wall 
#-Wno-pointer-sign
LDFLAGS = -lcrypto
SOURCES = main.c
HEADERS = main.h
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = main


#Commands
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
