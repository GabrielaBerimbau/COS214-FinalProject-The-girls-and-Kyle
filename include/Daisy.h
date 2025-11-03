/**
 * @file Daisy.h
 * @author Kyle McCalgan
 * @brief Declares the Daisy class, a specific type of flower.
 *
 * This class represents a daisy plant with specific petal color and variety attributes,
 * extending the Flower base class.
 */
#ifndef DAISY_H
#define DAISY_H

#include "Flower.h"

/**
 * @class Daisy
 * @brief Represents a daisy plant in the nursery system.
 *
 * This class extends the Flower class to represent daisies with
 * specific petal colors and variety types.
 */
class Daisy : public Flower {
public:
    /**
     * @brief Constructs a new Daisy object.
     * @param id Unique identifier for the daisy.
     * @param careStrategy Pointer to the care strategy for this daisy.
     * @param initialState Pointer to the initial state of the daisy.
     * @param petalColor The color of the daisy petals.
     * @param varietyType The variety type of daisy (defaults to "Common").
     */
    Daisy(const std::string& id, CareStrategy* careStrategy,
          PlantState* initialState, const std::string& petalColor,
          const std::string& varietyType = "Common");

    /**
     * @brief Destructor for Daisy.
     * Cleans up daisy-specific resources.
     */
    ~Daisy();

    /**
     * @brief Converts daisy information to a string representation.
     * @return String containing daisy-specific details including petal color and variety.
     */
    std::string toString() const override;
};

#endif