# ---- Compiler and Flags ----
CC     = gcc
CFLAGS = -Wall -Wextra -std=c11

# ---- Project Settings ----
# Executable name = current folder name
TARGET = $(notdir $(CURDIR))

# All .c files in the folder
SRC = $(wildcard *.c)

# Convert .c → .o
OBJ = $(SRC:.c=.o)

# ---- Build Rules ----

# Default: build the target
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Pattern rule for compiling .c → .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ---- Utility Commands ----

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run
