CXX := g++
CLANG_TIDY := clang-tidy
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
CPPFLAGS := -Iinclude

SRC_DIR := src
BUILD_DIR := build
TARGET_DIR := target

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC))
TARGET := $(TARGET_DIR)/fcppm

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(TARGET_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CLANG_TIDY) $< -- -Iinclude
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET_DIR)

install:
	@mkdir -p /usr/local/bin
	cp $(TARGET) /usr/local/bin/fcppm

.PHONY: all tidy clean install
