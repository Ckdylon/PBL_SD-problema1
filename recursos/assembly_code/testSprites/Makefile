# Makefile for compiling testNewSprites

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source files
SRCS = testNewSprites.c graphic_processor.c

# Header files (optional, for clarity)
HEADERS = graphic_processor.h hps_0.h

# Output executable
TARGET = testNewSprites

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(SRCS) $(HEADERS)
		$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean target to remove compiled files
clean:
		rm -f $(TARGET) *.o

# Phony targets
.PHONY: all clean

