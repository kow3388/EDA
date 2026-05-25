CXX = g++
CXXFLAGS = -std=c++17 -Werror -Iinclude -MMD -MP

BUILD_DIR = build

SRC = $(shell find src include -name "*.cpp")
OBJ = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRC))
DEP = $(OBJ:.o=.d)

TARGET = main

# Default testcase (override via: make run CELL=... NET=...)
CELL ?= ./testcase/p2-1.cells
NET  ?= ./testcase/p2-1.nets

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEP)

.PHONY: run clean

run: $(TARGET)
	./$(TARGET) $(CELL) $(NET)

clean:
	rm -rf $(TARGET) $(BUILD_DIR)
