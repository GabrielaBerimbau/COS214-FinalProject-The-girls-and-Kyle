/**
 * @file Succulent.h
 * @author Kyle McCalgan
 * @brief Declares the Succulent class, representing succulent plants in the nursery.
 *
 * This class provides a concrete implementation of the Plant base class
 * for succulent-type plants with specific care requirements and water retention.
 */
#ifndef SUCCULENT_H
#define SUCCULENT_H

#include "Plant.h"

/**
 * @class Succulent
 * @brief Represents a succulent plant in the nursery system.
 *
 * This class extends the Plant base class to represent succulents with
 * specific water storage capabilities and care requirements.
 */
class Succulent : public Plant {
public:
    /**
     * @brief Constructs a new Succulent object.
     * @param name The name of the succulent.
     * @param id Unique identifier for the succulent.
     * @param careStrategy Pointer to the care strategy for this succulent.
     * @param initialState Pointer to the initial state of the succulent.
     */
    Succulent(const std::string& name, const std::string& id,
              CareStrategy* careStrategy, PlantState* initialState);

    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup of succulent resources.
     */
    virtual ~Succulent();

    /**
     * @brief Performs daily update operations specific to succulents.
     * Overrides the base Plant class implementation.
     */
    void dailyUpdate() override;

    /**
     * @brief Converts succulent information to a string representation.
     * @return String containing succulent-specific details.
     */
    std::string toString() const override;
};

#endif