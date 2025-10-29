#ifndef MATURESTATE_H
#define MATURESTATE_H

#include "PlantState.h"

class Plant;

/**
 * @class MatureState
 * @brief Concrete state representing the mature stage of a plant's lifecycle
 * 
 * This class implements the State pattern for plants that have reached full maturity.
 * Mature plants are fully grown, ready for sale, and represent the most stable stage
 * in the plant lifecycle. They may transition to flowering or remain mature.
 * 
 * Characteristics of mature state:
 * - Fully grown and ready for sale
 * - Should be moved from Greenhouse to SalesFloor
 * - Stable condition with moderate care requirements
 * - May transition to FloweringState after extended time with excellent health (age ≥ 35 days, health ≥ 80%)
 * - Alerts when water level is low (< 20%)
 * - May transition to DeadState if health drops critically low (<10%)
 */
class MatureState : public PlantState {
public:
    /**
     * @brief Constructor for MatureState
     * 
     * Prints a message indicating the plant has entered the mature state
     * and is ready for sale
     */
    MatureState();
    
    /**
     * @brief Destructor for MatureState
     */
    ~MatureState();
    
    /**
     * @brief Handle mature-specific behavior and state transitions
     * 
     * Manages the behavior of plants in the mature stage:
     * - Checks if plant has died (health < 10%) → transitions to DeadState
     * - Checks if plant should begin flowering (age ≥ 35 days, health ≥ 80%) → transitions to FloweringState
     * - Marks plant as ready for sale
     * - Alerts if water level is low (< 20%)
     * 
     * @param plant Pointer to the plant in mature state
     */
    void handleChange(Plant* plant) override;

    /**
     * @brief Get the name of this state
     * 
     * @return std::string Returns "Mature"
     */
    std::string getStateName() override;
};

#endif