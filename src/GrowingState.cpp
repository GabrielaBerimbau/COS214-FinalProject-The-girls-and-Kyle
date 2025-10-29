#include "include/GrowingState.h"
#include "include/MatureState.h"
#include "include/DeadState.h"
#include "include/Plant.h"
#include <iostream>

/**
 * @brief Constructor for GrowingState
 * 
 * Initializes the growing state and outputs a message indicating
 * the plant has entered this active growth stage.
 */
GrowingState::GrowingState() {
    std::cout << "Plant entered Growing state." << std::endl;
}

/**
 * @brief Destructor for GrowingState
 * 
 * Cleans up the growing state object. No special cleanup required.
 */
GrowingState::~GrowingState() {
}

/**
 * @brief Handle growing-specific behavior and state transitions
 * 
 * Manages the plant's behavior during the active growth stage by:
 * 1. Checking if the plant has died due to poor health (health < 20%)
 *    - If so, transitions to DeadState
 * 2. Checking if the plant is ready to transition to mature stage
 *    - Requires: age ≥ 12 days AND health ≥ 50%
 *    - If conditions met, transitions to MatureState
 * 3. Monitoring water levels and alerting if low (< 30%)
 * 4. Monitoring nutrient levels and alerting if low (< 25%)
 * 
 * Growing plants require regular maintenance to ensure they develop
 * properly and reach maturity in good health.
 * 
 * @param plant Pointer to the plant whose state is being managed
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
 * 
 * Returns a string identifier for the growing state, used for
 * display purposes and state identification throughout the system.
 * 
 * @return std::string Returns "Growing"
 */
std::string GrowingState::getStateName() {
    return "Growing";
}