CC = gcc
CFLAGS = -nostdlib -I include
ENTRY = startProgram
TARGET = program
SRC = $(wildcard src/*.c)

all:
	@$(CC) $(CFLAGS) -e $(ENTRY) -o $(TARGET) $(SRC)

clean:
	@rm -f $(TARGET)