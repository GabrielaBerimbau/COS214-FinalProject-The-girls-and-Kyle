/**
 * @file PlantFactory.h
 * @author Megan Azmanov
 * @brief Declares the PlantFactory interface, serving as the base for Factory Method pattern implementations.
 * 
 * This abstract class defines the  interface for all concrete plant factory types in the system.
 * It enforces implementation of the factory method for creating various plant objects.
 */
#ifndef PLANT_FACTORY_H
#define PLANT_FACTORY_H

class Plant;
class CareScheduler;

/**
 * @class PlantFactory
 * @brief Abstract base class defining the interface for plant factories.
 * 
 * This class forms the foundation of the **Factory Method Pattern** implementation.
 * Clients call buildPlant() to create specific plant objects without knowing
 * the concrete class being instantiated.
 */
class PlantFactory {
public:
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup of derived classes.
     */
    virtual ~PlantFactory() {}

    /**
     * @brief Creates a new plant instance.
     * 
     * This factory method must be implemented by all subclasses to create
     * specific types of plants.
     * 
     * @param scheduler  pointer to a CareScheduler object for the plant. Defaults to nullptr.
     * @return A pointer to the newly created Plant object.
     */
    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const = 0;
};

#endif // PLANT_FACTORY_H
