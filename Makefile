CC = gcc
CFLAGS = -Wall -Wextra

TARGET = build/cron
.DEFAULT_GOAL := $(TARGET)

OBJ = build/main.o \
			build/shell.o \
			build/executor.o \
			build/parser.o \
			build/tokenizer.o \
			build/builtins.o \
			build/expansion.o \
			build/history.o \
			build/line_editor.o \
			build/prompt.o \
			build/completion.o

build:
	mkdir -p build

$(OBJ): | build

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

build/main.o: src/main.c src/shell.h
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o

build/shell.o: src/shell.c src/shell.h src/parser.h src/executor.h src/line_editor.h src/prompt.h
	$(CC) $(CFLAGS) -c src/shell.c -o build/shell.o

build/parser.o: src/parser.c src/parser.h
	$(CC) $(CFLAGS) -c src/parser.c -o build/parser.o

build/executor.o: src/executor.c src/executor.h
	$(CC) $(CFLAGS) -c src/executor.c -o build/executor.o

build/tokenizer.o: src/tokenizer.c src/tokenizer.h
	$(CC) $(CFLAGS) -c src/tokenizer.c -o build/tokenizer.o

build/builtins.o: src/builtins.c src/builtins.h
	$(CC) $(CFLAGS) -c src/builtins.c -o build/builtins.o

build/expansion.o: src/expansion.c src/expansion.h
	$(CC) $(CFLAGS) -c src/expansion.c -o build/expansion.o

build/history.o: src/history.c src/history.h
	$(CC) $(CFLAGS) -c src/history.c -o build/history.o

build/line_editor.o: src/line_editor.c src/line_editor.h src/history.h src/completion.h
	$(CC) $(CFLAGS) -c src/line_editor.c -o build/line_editor.o

build/prompt.o: src/prompt.c src/prompt.h
	$(CC) $(CFLAGS) -c src/prompt.c -o build/prompt.o

build/completion.o: src/completion.c src/completion.h
	$(CC) $(CFLAGS) -c src/completion.c -o build/completion.o

clean:
	rm -rf build/*.o build/cron
