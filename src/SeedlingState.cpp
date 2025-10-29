#include "include/SeedlingState.h"
#include "include/GrowingState.h"
#include "include/DeadState.h"
#include "include/Plant.h"
#include <iostream>

/**
 * @brief Constructor for SeedlingState
 * 
 * Initializes the seedling state and outputs a message indicating
 * the plant has entered this stage of growth.
 */
SeedlingState::SeedlingState() {
    std::cout << "Plant entered Seedling state." << std::endl;
}

/**
 * @brief Destructor for SeedlingState
 * 
 * Cleans up the seedling state object. No special cleanup required.
 */
SeedlingState::~SeedlingState() {
}

/**
 * @brief Handle seedling-specific behavior and state transitions
 * 
 * Manages the plant's behavior during the seedling stage by:
 * 1. Checking if the plant has died due to poor health (health < 20%)
 *    - If so, transitions to DeadState
 * 2. Checking if the seedling is ready to transition to growing stage
 *    - Requires: age ≥ 7 days AND health ≥ 50%
 *    - If conditions met, transitions to GrowingState
 * 3. Monitoring water levels and alerting if critically low (< 40%)
 * 
 * Seedlings are the most vulnerable stage and require frequent attention
 * to survive and progress to the next growth stage.
 * 
 * @param plant Pointer to the plant whose state is being managed
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
 * 
 * Returns a string identifier for the seedling state, used for
 * display purposes and state identification throughout the system.
 * 
 * @return std::string Returns "Seedling"
 */
std::string SeedlingState::getStateName() {
    return "Seedling";
}