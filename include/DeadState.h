#ifndef DEAD_STATE_H
#define DEAD_STATE_H

#include "PlantState.h"

class Plant;

/**
 * @class DeadState
 * @brief Concrete state representing the terminal stage of a plant's lifecycle
 * @author Sofia Finlayson
 * @date 2025-10-26
 * 
 * This class implements the State pattern for plants that have died due to
 * poor health or neglect. This is a terminal state with no further transitions.
 * 
 * Characteristics of dead state:
 * - Terminal state (no transitions to other states)
 * - Plant is not ready for sale
 * - Price is set to 0 (no value)
 * - Plant should be removed from inventory
 * - No care actions can revive the plant
 */
class DeadState : public PlantState {
public:
    /**
     * @brief Constructor for DeadState
     * 
     * Prints a message indicating the plant has died
     */
    DeadState();
    
    /**
     * @brief Destructor for DeadState
     * 
     * Cleans up the dead state object.
     */
    ~DeadState();
    
    /**
     * @brief Handle dead state behavior (no transitions)
     * 
     * Manages the plant in its terminal dead state by:
     * - Marking plant as not ready for sale
     * - Setting price to 0 (dead plants have no value)
     * - Printing a message that the plant should be removed from inventory
     * - No state transitions occur (terminal state)
     * 
     * Dead plants cannot be revived and should be removed from the system.
     * 
     * @param plant Pointer to the dead plant
     */
    void handleChange(Plant* plant) override;

    /**
     * @brief Get the name of this state
     * 
     * Returns a string identifier for the dead state, used for
     * display purposes and state identification throughout the system.
     * 
     * @return std::string Returns "Dead"
     */
    std::string getStateName() override;
};

#endif