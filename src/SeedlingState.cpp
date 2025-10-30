#include "include/SeedlingState.h"
#include "include/GrowingState.h"
#include "include/DeadState.h"
#include "include/Plant.h"
#include <iostream>

/**
 * @author Sofia Finlayson
 * @date 2025-10-26
 * 
 * @brief Constructor for SeedlingState
 */
SeedlingState::SeedlingState() {
    std::cout << "Plant entered Seedling state." << std::endl;
}

/**
 * @brief Destructor for SeedlingState
 * 
 */
SeedlingState::~SeedlingState() {
}

/**
 * @brief Handle seedling-specific behavior and state transitions
 */
void SeedlingState::handleChange(Plant* plant) {
    int age = plant->getAge();
    int health = plant->getHealthLevel();
    
    // Check if plant has died due to poor health
    if (health < 20) {
        std::cout << "Seedling " << plant->getID() << " has died due to poor health." << std::endl;
        plant->setState(new DeadState());
        return;
    }
    
    // Check if seedling is ready to transition to Growing state
    if (age >= 7 && health >= 50) {
        std::cout << "Seedling " << plant->getID() << " is growing... transitioning to Growing state." << std::endl;
        plant->setState(new GrowingState());
        return;
    }
    
    // Alert if water level is critically low
    if (plant->getWaterLevel() < 40) {
        std::cout << "Seedling " << plant->getID() << " needs water urgently!" << std::endl;
    }
}

/**
 * @brief Get the name of this state
 */
std::string SeedlingState::getStateName() {
    return "Seedling";
}