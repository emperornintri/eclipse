CC = gcc
CFLAGS = -nostdlib -I include -Wno-builtin-declaration-mismatch -march=native -mtune=native
ENTRY = startProgram
TARGET = program
SRC = $(wildcard src/*.c)

all:
	@$(CC) $(CFLAGS) -e $(ENTRY) -o $(TARGET) $(SRC)

clean:
	@rm -f $(TARGET)