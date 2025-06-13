
# Makefile zum Kompilieren aller .c-Dateien im src-Verzeichnis

# Verzeichnisse und Dateien
SRC_DIR := src
SRCS    := $(wildcard $(SRC_DIR)/*.c)
OBJS    := $(SRCS:.c=.o)
TARGET  := pfusch

# Compiler-Einstellungen
CC      := clang
CFLAGS  := -Wall -Wextra -std=c17 -I$(SRC_DIR)

.PHONY: all clean

# Standardziel
all: $(TARGET)

# Linken der Objektdateien
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Kompilieren jeder .c-Datei
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Aufräumen
clean:
	rm -f $(OBJS) $(TARGET)
