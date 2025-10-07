# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc

# Raylib linking flags
RAYLIB_LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Directories
SRC_DIR = src
BUILD_DIR = build

# Output executables
MAIN_EXEC = $(BUILD_DIR)/TestingMain
DEMO_EXEC = $(BUILD_DIR)/Demo

# Find all .cpp files in src directory (excluding TestingMain.cpp and Demo.cpp)
COMMON_SOURCES = $(filter-out $(SRC_DIR)/TestingMain.cpp $(SRC_DIR)/Demo.cpp, $(wildcard $(SRC_DIR)/*.cpp))
COMMON_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(COMMON_SOURCES))

# Main sources
MAIN_SRC = $(SRC_DIR)/TestingMain.cpp
DEMO_SRC = $(SRC_DIR)/Demo.cpp

# Default target
all: $(MAIN_EXEC) $(DEMO_EXEC)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile common object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile TestingMain executable (without raylib)
$(MAIN_EXEC): $(MAIN_SRC) $(COMMON_OBJECTS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) $(COMMON_OBJECTS) -o $(MAIN_EXEC)

# Compile Demo executable (with raylib)
$(DEMO_EXEC): $(DEMO_SRC) $(COMMON_OBJECTS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DEMO_SRC) $(COMMON_OBJECTS) -o $(DEMO_EXEC) $(RAYLIB_LIBS)

# Run TestingMain
run: $(MAIN_EXEC)
	./$(MAIN_EXEC)

# Run Demo
run-demo: $(DEMO_EXEC)
	./$(DEMO_EXEC)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Rebuild everything
rebuild: clean all

# Show detected source files (for debugging)
show-sources:
	@echo "Common sources: $(COMMON_SOURCES)"
	@echo "Common objects: $(COMMON_OBJECTS)"

.PHONY: all run run-Demo clean rebuild show-sources