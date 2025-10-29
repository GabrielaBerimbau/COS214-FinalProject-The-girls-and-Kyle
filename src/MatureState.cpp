#include "include/MatureState.h"
#include "include/FloweringState.h"
#include "include/DeadState.h"
#include "include/Plant.h"
#include <iostream>

/**
 * @brief Constructor for MatureState
 * 
 * Initializes the mature state and outputs a message indicating
 * the plant has reached full maturity and is ready for sale.
 */
MatureState::MatureState() {
    std::cout << "Plant entered Mature state - ready for sale!" << std::endl;
}

/**
 * @brief Destructor for MatureState
 * 
 * Cleans up the mature state object. No special cleanup required.
 */
MatureState::~MatureState() {
}

/**
 * @brief Handle mature-specific behavior and state transitions
 * 
 * Manages the plant's behavior during the mature stage by:
 * 1. Checking if the plant has died due to poor health (health < 10%)
 *    - If so, transitions to DeadState
 * 2. Checking if the plant is ready to begin flowering (age ≥ 35 days, health ≥ 80%)
 *    - If conditions met, transitions to FloweringState
 * 3. Marking the plant as ready for sale
 * 4. Monitoring water levels and alerting if low (< 20%)
 * 
 * Mature plants represent the stable, sale-ready stage and should be
 * moved from the Greenhouse to the SalesFloor for customer purchase.
 * Plants with excellent health may transition to flowering.
 * 
 * @param plant Pointer to the plant whose state is being managed
 */
void MatureState::handleChange(Plant* plant) {
   int age = plant->getAge();
    int health = plant->getHealthLevel();
    
    // Check if plant has died due to poor health
    if (health < 10) {
        std::cout << "Mature plant " << plant->getID() << " has died due to poor health." << std::endl;
        plant->setState(new DeadState());
        //delete this;
        return;
    }
    
    // Check if plant is ready to transition to Flowering state
    if (age >= 35 && health >= 80) {
        std::cout << "Mature plant " << plant->getID() << " is starting to flower... transitioning to Flowering state." << std::endl;
        plant->setState(new FloweringState());
        //delete this;
        return;
    }
    
    // Mark plant as ready for sale
    plant->setReadyForSale(true);
    
    // Alert if water level is low
    if (plant->getWaterLevel() < 20) {
        std::cout << "Mature plant " << plant->getID() << " needs water." << std::endl;
    }
}

/**
 * @brief Get the name of this state
 * 
 * Returns a string identifier for the mature state, used for
 * display purposes and state identification throughout the system.
 * 
 * @return std::string Returns "Mature"
 */
std::string MatureState::getStateName() {
    return "Mature";
}