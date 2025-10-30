/**
 * @file OtherPlantFactory.h
 * @author Megan Azmanov
 * @brief Declares the OtherPlantFactory abstract class for creating miscellaneous plant types.
 * 
 * This class extends PlantFactory to specialize in plants that don't fit into
 * the flower or vegetable categories.
 */
#ifndef OTHER_PLANT_FACTORY_H
#define OTHER_PLANT_FACTORY_H

#include "PlantFactory.h"

/**
 * @class OtherPlantFactory
 * @brief Abstract factory class for creating non-flower, non-vegetable plants.
 * 
 * This class serves as an intermediate abstraction in the factory hierarchy,
 * grouping factories for plants such as carnivorous plants.
 * Concrete factories will inherit from this class and provide specific implementations.
 */
class OtherPlantFactory : public PlantFactory {
public:
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup of derived classes.
     */
    virtual ~OtherPlantFactory() {}
    
    /**
     * @brief Creates a new miscellaneous plant instance.
     * 
     * This method remains pure virtual - concrete factories
     * must override this to create specific plant types.
     * 
     * @param scheduler pointer to a CareScheduler object for the plant. Defaults to nullptr.
     * @return A pointer to the newly created Plant object.
     */
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const = 0;
};

#endif // OTHER_PLANT_FACTORY_H
