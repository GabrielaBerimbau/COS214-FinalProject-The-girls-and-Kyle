# ============================================================================
# Plant Shop Project Makefile with Google Test and Raylib Support
# Both are local submodules - no system installation needed!
# ============================================================================

# Compiler and flags
CXX = g++
CXXFLAGS += -std=c++17 -Wall -Wextra -I. -Iinclude
TEST_FLAGS = -pthread
LDFLAGS ?=

# ============================================================================
# RAYLIB SETUP (local installation in external/)
# ============================================================================

RAYLIB_DIR = external/raylib
RAYLIB_INCLUDE = $(RAYLIB_DIR)/src
RAYLIB_LIB_DIR = $(RAYLIB_DIR)/src
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)
    # macOS linking flags
    RAYLIB_LIBS = -L$(RAYLIB_LIB_DIR) -lraylib \
                  -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else
    # Linux linking flags
    RAYLIB_LIBS = -L$(RAYLIB_LIB_DIR) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

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
GUI_DIR = GUI
BUILD_DIR = build

# ============================================================================
# SOURCE FILES
# ============================================================================

# Find all source files (exclude main programs)
COMMON_SOURCES = $(filter-out $(SRC_DIR)/TestingMain.cpp $(SRC_DIR)/DemoMain.cpp $(SRC_DIR)/Demo.cpp, \
                 $(wildcard $(SRC_DIR)/*.cpp))
COMMON_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(COMMON_SOURCES))

# Find all test files
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/test_%.o, $(TEST_SOURCES))

# Find all GUI files
GUI_SOURCES = $(wildcard $(GUI_DIR)/*.cpp)
GUI_OBJECTS = $(patsubst $(GUI_DIR)/%.cpp, $(BUILD_DIR)/gui_%.o, $(GUI_SOURCES))

# ============================================================================
# EXECUTABLES
# ============================================================================

TESTING_EXEC = $(BUILD_DIR)/TestingMain
DEMO_EXEC = $(BUILD_DIR)/DemoMain
GUI_EXEC = $(BUILD_DIR)/PlantShopGUI
TEST_EXEC = $(BUILD_DIR)/RunTests

# ============================================================================
# VALGRIND CONFIGURATION
# ============================================================================

VALGRIND = valgrind
VALGRIND_FLAGS = --leak-check=full \
                 --show-leak-kinds=all \
                 --track-origins=yes \
                 --verbose \
                 --log-file=valgrind-out.txt

# ============================================================================
# MAIN TARGETS
# ============================================================================

# Default target - show help
all: help

# Build everything
build-all: $(TESTING_EXEC) $(GUI_EXEC)

# ============================================================================
# EXTERNAL LIBRARY BUILDS
# ============================================================================

# Build Raylib (first time only)
$(RAYLIB_LIB_DIR)/libraylib.a:
	@echo "Building Raylib..."
	@cd $(RAYLIB_DIR)/src && $(MAKE) PLATFORM=PLATFORM_DESKTOP

# Build Google Test (first time only)
$(GTEST_LIB_DIR)/libgtest.a:
	@echo "Building Google Test..."
	@mkdir -p $(GTEST_DIR)/build
	@cd $(GTEST_DIR)/build && cmake .. > /dev/null && $(MAKE) > /dev/null

# ============================================================================
# COMPILATION RULES
# ============================================================================

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compile common object files (backend)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile GUI object files (with raylib includes AND GUI directory for headers)
$(BUILD_DIR)/gui_%.o: $(GUI_DIR)/%.cpp | $(BUILD_DIR) $(RAYLIB_LIB_DIR)/libraylib.a
	@echo "Compiling GUI $<..."
	@$(CXX) $(CXXFLAGS) -I$(RAYLIB_INCLUDE) -I$(GUI_DIR) -c $< -o $@

# Compile test object files (with gtest includes)
$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR) $(GTEST_LIB_DIR)/libgtest.a
	@echo "Compiling test $<..."
	@$(CXX) $(CXXFLAGS) -I$(GTEST_INCLUDE) -c $< -o $@

# ============================================================================
# EXECUTABLE LINKING
# ============================================================================

# Build TestingMain
$(TESTING_EXEC): $(SRC_DIR)/TestingMain.cpp $(COMMON_OBJECTS) | $(BUILD_DIR)
	@echo "Building TestingMain..."
	@$(CXX) $(CXXFLAGS) $(SRC_DIR)/TestingMain.cpp $(COMMON_OBJECTS) -o $(TESTING_EXEC) $(LDFLAGS)
	@echo "✓ TestingMain built successfully!"

# Build DemoMain (placeholder for future)
$(DEMO_EXEC): $(SRC_DIR)/DemoMain.cpp $(COMMON_OBJECTS) | $(BUILD_DIR)
	@echo "Building DemoMain..."
	@$(CXX) $(CXXFLAGS) $(SRC_DIR)/DemoMain.cpp $(COMMON_OBJECTS) -o $(DEMO_EXEC) $(LDFLAGS)
	@echo "✓ DemoMain built successfully!"

# Build GUI application (with raylib)
$(GUI_EXEC): $(COMMON_OBJECTS) $(GUI_OBJECTS) | $(BUILD_DIR) $(RAYLIB_LIB_DIR)/libraylib.a
	@echo "Building Plant Shop GUI..."
	@$(CXX) $(CXXFLAGS) -I$(RAYLIB_INCLUDE) $(GUI_OBJECTS) $(COMMON_OBJECTS) \
	-o $(GUI_EXEC) $(RAYLIB_LIBS) $(LDFLAGS)
	@echo "✓ Plant Shop GUI built successfully!"

# Build test executable (with gtest)
$(TEST_EXEC): $(COMMON_OBJECTS) $(TEST_OBJECTS) | $(BUILD_DIR) $(GTEST_LIB_DIR)/libgtest.a
	@echo "Building tests..."
	@$(CXX) $(CXXFLAGS) $(TEST_FLAGS) $(COMMON_OBJECTS) $(TEST_OBJECTS) \
-o $(TEST_EXEC) $(GTEST_LIBS) $(LDFLAGS)
	@echo "✓ Tests built successfully!"

# ============================================================================
# RUN TARGETS
# ============================================================================

# Run TestingMain
testing: $(TESTING_EXEC)
	@echo ""
	@echo "========================================="
	@echo "   Running TestingMain"
	@echo "========================================="
	@./$(TESTING_EXEC)

# Run DemoMain
demo: $(DEMO_EXEC)
	@echo ""
	@echo "========================================="
	@echo "   Running DemoMain"
	@echo "========================================="
	@./$(DEMO_EXEC)

# Run GUI application
gui: $(GUI_EXEC)
	@echo ""
	@echo "========================================="
	@echo "   Running Plant Shop GUI"
	@echo "========================================="
	@./$(GUI_EXEC)

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
# VALGRIND TARGET
# ============================================================================

# Run TestingMain with Valgrind
valgrind: $(TESTING_EXEC)
	@echo "Running TestingMain with Valgrind..."
	@echo "═══════════════════════════════════════════════════════════════"
	@$(VALGRIND) $(VALGRIND_FLAGS) ./$(TESTING_EXEC)
	@echo ""
	@echo "═══════════════════════════════════════════════════════════════"
	@echo "                    MEMORY ANALYSIS REPORT"
	@echo "═══════════════════════════════════════════════════════════════"
	@echo ""
	@echo "HEAP USAGE:"
	@grep "total heap usage" valgrind-out.txt | sed 's/^==.*== */  /' || echo "  No data"
	@grep "in use at exit" valgrind-out.txt | sed 's/^==.*== */  /' || echo "  No data"
	@echo ""
	@echo "LEAK SUMMARY:"
	@grep -A 5 "LEAK SUMMARY" valgrind-out.txt | grep -E "(definitely|indirectly|possibly|still reachable)" | sed 's/^==.*== */  /' || echo "  No leaks"
	@echo ""
	@echo "ERROR SUMMARY:"
	@grep "ERROR SUMMARY" valgrind-out.txt | sed 's/^==.*== */  /' || echo "  No errors"
	@echo ""
	@echo "═══════════════════════════════════════════════════════════════"
	@if grep -q "All heap blocks were freed" valgrind-out.txt; then \
		echo "✓ SUCCESS: All memory properly freed!"; \
	elif grep -q "definitely lost: 0 bytes" valgrind-out.txt; then \
		echo "✓ SUCCESS: No memory leaks detected!"; \
	else \
		echo "✗ FAILED: Memory leaks detected - check valgrind-out.txt"; \
	fi
	@echo ""
	@echo "Full report saved to: valgrind-out.txt"
	@echo "═══════════════════════════════════════════════════════════════"

# ============================================================================
# UTILITY TARGETS
# ============================================================================

# Clean build artifacts (keeps external libraries built)
clean:
	@rm -rf $(BUILD_DIR)
	@rm -f valgrind-out.txt
	@echo "✓ Build artifacts cleaned"

# Clean everything including external library builds
clean-all: clean
	@rm -rf $(GTEST_DIR)/build
	@cd $(RAYLIB_DIR)/src && $(MAKE) clean
	@echo "✓ Everything cleaned"

# Rebuild from scratch
rebuild: clean build-all

# Rebuild everything including external libraries
rebuild-all: clean-all build-all

# Show detected files
show-sources:
	@echo "Common sources:"
	@echo "  $(COMMON_SOURCES)"
	@echo ""
	@echo "Common objects:"
	@echo "  $(COMMON_OBJECTS)"
	@echo ""
	@echo "GUI sources:"
	@echo "  $(GUI_SOURCES)"
	@echo ""
	@echo "GUI objects:"
	@echo "  $(GUI_OBJECTS)"
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
	@echo "Primary Targets:"
	@echo "  make testing      - Build and run TestingMain"
	@echo "  make gui          - Build and run Plant Shop GUI"
	@echo "  make demo         - Build and run DemoMain (future)"
	@echo ""
	@echo "Building:"
	@echo "  make build-all    - Build all executables"
	@echo ""
	@echo "Testing:"
	@echo "  make test         - Run all tests"
	@echo "  make test-verbose - Run tests with timing info"
	@echo "  make test-filter FILTER='TestName*' - Run specific tests"
	@echo ""
	@echo "Memory Checking:"
	@echo "  make valgrind     - Run TestingMain with memory leak detection"
	@echo ""
	@echo "Cleaning:"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make clean-all    - Remove build + external builds"
	@echo "  make rebuild      - Clean and rebuild all"
	@echo "  make rebuild-all  - Clean everything and rebuild"
	@echo ""
	@echo "Documentation:"
	@echo "  make docs         - Generate Doxygen documentation and open in browser"
	@echo "  make clean-docs   - Remove generated documentation"
	@echo ""
	@echo "Utilities:"
	@echo "  make show-sources - Show detected source files"
	@echo "  make help         - Show this help message"


# ============================================================================
# Doxygen documentation
# ============================================================================
docs:
	@echo "Generating Doxygen documentation..."
	@doxygen Doxyfile
	@echo "✓ Documentation generated successfully!"

clean-docs:
	rm -rf docs/html docs/latex

# ============================================================================
# PHONY TARGETS
# ============================================================================

.PHONY: all build-all testing demo gui test test-verbose test-filter \
        clean clean-all rebuild rebuild-all show-sources help valgrind clean-docs