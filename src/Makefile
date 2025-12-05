# Simple Makefile for a single C program

# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -Wextra -O2
LDFLAGS :=

# Target program name
TARGET  := test

# Source and object files
SRCS    := test.c
OBJS    := $(SRCS:.c=.o)

# Default rule: build the program
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile step (C → .o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Remove build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Convenience: rebuild from scratch
rebuild: clean all

.PHONY: all clean rebuild
