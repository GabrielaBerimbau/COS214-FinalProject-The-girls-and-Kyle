#include "include/GrowingState.h"
#include "include/MatureState.h"
#include "include/DeadState.h"
#include "include/Plant.h"
#include <iostream>

/**
 * @author Sofia Finlayson
 * @date 2025-10-26
 * 
 * @brief Constructor for GrowingState
 */
GrowingState::GrowingState() {
    std::cout << "Plant entered Growing state." << std::endl;
}

/**
 * @brief Destructor for GrowingState
 */
GrowingState::~GrowingState() {
}

/**
 * @brief Handle growing-specific behavior and state transitions
 */
void GrowingState::handleChange(Plant* plant) {
    
    int age = plant->getAge();
    int health = plant->getHealthLevel();
    
    // Check if plant has died due to poor health
    if (health < 20) {
        std::cout << "Growing plant " << plant->getID() << " has died due to poor health." << std::endl;
        plant->setState(new DeadState());
        return;
    }
    
    // Check if plant is ready to transition to Mature state
    if (age >= 12 && health >= 50) {
        std::cout << "Plant " << plant->getID() << " is maturing... transitioning to Mature state." << std::endl;
        plant->setState(new MatureState());
        return;
    }
    
    // Alert if water level is low
    if (plant->getWaterLevel() < 30) {
        std::cout << "Growing plant " << plant->getID() << " needs water." << std::endl;
    }
    
    // Alert if nutrient level is low
    if (plant->getNutrientLevel() < 25) {
        std::cout << "Growing plant " << plant->getID() << " needs fertilizer." << std::endl;
    }
}

/**
 * @brief Get the name of this state
 */
std::string GrowingState::getStateName() {
    return "Growing";
}