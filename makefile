# compiler
CC := gcc
CFLAGS := -g -Wall -l curl

# Directories
SRC_DIR := ./src
BUILD_DIR := ./build
BIN_DIR := ./bin

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/**/*.cpp)
OBJ_FILES := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(filter %.cpp, $(subst /, ,$(SRC_FILES))))
BIN_FILES := $(BIN_DIR)/main

.PHONY: build clean

# Compile and assemble the artifacts from src/ to build/
build: $(OBJ_FILES)

$(BUILD_DIR)/%.o: $(SRC_DIR)/*/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(BIN_DIR)/*
