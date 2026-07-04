CXX = g++
CXXFLAGS = -std=c++17 -O2 -Werror -Iinclude -MMD -MP

BUILD_DIR = build

SRC = $(shell find src include -name "*.cpp")
OBJ = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRC))
DEP = $(OBJ:.o=.d)

TARGET = main

# Default empty
CASE ?=
RATIO ?=

CASE_SET  := $(if $(strip $(CASE)),1,0)
RATIO_SET := $(if $(strip $(RATIO)),1,0)

# Require both CASE and RATIO, or neither.
ifneq ($(CASE_SET)$(RATIO_SET),00)
ifneq ($(CASE_SET)$(RATIO_SET),11)
$(error Must provide both CASE and RATIO, or neither. Example: make run CASE=n200 RATIO=0.2)
endif
endif

RUN_ARGS :=
ifeq ($(CASE_SET)$(RATIO_SET),11)
RUN_ARGS := $(CASE) $(RATIO)
endif

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEP)

.PHONY: run clean

run: $(TARGET)
	./$(TARGET) $(RUN_ARGS)

clean:
	rm -rf $(TARGET) $(BUILD_DIR)
	rm -rf ./output
