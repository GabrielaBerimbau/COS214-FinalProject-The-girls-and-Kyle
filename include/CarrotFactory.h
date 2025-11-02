/**
 * @file CarrotFactory.h
 * @author Kyle McCalgan
 * @brief Declares the CarrotFactory concrete class for creating carrot plants.
 * 
 * This class provides a concrete implementation of the VegetableFactory
 * for creating Carrot plant instances.
 */
#ifndef CARROT_FACTORY_H
#define CARROT_FACTORY_H

#include "VegetableFactory.h"

/**
 * @class CarrotFactory
 * @brief Concrete factory class for creating Carrot plants.
 * 
 * This class implements the Factory Method pattern to create
 * Carrot plant objects with optional care scheduling capabilities.
 */
class CarrotFactory : public VegetableFactory {
public:
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup.
     */
    virtual ~CarrotFactory() {}
    
    /**
     * @brief Creates a new Carrot plant instance.
     * 
     * Instantiates and returns a new Carrot object, 
     * configured with a care scheduler.
     * 
     * @param scheduler  pointer to a CareScheduler object for the plant. Defaults to nullptr.
     * @return A pointer to the newly created Carrot plant object.
     */
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const;
};

#endif // CARROT_FACTORY_H
