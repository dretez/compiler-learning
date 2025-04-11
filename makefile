NAME := compiler

SRC_DIR := src
HEADER_DIR := header
BUILD_DIR := build
OBJ_DIR := obj

SRC_EXT := c
HEADER_EXT := h

SRCS := $(shell find $(SRC_DIR) -name '*.$(SRC_EXT)')
OBJS := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%.o,$(SRCS))
# OBJS := $(SRCS:$(SRC_DIR)/%$(SRC_EXT)=$(OBJ_DIR)/%.o)

CC := gcc
# 0 = no debug info, 2 = default, 3 = max debug info
DEBUG_LEVEL := 0
CFLAGS := -Wall -Wextra -g$(DEBUG_LEVEL) -I $(shell pwd)

all: $(BUILD_DIR)/$(NAME)

$(BUILD_DIR)/$(NAME): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	@echo Linking object files
	@$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo Compiling $@
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)

.PHONY: all clean
