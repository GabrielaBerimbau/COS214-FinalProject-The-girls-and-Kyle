/**
 * @file Aloe.h
 * @author Kyle McCalgan
 * @brief Declares the Aloe class, a specific type of succulent.
 *
 * This class represents an aloe plant with specific variety attributes,
 * extending the Succulent base class.
 */
#ifndef ALOE_H
#define ALOE_H

#include "Succulent.h"

/**
 * @class Aloe
 * @brief Represents an aloe plant in the nursery system.
 *
 * This class extends the Succulent class to represent aloe plants with
 * specific variety types, commonly used for medicinal purposes.
 */
class Aloe : public Succulent {
public:
    /**
     * @brief Constructs a new Aloe object.
     * @param id Unique identifier for the aloe.
     * @param careStrategy Pointer to the care strategy for this aloe.
     * @param initialState Pointer to the initial state of the aloe.
     * @param variety The variety of aloe (defaults to "Vera").
     */
    Aloe(const std::string& id, CareStrategy* careStrategy,
         PlantState* initialState, const std::string& variety = "Vera");

    /**
     * @brief Destructor for Aloe.
     * Cleans up aloe-specific resources.
     */
    ~Aloe();

    /**
     * @brief Converts aloe information to a string representation.
     * @return String containing aloe-specific details including variety.
     */
    std::string toString() const override;
};

#endif