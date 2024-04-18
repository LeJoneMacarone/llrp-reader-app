CC := g++
C_FLAGS := -g -Wall -L lib/ltkcpp

SRC_DIR := ./src
BUILD_DIR := ./build
BIN_DIR := ./bin
LIB_DIR := ./lib

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/**/*.cpp)
OBJ_FILES := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(filter %.cpp, $(subst /, ,$(SRC_FILES))))
BIN_FILES := $(BIN_DIR)/main
LIB_FILES := $(wildcard $(LIB_DIR)/**/*.a)

SUFFIX := _x86_64

LIBS := $(filter %$(SUFFIX).a, $(LIB_FILES))
LIB_FLAGS := -l dl -l ssl -l crypto -l curl

.PHONY: build clean

$(BUILD_DIR)/%.o: $(SRC_DIR)/*/%.cpp
	$(CC) $(C_FLAGS) -c $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(C_FLAGS) -c $^ -o $@

$(BIN_FILES): $(OBJ_FILES)
	$(CC) $(C_FLAGS) $^ $(LIBS) $(LIB_FLAGS) -o $@ 

build: $(BIN_FILES)

clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(BIN_DIR)/*
