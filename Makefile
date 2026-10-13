CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -D_DEFAULT_SOURCE
TARGET = myls
SOURCES = src/main.c src/options.c src/list.c
OBJECTS = $(SOURCES:.c=.o)
INCLUDES = -Iinclude

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
