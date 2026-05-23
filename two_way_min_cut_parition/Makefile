CXX = g++
CXXFLAGS = -std=c++17 -Werror -Iinclud -MMD -MP

BUILD_DIR = build

SRC = $(shell find src include -name "*.cpp")
OBJ = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRC))
DEP = $(obj:.o=.d)

TARGET = main

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEP)

clean:
	rm -rf $(TARGET) $(BUILD_DIR)
