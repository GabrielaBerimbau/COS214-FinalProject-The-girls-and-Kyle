#include "include/DeadState.h"
#include "include/Plant.h"
#include <iostream>

/**
 * @brief Constructor for DeadState
 * 
 * Initializes the dead state and outputs a message indicating
 * the plant has died and entered the terminal state.
 */
DeadState::DeadState() {
    std::cout << "Plant has died." << std::endl;
}

/**
 * @brief Destructor for DeadState
 * 
 * Cleans up the dead state object. No special cleanup required.
 */
DeadState::~DeadState() {
}

/**
 * @brief Handle dead state behavior (terminal state with no transitions)
 * 
 * Manages the plant in its terminal dead state by:
 * 1. Marking the plant as not ready for sale (cannot be sold)
 * 2. Setting the plant's price to 0 (dead plants have no value)
 * 3. Printing a notification that the plant should be removed from inventory
 * 
 * This is a terminal state - no state transitions occur from here.
 * Dead plants cannot be revived and should be removed from the greenhouse
 * or sales floor to free up space for healthy plants.
 * 
 * @param plant Pointer to the dead plant whose state is being managed
 */
void DeadState::handleChange(Plant* plant) {
    // Mark plant as not ready for sale
    plant->setReadyForSale(false);
    
    // Set price to 0 (dead plants have no value)
    plant->setPrice(0.0);
    
    // Notify that plant should be removed from inventory
    std::cout << "Plant " << plant->getID() << " is dead and should be removed from inventory." << std::endl;
}

/**
 * @brief Get the name of this state
 * 
 * Returns a string identifier for the dead state, used for
 * display purposes and state identification throughout the system.
 * 
 * @return std::string Returns "Dead"
 */
std::string DeadState::getStateName() {
    return "Dead";
}