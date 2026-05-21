CC = gcc
CFLAGS = -Wall -Wextra -O2 -I./lib

BIN_DIR = bin
LIB_DIR = lib
APP_DIR = src

APP_DIRS = $(wildcard $(APP_DIR)/*)

BINS = $(patsubst $(APP_DIR)/%, $(BIN_DIR)/%, $(APP_DIRS))

LIB_OBJ = $(LIB_DIR)/utils.o

all: $(BIN_DIR) $(LIB_OBJ) $(BINS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(LIB_OBJ): $(LIB_DIR)/utils.c $(LIB_DIR)/utils.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%: $(APP_DIR)/%/main.c $(LIB_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(LIB_OBJ) -o $@

clean:
	rm -rf $(BIN_DIR)
	rm -f $(LIB_DIR)/*.o

.PHONY: all clean