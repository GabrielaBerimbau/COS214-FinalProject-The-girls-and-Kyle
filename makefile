# ============================================================================
# Plant Shop Project Makefile with Google Test Support
# ============================================================================

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
TEST_FLAGS = -pthread

# Raylib linking flags
RAYLIB_LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Google Test setup (local installation in external/)
GTEST_DIR = external/googletest
GTEST_INCLUDE = $(GTEST_DIR)/googletest/include
GTEST_LIB_DIR = $(GTEST_DIR)/build/lib
GTEST_LIBS = -L$(GTEST_LIB_DIR) -lgtest -lgtest_main -pthread

# Directories
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests
BUILD_DIR = build

# Find all source files
# Exclude Demo.cpp and TestingMain.cpp (they have main() functions)
COMMON_SOURCES = $(filter-out $(SRC_DIR)/TestingMain.cpp $(SRC_DIR)/Demo.cpp, \
                 $(wildcard $(SRC_DIR)/*.cpp))
COMMON_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(COMMON_SOURCES))

# Find all test files
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/test_%.o, $(TEST_SOURCES))

# Executables
MAIN_EXEC = $(BUILD_DIR)/TestingMain
DEMO_EXEC = $(BUILD_DIR)/Demo
TEST_EXEC = $(BUILD_DIR)/RunTests

# ============================================================================
# TARGETS
# ============================================================================

# Default target - builds main programs (not tests)
all: $(MAIN_EXEC) $(DEMO_EXEC)
	@echo ""
	@echo "✓ Build complete!"
	@echo "  Run programs:"
	@echo "    make run       - Run TestingMain"
	@echo "    make run-demo  - Run Demo"
	@echo "  Run tests:"
	@echo "    make test      - Run all tests"

# Build everything including tests
all-with-tests: all $(TEST_EXEC)

# ============================================================================
# GOOGLE TEST SETUP
# ============================================================================

# Build Google Test library (only needed once)
$(GTEST_LIB_DIR)/libgtest.a:
	@echo "Building Google Test (first time only, this takes ~1 minute)..."
	@mkdir -p $(GTEST_DIR)/build
	@cd $(GTEST_DIR)/build && cmake .. > /dev/null && $(MAKE) > /dev/null
	@echo "✓ Google Test built successfully!"

# ============================================================================
# COMPILATION RULES
# ============================================================================

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compile common object files (your classes)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test object files (with gtest includes)
$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR) $(GTEST_LIB_DIR)/libgtest.a
	@echo "Compiling test $<..."
	@$(CXX) $(CXXFLAGS) -I$(GTEST_INCLUDE) -c $< -o $@

# ============================================================================
# EXECUTABLES
# ============================================================================

# Build TestingMain (no raylib, no gtest)
$(MAIN_EXEC): $(SRC_DIR)/TestingMain.cpp $(COMMON_OBJECTS) | $(BUILD_DIR)
	@echo "Linking TestingMain..."
	@$(CXX) $(CXXFLAGS) $(SRC_DIR)/TestingMain.cpp $(COMMON_OBJECTS) -o $(MAIN_EXEC)

# Build Demo (with raylib)
$(DEMO_EXEC): $(SRC_DIR)/Demo.cpp $(COMMON_OBJECTS) | $(BUILD_DIR)
	@echo "Linking Demo..."
	@$(CXX) $(CXXFLAGS) $(SRC_DIR)/Demo.cpp $(COMMON_OBJECTS) -o $(DEMO_EXEC) $(RAYLIB_LIBS)

# Build test executable (with gtest)
$(TEST_EXEC): $(COMMON_OBJECTS) $(TEST_OBJECTS) | $(BUILD_DIR) $(GTEST_LIB_DIR)/libgtest.a
	@echo "Linking tests..."
	@$(CXX) $(CXXFLAGS) $(TEST_FLAGS) $(COMMON_OBJECTS) $(TEST_OBJECTS) \
	-o $(TEST_EXEC) $(GTEST_LIBS)

# ============================================================================
# RUN TARGETS
# ============================================================================

# Run TestingMain
run: $(MAIN_EXEC)
	@echo "Running TestingMain..."
	@./$(MAIN_EXEC)

# Run Demo with raylib
run-demo: $(DEMO_EXEC)
	@echo "Running Demo..."
	@./$(DEMO_EXEC)

# Run all tests
test: $(TEST_EXEC)
	@echo "Running all tests..."
	@./$(TEST_EXEC)

# Run tests with timing information
test-verbose: $(TEST_EXEC)
	@echo "Running tests (verbose mode)..."
	@./$(TEST_EXEC) --gtest_print_time=1

# Run specific test(s) using filter
# Usage: make test-filter FILTER="TestName*"
test-filter: $(TEST_EXEC)
	@echo "Running filtered tests: $(FILTER)"
	@./$(TEST_EXEC) --gtest_filter=$(FILTER)

# Run tests with color output (useful for CI/CD)
test-color: $(TEST_EXEC)
	@./$(TEST_EXEC) --gtest_color=yes

# ============================================================================
# UTILITY TARGETS
# ============================================================================

# Clean build artifacts (keeps gtest built)
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)
	@echo "✓ Clean complete"

# Clean everything including gtest
clean-all: clean
	@echo "Cleaning Google Test build..."
	@rm -rf $(GTEST_DIR)/build
	@echo "✓ Deep clean complete"

# Rebuild from scratch
rebuild: clean all

# Rebuild everything including gtest
rebuild-all: clean-all all-with-tests

# Show detected files (useful for debugging makefile)
show-sources:
	@echo "Common sources:"
	@echo "  $(COMMON_SOURCES)"
	@echo ""
	@echo "Common objects:"
	@echo "  $(COMMON_OBJECTS)"
	@echo ""
	@echo "Test sources:"
	@echo "  $(TEST_SOURCES)"
	@echo ""
	@echo "Test objects:"
	@echo "  $(TEST_OBJECTS)"

# Display help information
help:
	@echo "Plant Shop Project - Available Commands"
	@echo "========================================"
	@echo ""
	@echo "Building:"
	@echo "  make              - Build TestingMain and Demo"
	@echo "  make all-with-tests - Build everything including tests"
	@echo ""
	@echo "Running Programs:"
	@echo "  make run          - Run TestingMain"
	@echo "  make run-demo     - Run Demo (raylib)"
	@echo ""
	@echo "Testing:"
	@echo "  make test         - Run all tests"
	@echo "  make test-verbose - Run tests with timing info"
	@echo "  make test-filter FILTER='TestName*' - Run specific tests"
	@echo "  make test-color   - Run tests with color output"
	@echo ""
	@echo "Cleaning:"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make clean-all    - Remove build + gtest build"
	@echo "  make rebuild      - Clean and rebuild"
	@echo "  make rebuild-all  - Clean everything and rebuild"
	@echo ""
	@echo "Utilities:"
	@echo "  make show-sources - Show detected source files"
	@echo "  make help         - Show this help message"

# ============================================================================
# PHONY TARGETS (not actual files)
# ============================================================================

.PHONY: all all-with-tests run run-demo test test-verbose test-filter test-color \
        clean clean-all rebuild rebuild-all show-sources help