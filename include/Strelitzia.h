/**
 * @file Strelitzia.h
 * @author Kyle McCalgan
 * @brief Declares the Strelitzia class, a specific type of flower.
 *
 * This class represents a strelitzia plant (Bird of Paradise), known for its
 * distinctive orange and blue flowers, extending the Flower base class.
 */
#ifndef STRELITZIA_H
#define STRELITZIA_H

#include "Flower.h"

/**
 * @class Strelitzia
 * @brief Represents a strelitzia (Bird of Paradise) plant in the nursery system.
 *
 * This class extends the Flower class to represent strelitzia plants,
 * known for their exotic bird-like flowers and tropical appearance.
 */
class Strelitzia : public Flower {
public:
    /**
     * @brief Constructs a new Strelitzia object.
     * @param id Unique identifier for the strelitzia.
     * @param careStrategy Pointer to the care strategy for this strelitzia.
     * @param initialState Pointer to the initial state of the strelitzia.
     */
    Strelitzia(const std::string& id, CareStrategy* careStrategy,
               PlantState* initialState);

    /**
     * @brief Destructor for Strelitzia.
     * Cleans up strelitzia-specific resources.
     */
    ~Strelitzia();

    /**
     * @brief Converts strelitzia information to a string representation.
     * @return String containing strelitzia-specific details.
     */
    std::string toString() const override;
};

#endif