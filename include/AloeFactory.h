/**
 * @file AloeFactory.h
 * @author Megan Azmanov
 * @brief Declares the AloeFactory concrete class for creating aloe plants.
 * 
 * This class provides a concrete implementation of the SucculentFactory
 * for creating Aloe plant instances.
 */

#ifndef ALOE_FACTORY_H
#define ALOE_FACTORY_H

#include "SucculentFactory.h"

/**
 * @class AloeFactory
 * @brief Concrete factory class for creating Aloe plants.
 * 
 * This class implements the Factory Method pattern to create
 * Aloe plant objects with  care scheduling capabilities.
 */

class AloeFactory : public SucculantFactory {
public:

/**
     * @brief Virtual destructor.
     * Ensures proper cleanup.
     */
    virtual ~AloeFactory() {}

    /**
     * @brief Creates a new Aloe plant instance.
     * 
     * Instantiates and returns a new Aloe object, 
     * configured with a care scheduler.
     * 
     * @param scheduler  pointer to a CareScheduler object for the plant. Defaults to nullptr.
     * @return A pointer to the newly created Aloe plant object.
     */
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const;
};

#endif // ALOE_FACTORY_H