# ============================================================================
# Plant Shop Project Makefile with Google Test and Raylib Support
# Both are local submodules - no system installation needed!
# ============================================================================

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I.
TEST_FLAGS = -pthread

# ============================================================================
# RAYLIB SETUP (local installation in external/)
# ============================================================================

RAYLIB_DIR = external/raylib
RAYLIB_INCLUDE = $(RAYLIB_DIR)/src
RAYLIB_LIB_DIR = $(RAYLIB_DIR)/src
RAYLIB_LIBS = -L$(RAYLIB_LIB_DIR) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# ============================================================================
# GOOGLE TEST SETUP (local installation in external/)
# ============================================================================

GTEST_DIR = external/googletest
GTEST_INCLUDE = $(GTEST_DIR)/googletest/include
GTEST_LIB_DIR = $(GTEST_DIR)/build/lib
GTEST_LIBS = -L$(GTEST_LIB_DIR) -lgtest -lgtest_main -pthread

# ============================================================================
# DIRECTORIES
# ============================================================================

SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests
BUILD_DIR = build

# ============================================================================
# SOURCE FILES
# ============================================================================

# Find all source files (exclude main programs)
COMMON_SOURCES = $(filter-out $(SRC_DIR)/TestingMain.cpp $(SRC_DIR)/Demo.cpp, \
                 $(wildcard $(SRC_DIR)/*.cpp))
COMMON_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(COMMON_SOURCES))

# Find all test files
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/test_%.o, $(TEST_SOURCES))

# ============================================================================
# EXECUTABLES
# ============================================================================

MAIN_EXEC = $(BUILD_DIR)/TestingMain
DEMO_EXEC = $(BUILD_DIR)/Demo
TEST_EXEC = $(BUILD_DIR)/RunTests

# ============================================================================
# MAIN TARGETS
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
# EXTERNAL LIBRARY BUILDS
# ============================================================================

# Build Raylib (first time only)
$(RAYLIB_LIB_DIR)/libraylib.a:
	@cd $(RAYLIB_DIR)/src && $(MAKE) PLATFORM=PLATFORM_DESKTOP


# Build Google Test (first time only)
$(GTEST_LIB_DIR)/libgtest.a:
	@mkdir -p $(GTEST_DIR)/build
	@cd $(GTEST_DIR)/build && cmake .. > /dev/null && $(MAKE) > /dev/null


# ============================================================================
# COMPILATION RULES
# ============================================================================

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compile common object files (with raylib includes)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -I$(RAYLIB_INCLUDE) -c $< -o $@

# Compile test object files (with gtest includes)
$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR) $(GTEST_LIB_DIR)/libgtest.a
	@$(CXX) $(CXXFLAGS) -I$(GTEST_INCLUDE) -c $< -o $@

# ============================================================================
# EXECUTABLE LINKING
# ============================================================================

# Build TestingMain (no raylib, no gtest)
$(MAIN_EXEC): $(SRC_DIR)/TestingMain.cpp $(COMMON_OBJECTS) | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) $(SRC_DIR)/TestingMain.cpp $(COMMON_OBJECTS) -o $(MAIN_EXEC)

# Build Demo (with raylib)
$(DEMO_EXEC): $(SRC_DIR)/Demo.cpp $(COMMON_OBJECTS) | $(BUILD_DIR) $(RAYLIB_LIB_DIR)/libraylib.a
	@$(CXX) $(CXXFLAGS) -I$(RAYLIB_INCLUDE) $(SRC_DIR)/Demo.cpp $(COMMON_OBJECTS) \
	-o $(DEMO_EXEC) $(RAYLIB_LIBS)

# Build test executable (with gtest)
$(TEST_EXEC): $(COMMON_OBJECTS) $(TEST_OBJECTS) | $(BUILD_DIR) $(GTEST_LIB_DIR)/libgtest.a
	@$(CXX) $(CXXFLAGS) $(TEST_FLAGS) $(COMMON_OBJECTS) $(TEST_OBJECTS) \
	-o $(TEST_EXEC) $(GTEST_LIBS)

# ============================================================================
# RUN TARGETS
# ============================================================================

# Run TestingMain
run: $(MAIN_EXEC)
	@./$(MAIN_EXEC)

# Run Demo with raylib
run-demo: $(DEMO_EXEC)
	@./$(DEMO_EXEC)

# Run all tests
test: $(TEST_EXEC)
	@./$(TEST_EXEC)

# Run tests with timing information
test-verbose: $(TEST_EXEC)
	@./$(TEST_EXEC) --gtest_print_time=1

# Run specific test(s) using filter
test-filter: $(TEST_EXEC)
	@./$(TEST_EXEC) --gtest_filter=$(FILTER)

# ============================================================================
# UTILITY TARGETS
# ============================================================================

# Clean build artifacts (keeps external libraries built)
clean:
	@rm -rf $(BUILD_DIR)

# Clean everything including external library builds
clean-all: clean
	@rm -rf $(GTEST_DIR)/build
	@cd $(RAYLIB_DIR)/src && $(MAKE) clean


# Rebuild from scratch
rebuild: clean all

# Rebuild everything including external libraries
rebuild-all: clean-all all-with-tests

# Show detected files
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

# Display help
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
	@echo ""
	@echo "Cleaning:"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make clean-all    - Remove build + external builds"
	@echo "  make rebuild      - Clean and rebuild"
	@echo "  make rebuild-all  - Clean everything and rebuild"
	@echo ""
	@echo "Utilities:"
	@echo "  make show-sources - Show detected source files"
	@echo "  make help         - Show this help message"

# ============================================================================
# PHONY TARGETS
# ============================================================================

.PHONY: all all-with-tests run run-demo test test-verbose test-filter \
        clean clean-all rebuild rebuild-all show-sources help