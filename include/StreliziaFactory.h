/**
 * @file StrelitziaFactory.h
 * @author Megan Azmanov
 * @brief Declares the StrelitziaFactory concrete class for creating strelitzia plants.
 * 
 * This class provides a concrete implementation of the FlowerFactory
 * for creating Strelitzia plant instances.
 */
#ifndef STRELITZIA_FACTORY_H
#define STRELITZIA_FACTORY_H

#include "FlowerFactory.h"

/**
 * @class StrelitziaFactory
 * @brief Concrete factory class for creating Strelitzia plants.
 * 
 * This class implements the Factory Method pattern to create
 * Strelitzia plant objects.
 */
class StrelitziaFactory : public FlowerFactory {
public:
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup.
     */
    virtual ~StrelitziaFactory() {}
    
    /**
     * @brief Creates a new Strelitzia plant instance.
     * 
     * Instantiates and returns a new Strelitzia object, 
     * configured with a care scheduler.
     * 
     * @param scheduler  pointer to a CareScheduler object for the plant. Defaults to nullptr.
     * @return A pointer to the newly created Strelitzia plant object.
     */
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const;
};

#endif // STRELITZIA_FACTORY_H
