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

build/main.o: src/main.c src/shell.h
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o

build/shell.o: src/shell.c src/shell.h src/parser.h src/executor.h
	$(CC) $(CFLAGS) -c src/shell.c -o build/shell.o

build/parser.0: src/parser.c src/parser.h
	$(CC) $(CFLAGS) -c src/parser.c -o build/parser.o

build/executor.o: src/executor.c src/executor.h
	$(CC) $(CFLAGS) -c src/executor.c -o build/executor.o

clean:
	rm -rf $(TARGET)
