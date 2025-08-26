#CFLAGS=-c -g -Wall -pg
CFLAGS=-g -Wall

LDFLAGS=-lSDL2 -lSDL2_ttf -no-pie

SOURCES=glh2menu.c

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=glh2menu

CC=gcc
CFLAGS+=-D_GNU_SOURCE=1

all: 
	$(MAKE) main
	$(MAKE) cleanobjs


main: $(SOURCES) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS) 
	cp $(OBJECTS) $(EXECUTABLE)

.c.o:
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) 

cleanobjs:
	rm -f $(OBJECTS)

clean:
	-rm -f glh2menu *.o

