CXX = g++
CXXFLAGS = -std=c++17 -O2 -Werror -Iinclude -MMD -MP

BUILD_DIR = build

SRC = $(shell find src include -name "*.cpp")
OBJ = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRC))
DEP = $(OBJ:.o=.d)

TARGET = main

# Default empty
CASE ?=

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEP)

.PHONY: run clean

run: $(TARGET)
	ifeq($(strip $(CASE)),)
		./$(TARGET)
	else
		./$(TARGET) $(CASE)
	endif

clean:
	rm -rf $(TARGET) $(BUILD_DIR)
	rm -rf ./output
