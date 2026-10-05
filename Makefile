CC = gcc
CFLAGS = -Wall -Wextra

TARGET = build/cron

OBJ = build/main.o \
			build/shell.o \
			build/executor.o \
			build/parser.o 

SRC = src/main.c src/shell.c src/parser.c src/executor.c

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

build/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o

build/shell.o: src/shell.c
	$(CC) $(CFLAGS) -c src/shell.c -o build/shell.o

build.parser.0: src/parser.c
	$(CC) $(CFLAGS) -c src/parser.c -o build/parser.o

build/executor.o: src/executor.c
	$(CC) $(CFLAGS) -c executor.c -o executor.o

clean:
	rm -rf $(TARGET)
