#ifndef SEEDLINGSTATE_H
#define SEEDLINGSTATE_H

#include "PlantState.h"
#include <string>

class Plant;

/**
 * @class SeedlingState
 * @brief Concrete state representing the seedling stage of a plant's lifecycle
 * @author Sofia Finlayson
 * @date 2025-10-26
 */
class SeedlingState : public PlantState {
public:
    /**
     * @brief Constructor for SeedlingState
     * 
     * Prints a message indicating the plant has entered the seedling state
     * Initializes the seedling state and outputs a message indicating
     * the plant has entered this stage of growth.
     */
    SeedlingState();
    
    /**
     * @brief Destructor for SeedlingState
     * 
     * Cleans up the seedling state object.
     */
    ~SeedlingState();
    
    /**
     * @brief Handle seedling-specific behavior and state transitions
     * 
     * Manages the behavior of plants in the seedling stage:
     * - Checks if plant has died (health < 10%) → transitions to DeadState
     * - Checks if seedling is ready to grow (age ≥ 7 days, health ≥ 50%) → transitions to GrowingState
     * - Alerts if water level is critically low (< 40%)
     * 
     * @param plant Pointer to the plant in seedling state
     */
    void handleChange(Plant* plant) override;

    /**
     * @brief Get the name of this state
     * 
     * Returns a string identifier for the seedling state, used for
     * display purposes and state identification throughout the system.
     * 
     * @return std::string Returns "Seedling"
     */
    std::string getStateName() override;
};

#endif