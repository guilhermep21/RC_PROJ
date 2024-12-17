CC = gcc
CFLAGS = -Wall -Wextra

SRCDIR = .
SOURCES := $(shell find $(SRCDIR) -name '*.c')
OBJECTS := $(SOURCES:.c=.o)
EXECUTABLE = player

.PHONY: all clean run

all: clean $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: all
	./$(EXECUTABLE)