/**
 * @file OtherPlant.h
 * @author Kyle McCalgan
 * @brief Declares the OtherPlant class for miscellaneous plant types in the nursery.
 *
 * This class provides a concrete implementation of the Plant base class
 * for plant types that don't fit into the standard categories (flowers, succulents, vegetables).
 */
#ifndef OTHERPLANT_H
#define OTHERPLANT_H

#include "Plant.h"

/**
 * @class OtherPlant
 * @brief Represents miscellaneous plant types in the nursery system.
 *
 * This class extends the Plant base class to represent plants that don't
 * fit into standard categories, with their own unique care requirements.
 */
class OtherPlant : public Plant {
public:
    /**
     * @brief Constructs a new OtherPlant object.
     * @param name The name of the plant.
     * @param id Unique identifier for the plant.
     * @param careStrategy Pointer to the care strategy for this plant.
     * @param initialState Pointer to the initial state of the plant.
     */
    OtherPlant(const std::string& name, const std::string& id,
               CareStrategy* careStrategy, PlantState* initialState);

    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup of plant resources.
     */
    virtual ~OtherPlant();

    /**
     * @brief Converts plant information to a string representation.
     * @return String containing plant-specific details.
     */
    std::string toString() const override;
};

#endif