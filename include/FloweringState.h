#ifndef FLOWERINGSTATE_H
#define FLOWERINGSTATE_H

#include "PlantState.h"

class Plant;

/**
 * @class FloweringState
 * @brief Concrete state representing the flowering stage of a plant's lifecycle
 * 
 * This class implements the State pattern for plants in their flowering stage.
 * Flowering plants are at their premium quality, displaying blooms and requiring
 * specific care to maintain their appearance.
 * 
 * Characteristics of flowering state:
 * - Premium quality with enhanced pricing (1.5x base price)
 * - Ready for sale to customers
 * - Requires careful maintenance (water ≥ 30%, nutrients ≥ 30%, sunlight ≥ 50%)
 * - Transitions back to MatureState after flowering period ends (approximately 50 days total age)
 * - May transition to DeadState if health drops critically low (<10%)
 */
class FloweringState : public PlantState {
public:
    /**
     * @brief Constructor for FloweringState
     * 
     * Prints a message indicating the plant has entered the premium flowering state
     */
    FloweringState();
    
    /**
     * @brief Destructor for FloweringState
     */
    ~FloweringState();
    
    /**
     * @brief Handle flowering-specific behavior and state transitions
     * 
     * Manages the behavior of plants in the flowering stage:
     * - Checks if plant has died (health < 10%) → transitions to DeadState
     * - Marks plant as ready for sale with premium pricing
     * - Applies premium price multiplier (1.5x) if not already applied
     * - Checks if flowering period is over (age ≥ 50 days) → transitions to MatureState
     * - Alerts if water level is low (< 30%)
     * - Alerts if nutrient level is low (< 30%)
     * - Alerts if sunlight exposure is insufficient (< 50%)
     * 
     * @param plant Pointer to the plant in flowering state
     */
    void handleChange(Plant* plant) override;

    /**
     * @brief Get the name of this state
     * 
     * @return std::string Returns "Flowering"
     */
    std::string getStateName() override;
};

#endif