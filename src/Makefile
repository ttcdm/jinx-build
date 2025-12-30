# Simple Makefile for a single C program

# Compiler and flags
CC      ?=
CFLAGS  ?= -Wall -Wextra -g -O0
LDFLAGS ?=


override CFLAGS += \
	-fno-PIC \
	-fno-pie \
	-ffreestanding \
	-Wall \
	-Wextra \
	-std=gnu11 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-PIC \
	-fno-pie \
	-mno-80387 \
	-mno-mmx \
	-mno-red-zone \
	-m64 \
	-Wall \
	-Wextra \
	-g \
	-O0

override CPPFLAGS += \
    $(CPPFLAGS) \
    -MMD \
    -MP
# #
override LDFLAGS += \
	-fno-pie \
    -static \
    -z max-page-size=0x1000 \
# 	-fno-pie

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