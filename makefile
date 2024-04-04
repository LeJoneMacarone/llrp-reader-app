CC := g++
CFLAGS := -g -Wall
LIBFLAGS := -l curl

SRC_DIR := ./src
BUILD_DIR := ./build
BIN_DIR := ./bin

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/**/*.cpp)
OBJ_FILES := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(filter %.cpp, $(subst /, ,$(SRC_FILES))))
BIN_FILES := $(BIN_DIR)/main

.PHONY: build clean

$(BUILD_DIR)/%.o: $(SRC_DIR)/*/%.cpp
	$(CC) $(CFLAGS) -c $^ $(LIBFLAGS) -o $@
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $^ $(LIBFLAGS) -o $@

$(BIN_FILES): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ $(LIBFLAGS) -o $@

build: $(BIN_FILES)

clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(BIN_DIR)/*
