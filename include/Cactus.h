/**
 * @file Cactus.h
 * @author Kyle McCalgan
 * @brief Declares the Cactus class, a specific type of succulent.
 *
 * This class represents a cactus plant with specific shape and variety attributes,
 * extending the Succulent base class.
 */
#ifndef CACTUS_H
#define CACTUS_H

#include "Succulent.h"

/**
 * @class Cactus
 * @brief Represents a cactus plant in the nursery system.
 *
 * This class extends the Succulent class to represent cacti with
 * specific shape and variety characteristics.
 */
class Cactus : public Succulent {
public:
    /**
     * @brief Constructs a new Cactus object.
     * @param id Unique identifier for the cactus.
     * @param careStrategy Pointer to the care strategy for this cactus.
     * @param initialState Pointer to the initial state of the cactus.
     * @param shape The shape/form of the cactus.
     * @param variety The variety of cactus (defaults to "Saguaro").
     */
    Cactus(const std::string& id, CareStrategy* careStrategy,
           PlantState* initialState, const std::string& shape,
           const std::string& variety = "Saguaro");

    /**
     * @brief Destructor for Cactus.
     * Cleans up cactus-specific resources.
     */
    ~Cactus();

    /**
     * @brief Converts cactus information to a string representation.
     * @return String containing cactus-specific details including shape and variety.
     */
    std::string toString() const override;
};

#endif