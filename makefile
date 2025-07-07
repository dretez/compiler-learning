NAME := compiler

SRC_DIR := src
HEADER_DIR := header
BUILD_DIR := build
OBJ_DIR := obj

SRC_EXT := c
HEADER_EXT := h

SRCS := $(shell find $(SRC_DIR) -name '*.$(SRC_EXT)')
OBJS := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%.o,$(SRCS))

CC := gcc
CFLAGS := -Wall -Wextra -iquote $(shell pwd)/$(HEADER_DIR)

all: $(BUILD_DIR)/$(NAME)

$(BUILD_DIR)/$(NAME): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	@echo Linking object files
	@$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo Compiling $@
	@$(CC) $(CFLAGS) -c $< -o $@

wc:
	@wc $(SRCS) $(shell find $(HEADER_DIR) -name '*.$(HEADER_EXT)') | tail -1

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)

debug: CFLAGS += -g3
debug: $(BUILD_DIR)/$(NAME)

.PHONY: all debug clean wc
