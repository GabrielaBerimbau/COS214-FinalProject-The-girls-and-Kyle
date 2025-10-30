# Plant Nursery Management System

![Lint](https://github.com/GabrielaBerimbau/COS214-FinalProject-The-girls-and-Kyle/actions/workflows/lint.yml/badge.svg?branch=Dev) ![Build](https://github.com/GabrielaBerimbau/COS214-FinalProject-The-girls-and-Kyle/actions/workflows/build.yml/badge.svg?branch=Dev) ![Test](https://github.com/GabrielaBerimbau/COS214-FinalProject-The-girls-and-Kyle/actions/workflows/test.yml/badge.svg?branch=Dev) ![Coverage](https://github.com/GabrielaBerimbau/COS214-FinalProject-The-girls-and-Kyle/actions/workflows/coverage.yml/badge.svg?branch=Dev)

A comprehensive C++ simulation of a plant nursery management system, demonstrating advanced object-oriented design through the implementation of multiple design patterns to handle plant inventory, customer interactions, staff coordination, and automated plant care.

---

## Table of Contents

- [Overview](#overview)
- [Getting Started](#getting-started)
- [How to Use](#how-to-use)
- [System Architecture](#system-architecture)
- [Testing](#testing)
- [Project Structure](#project-structure)
- [Troubleshooting](#troubleshooting)
- [Contributors](#contributors)
- [Technical Details](#technical-details)
---

## Overview

### Project Description

This project simulates a fully functional plant nursery store where customers can browse plants, make purchases, and interact with staff members. The system manages the complete lifecycle of plants from seedling to mature specimens, coordinates sales floor operations, and processes customer orders through various payment methods.

### Key Features

**Plant Management System**

- Multiple plant categories: Flowers (Rose, Daisy, Strelitzia), Succulents (Cactus, Aloe), Vegetables (Potato, Radish), and specialty plants (Monstera, Venus Fly Trap)
- Dynamic growth states: Seedling → Growing → Mature → Flowering → Dead
- Automated care scheduling based on plant type and current condition
- Health monitoring with water level, nutrient level, and sunlight exposure tracking

**Customer Experience**

- Browse plants on an interactive sales floor
- Add plants to shopping cart with position-based or name-based selection
- Customize purchases with decorations (ribbons, gift wrap, decorative pots)
- Build complex orders using the Composite pattern
- Complete checkout with multiple payment options (Cash, Credit Card, Mobile Payment)

**Staff Coordination**

- Hierarchical request handling: Sales Assistant → Floor Manager → Nursery Owner
- Automatic request routing based on complexity level
- Direct customer assistance through the Mediator pattern
- Inventory management between greenhouse and sales floor

**Automated Operations**

- Observer-based plant health monitoring
- Command pattern for care task scheduling
- Automatic plant relocation from greenhouse to sales floor when ready
- Real-time inventory tracking

### Interactive Demo

The project includes a graphical user interface built with Raylib that provides a visual, interactive shopping experience with three main phases:

1. **Shopping Phase**: Grid-based plant selection from sales floor
2. **Customization Phase**: Add decorative elements to selected plants
3. **Checkout Phase**: Payment selection and transaction completion

---

## Getting Started

### Prerequisites

Before you begin, ensure you have the following installed on your system:

**For Linux/WSL (Ubuntu 20.04 or later):**

- GCC/G++ compiler
- Make build system
- Git version control
- X11 development libraries
- OpenGL libraries
- Audio libraries (ALSA)

**For macOS (10.15 or later):**

- Xcode Command Line Tools
- Homebrew package manager
- CMake build system
- Git version control

### Installation Steps

The setup process is automated through a setup script that handles all dependencies and submodule initialization.

1. **Clone the Repository**
    
    ```bash
    git clone https://github.com/GabrielaBerimbau/COS214-FinalProject-The-girls-and-Kyle
    cd COS214-FinalProject-The-girls-and-Kyle
    ```
    
2. **Run the Setup Script**
    
    ```bash
    chmod +x setup.sh
    ./setup.sh
    ```
    
    The setup script will:
    
    - Detect your operating system automatically
    - Install all required system dependencies
    - Initialize and update git submodules (Raylib for graphics, Google Test for unit testing)
    - Configure the build environment
3. **Build and Run**
    
    ```bash
    make gui
    ```
    

The setup process typically takes 2-5 minutes depending on your internet connection and system specifications.

---

## How-to-Use

### Available Commands

The project uses a Makefile-based build system with several convenient commands:

```bash
make gui          # Build and run the interactive graphical demo
make testing      # Build and run the console-based testing program
make test         # Build and run the complete unit test suite
make test-verbose # Run tests with detailed timing information
make valgrind     # Run memory leak detection on TestingMain
make clean        # Remove all build artifacts
make rebuild      # Clean and rebuild everything
make help         # Display all available commands
```

### Using the Graphical Demo

The GUI demo provides an interactive shopping experience:

**Phase 1: Shopping**

- The sales floor is displayed as a grid with plants
- Click on any plant to add it to your shopping cart
- Plants in your cart are highlighted in gold
- Click again to remove a plant from your cart
- Your current budget and cart total are displayed on the left
- Click "CHECKOUT" when ready to proceed

**Phase 2: Customization**

- View each plant in your cart individually
- Choose from three decorative pot options (Blue, Red, Terracotta)
- Each decoration adds to the total price
- Navigate between plants using "NEXT PLANT"
- Click "FINISH" when satisfied with your customizations
- Use "BACK" to return to shopping

**Phase 3: Checkout**

- Review your total purchase amount
- Select a payment method: Cash, Credit Card, or Mobile Payment
- Confirm the transaction
- Plants are removed from the sales floor inventory
- Budget is updated to reflect the purchase

### Using the Testing Program

The console-based testing program demonstrates the system's capabilities through a scripted scenario:

```bash
make testing
```

This program showcases:

- Customer creation and initialization
- Adding plants from the sales floor (by name and position)
- Applying multiple decorators to cart items
- Returning plants to the sales floor
- Staff request handling through the chain of responsibility
- Building composite orders from cart contents
- Finalizing orders with the Prototype pattern
- Budget checking and transaction processing

The program outputs detailed logs of each operation, making it useful for understanding how different components interact.

---

## System-Architecture

### Design Patterns Implementation

This project implements 12 core design patterns, each serving a specific purpose in the system:

#### Creational Patterns

**Factory Method Pattern**

- **Purpose**: Creates different types of plants with their appropriate care strategies and initial states
- **Implementation**: Abstract `PlantFactory` with concrete factories for each plant type (RoseFactory, CactusFactory, etc.)
- **Benefits**: Centralizes plant creation logic, ensures consistent initialization

**Prototype Pattern**

- **Purpose**: Clones existing orders for repeat purchases
- **Implementation**: `FinalOrder::clone()` creates deep copies of complex order structures
- **Benefits**: Allows customers to quickly reorder favorite combinations

#### Structural Patterns

**Decorator Pattern**

- **Purpose**: Dynamically adds decorative features to plants without modifying base plant classes
- **Implementation**: `RibbonDecorator`, `GiftWrapDecorator`, `DecorativePotDecorator` wrap plant objects
- **Benefits**: Flexible customization, price calculations automatically update

**Composite Pattern**

- **Purpose**: Treats individual plants and groups of plants uniformly in orders
- **Implementation**: `Order` interface with `Leaf` (single plant) and `ConcreteOrder` (group) implementations
- **Benefits**: Builds complex nested order structures, simplifies price calculation

**Facade Pattern**

- **Purpose**: Provides simplified interfaces to complex subsystems
- **Implementation**: Customer class facade for cart operations, order building, and checkout
- **Benefits**: Reduces coupling, easier to use and test

#### Behavioral Patterns

**Strategy Pattern**

- **Purpose**: Defines different care algorithms for different plant types
- **Implementation**: `CareStrategy` interface with `FlowerCareStrategy`, `SucculentCareStrategy`, `VegetableCareStrategy`, `OtherPlantCareStrategy`
- **Benefits**: Encapsulates care logic, easily add new plant care types

**State Pattern**

- **Purpose**: Allows plants to change behavior based on their growth stage
- **Implementation**: `PlantState` with `SeedlingState`, `GrowingState`, `MatureState`, `FloweringState`, `DeadState`
- **Benefits**: State-specific logic is isolated, transitions are explicit

**Observer Pattern**

- **Purpose**: Monitors plant conditions and triggers care actions
- **Implementation**: `PlantObserver` interface with `WaterObserver`, `FertilizeObserver`, `SunlightObserver`
- **Benefits**: Decouples monitoring from action execution, automatic response to changes

**Command Pattern**

- **Purpose**: Encapsulates care actions as objects for scheduling and execution
- **Implementation**: `Command` interface with `WaterPlantCommand`, `FertilizePlantCommand`, `AdjustSunlightCommand`
- **Benefits**: Queues tasks, supports undo operations, separates invoker from executor

**Iterator Pattern**

- **Purpose**: Traverses composite order structures without exposing internal representation
- **Implementation**: `ConcreteIterator` flattens nested orders into sequential access
- **Benefits**: Uniform traversal of complex structures, used in price calculation

**Mediator Pattern**

- **Purpose**: Coordinates communication between greenhouse, sales floor, and customers
- **Implementation**: `NurseryMediator` and `NurseryCoordinator` manage object interactions
- **Benefits**: Reduces coupling, centralizes complex communication logic

**Chain of Responsibility Pattern**

- **Purpose**: Passes customer requests through a hierarchy of staff members
- **Implementation**: `SalesAssistant` → `FloorManager` → `NurseryOwner`
- **Benefits**: Decouples senders from receivers, handlers can be reordered

**Template Method Pattern**

- **Purpose**: Defines the skeleton of payment processing algorithm
- **Implementation**: `PaymentProcessor` abstract class with `CashPayment` and `CreditCardPayment` implementations
- **Benefits**: Ensures consistent payment workflow, subclasses customize specific steps

### Component Interaction

The system is organized into several interconnected subsystems:

**Plant Management Layer**

- Factory creates plants with appropriate strategies and states
- Plants notify observers when conditions change
- Observers create commands for the care scheduler
- Commands execute care strategies on plants

**Inventory Layer**

- Greenhouse stores growing plants in a grid structure
- Sales floor displays ready-for-sale plants
- Mediator coordinates plant transfers between locations
- Automated relocation when plants reach mature state

**Customer Interaction Layer**

- Customers browse and select plants through the mediator
- Decorator pattern adds customizations to cart items
- Composite pattern builds complex orders
- Iterator pattern traverses orders for price calculation

**Staff Management Layer**

- Chain of responsibility handles customer requests by complexity
- Sales assistants handle simple queries
- Floor managers handle bulk orders and special arrangements
- Nursery owner handles complaints and refunds

---

## Testing

### Test Suite Overview

The project includes a comprehensive test suite built with Google Test, covering all major components and design patterns.

### Running Tests

**Basic Test Execution**

```bash
make test                        # Run all tests with summary output
make test-verbose                # Run tests with detailed timing information
make test-filter FILTER='Factory*'  # Run only tests matching the filter pattern
```

**Memory Leak Detection**

```bash
make valgrind                    # Run TestingMain with Valgrind memory analysis
```

The Valgrind target provides detailed memory analysis including:

- Heap usage statistics
- Memory leak detection (definitely lost, indirectly lost, possibly lost)
- Error summary
- Full report saved to `valgrind-out.txt`

### Test Coverage

The test suite includes:

**Factory Pattern Tests** (`FactoryTests.cpp`)

- Validates all plant factories create correct plant types
- Verifies proper strategy assignment for each plant category
- Tests observer attachment when scheduler is provided
- Ensures unique plant IDs across factory instances

**Decorator Pattern Tests** (`DecoratorTests.cpp`)

- Tests single and stacked decorators
- Validates price calculations with multiple decorations
- Verifies description accumulation through decorator chain
- Tests decorator behavior on edge cases (zero price plants)

**Composite Pattern Tests** (`CompositeTests.cpp`)

- Tests leaf nodes and composite orders
- Validates price calculation through nested structures
- Tests adding and removing items from orders
- Verifies clone functionality creates deep copies

**Iterator Pattern Tests** (`IteratorTests.cpp`)

- Tests iteration over flat and nested order structures
- Validates iterator reset functionality
- Tests multiple independent iterators on same structure
- Verifies price calculation using iterator traversal

**Customer Workflow Tests** (`CustomerTests.cpp`)

- Tests cart management operations
- Validates plant decoration workflow
- Tests order building and finalization
- Verifies budget management and payment processing
- Tests staff request handling integration

**Template Method Tests** (`TestingTemplateMethod.cpp`)

- Validates payment processor workflow
- Tests cash and credit card payment implementations
- Verifies null order safety handling

All tests follow the Arrange-Act-Assert pattern and include both positive and negative test cases.

---

## Project-Structure

### Directory Organization

```
plant-nursery/
├── src/                    # Implementation files (.cpp)
│   ├── Plant.cpp          # Base plant class
│   ├── *Factory.cpp       # Factory implementations
│   ├── *Strategy.cpp      # Care strategy implementations
│   ├── *State.cpp         # Plant state implementations
│   ├── *Observer.cpp      # Observer implementations
│   ├── *Command.cpp       # Command implementations
│   ├── *Decorator.cpp     # Decorator implementations
│   ├── Customer.cpp       # Customer and derived classes
│   ├── Staff*.cpp         # Staff hierarchy implementations
│   ├── Greenhouse.cpp     # Greenhouse management
│   ├── SalesFloor.cpp     # Sales floor operations
│   ├── *Mediator.cpp      # Mediator implementations
│   ├── Order classes      # Composite pattern implementations
│   ├── Payment*.cpp       # Template method implementations
│   ├── TestingMain.cpp    # Console demo program
│   └── Demo.cpp           # GUI demo program
│
├── include/               # Header files (.h)
│   └── [Mirrors src structure]
│
├── tests/                 # Google Test unit tests
│   ├── test_main.cpp     # Test runner entry point
│   ├── FactoryTests.cpp
│   ├── DecoratorTests.cpp
│   ├── CompositeTests.cpp
│   ├── IteratorTests.cpp
│   ├── CustomerTests.cpp
│   └── TestingTemplateMethod.cpp
│
├── GUI/                   # Raylib GUI implementation
│   └── Demo.cpp          # Graphical interface code
│
├── external/              # Git submodules (not committed)
│   ├── raylib/           # Graphics library
│   └── googletest/       # Testing framework
│
├── build/                 # Compiled output (generated)
│   ├── *.o               # Object files
│   ├── TestingMain       # Console executable
│   ├── PlantShopGUI      # GUI executable
│   └── RunTests          # Test suite executable
│
├── makefile              # Build configuration
├── setup.sh              # Automated setup script
├── .gitignore           # Git ignore patterns
├── .gitmodules          # Submodule configuration
└── README.md            # This file
```

### Key Files Description

**TestingMain.cpp**: Console-based demonstration program that walks through a complete customer shopping scenario with detailed logging.

**Demo.cpp**: Interactive graphical interface using Raylib, providing a visual shopping experience with three phases (shopping, customization, checkout).

**Customer.cpp**: Facade implementation that simplifies complex interactions between customers, the sales floor, and the mediator.

**NurseryCoordinator.cpp**: Concrete mediator that manages all communication between greenhouse, sales floor, customers, and staff.

**makefile**: Comprehensive build system supporting multiple platforms, automatic dependency detection, and various build targets.

---

## Troubleshooting

### Common Issues and Solutions

**Permission Denied When Running setup.sh**

```bash
chmod +x setup.sh
./setup.sh
```

The script needs execute permissions before it can run.

**Build Fails on macOS**

First, ensure Xcode Command Line Tools are installed:

```bash
xcode-select --install
```

If Homebrew is not installed:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

After installing Homebrew, run the setup script again.

**Build Fails After Successful Setup**

Try a clean rebuild:

```bash
make clean
make gui
```

If the issue persists, you may need to clean external library builds:

```bash
make clean-all
./setup.sh
make gui
```

**Submodule Errors**

If you encounter git submodule errors:

```bash
git submodule update --init --recursive --force
```

**Raylib Not Building on Linux**

Ensure all X11 development packages are installed:

```bash
sudo apt-get update
sudo apt-get install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev
```

**CMake Not Found (Linux/WSL)**

If you get a "cmake: command not found" error when building Google Test:

```bash
sudo apt-get update
sudo apt-get install cmake
```

After installing cmake, reinitialize the submodules:

```bash
cd external/googletest
mkdir -p build
cd build
cmake ..
make
```

**CMake Not Found (macOS)**

The setup.sh script should install cmake automatically via Homebrew. If it didn't:

```bash
brew install cmake
```

**Tests Fail to Compile**

Ensure Google Test submodule is properly initialized:

```bash
cd external/googletest
mkdir -p build
cd build
cmake ..
make
```

**Memory Leak False Positives with Valgrind**

Some memory leaks reported by Valgrind may be from external libraries. Focus on leaks in your own code (files in `src/` or `include/`).

---

## Contributors

**The Girls and Kyle**

This project was developed as part of the COS214 (Design Patterns) course to demonstrate practical implementation of software design patterns in a cohesive, real-world-inspired system.

---

## Technical-Details

**Language**: C++17  
**Graphics Library**: Raylib 4.5  
**Testing Framework**: Google Test  
**Build System**: GNU Make  
**Supported Platforms**: Linux (Ubuntu 20.04+), macOS (10.15+), WSL  
**License**: Academic Use

---

## Acknowledgments

This project demonstrates the practical application of design patterns taught in COS214, showcasing:

- Object-oriented design principles (SOLID, DRY, KISS)
- Memory management in C++ (RAII, smart pointer patterns)
- Software architecture and component interaction
- Unit testing and test-driven development
- Cross-platform development considerations