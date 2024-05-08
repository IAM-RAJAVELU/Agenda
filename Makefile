CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC_DIR = src
DATA_DIR = data
BUILD_DIR = build

# List of source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
SRCS += $(wildcard $(DATA_DIR)/*.json)

# List of object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Name of the executable
TARGET = agenda_program

.PHONY: all clean

all: $(TARGET)

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
    $(CC) $(CFLAGS) -c $< -o $@

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJS) $(TARGET)
