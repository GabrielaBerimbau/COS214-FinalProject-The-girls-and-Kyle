/**
 * @file DaisyFactory.h
 * @author Megan Azmanov
 * @brief Declares the DaisyFactory concrete class for creating daisy plants.
 * 
 * This class provides a concrete implementation of the FlowerFactory
 * for creating Daisy plant instances.
 */
#ifndef DAISY_FACTORY_H
#define DAISY_FACTORY_H

#include "FlowerFactory.h"

/**
 * @class DaisyFactory
 * @brief Concrete factory class for creating Daisy plants.
 * 
 * This class implements the Factory Method pattern to create
 * Daisy plant objects with  care scheduling capabilities.
 */
class DaisyFactory : public FlowerFactory {
public:
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup.
     */
    virtual ~DaisyFactory() {}
    
    /**
     * @brief Creates a new Daisy plant instance.
     * 
     * Instantiates and returns a new Daisy object,
     * configured with a care scheduler.
     * 
     * @param scheduler  pointer to a CareScheduler object for the plant. Defaults to nullptr.
     * @return A pointer to the newly created Daisy plant object.
     */
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const;
};

#endif // DAISY_FACTORY_H
