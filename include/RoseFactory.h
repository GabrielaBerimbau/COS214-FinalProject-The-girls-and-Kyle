/**
 * @file RoseFactory.h
 * @author Megan Azmanov
 * @brief Declares the RoseFactory concrete class for creating rose plants.
 * 
 * This class provides a concrete implementation of the FlowerFactory
 * for creating Rose plant instances.
 */
#ifndef ROSE_FACTORY_H
#define ROSE_FACTORY_H

#include "FlowerFactory.h"

/**
 * @class RoseFactory
 * @brief Concrete factory class for creating Rose plants.
 * 
 * This class implements the Factory Method pattern to create
 * Rose plant objects with optional care scheduling capabilities.
 */
class RoseFactory : public FlowerFactory {
public:
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup.
     */
    virtual ~RoseFactory() {}
    
    /**
     * @brief Creates a new Rose plant instance.
     * 
     * Instantiates and returns a new Rose object, 
     * configured with a care scheduler.
     * 
     * @param scheduler pointer to a CareScheduler object for the plant. Defaults to nullptr.
     * @return A pointer to the newly created Rose plant object.
     */
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const;
};

#endif // ROSE_FACTORY_H
