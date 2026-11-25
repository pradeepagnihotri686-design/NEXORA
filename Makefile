CC     = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
SRC    = $(wildcard src/*.c)
TARGET = student360

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) data/*.dat data/*.csv
