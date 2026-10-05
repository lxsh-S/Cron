CC = gcc
CFLAGS = -Wall -Wextra

TARGET = build/cron

SRC = src/main.c src/shell.c src/parser.c src/executor.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -rf $(TARGET)
