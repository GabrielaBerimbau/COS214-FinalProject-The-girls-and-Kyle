/**
 * @file RadishFactory.h
 * @author Megan Azmanov
 * @brief Declares the RadishFactory concrete class for creating radish plants.
 * 
 * This class provides a concrete implementation of the VegetableFactory
 * for creating Radish plant instances.
 */
#ifndef RADISH_FACTORY_H
#define RADISH_FACTORY_H

#include "VegetableFactory.h"

/**
 * @class RadishFactory
 * @brief Concrete factory class for creating Radish plants.
 * 
 * This class implements the Factory Method pattern to create
 * Radish plant objects with care scheduling capabilities.
 */
class RadishFactory : public VegetableFactory {
public:
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup.
     */
    virtual ~RadishFactory() {}
    
    /**
     * @brief Creates a new Radish plant instance.
     * 
     * Instantiates and returns a new Radish object, 
     * configured with a care scheduler.
     * 
     * @param scheduler  pointer to a CareScheduler object for the plant. Defaults to nullptr.
     * @return A pointer to the newly created Radish plant object.
     */
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const;
};

#endif // RADISH_FACTORY_H
