/**
 * @file Flower.h
 * @author Kyle McCalgan
 * @brief Declares the Flower class, representing flowering plants in the nursery.
 *
 * This class provides a concrete implementation of the Plant base class
 * for flower-type plants with specific care requirements and behaviors.
 */
#ifndef FLOWER_H
#define FLOWER_H

#include "Plant.h"

/**
 * @class Flower
 * @brief Represents a flowering plant in the nursery system.
 *
 * This class extends the Plant base class to represent flowers with
 * specific growth patterns and care requirements.
 */
class Flower : public Plant {
public:
    /**
     * @brief Constructs a new Flower object.
     * @param name The name of the flower.
     * @param id Unique identifier for the flower.
     * @param careStrategy Pointer to the care strategy for this flower.
     * @param initialState Pointer to the initial state of the flower.
     */
    Flower(const std::string& name, const std::string& id,
           CareStrategy* careStrategy, PlantState* initialState);

    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup of flower resources.
     */
    virtual ~Flower();

    /**
     * @brief Performs daily update operations specific to flowers.
     * Overrides the base Plant class implementation.
     */
    void dailyUpdate() override;

    /**
     * @brief Converts flower information to a string representation.
     * @return String containing flower-specific details.
     */
    std::string toString() const override;
};

#endif