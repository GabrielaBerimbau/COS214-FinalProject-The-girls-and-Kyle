/**
 * @file PlantObserver.h
 * @author Megan Azmanov
 * @brief Declares the PlantObserver interface, serving as the base for Observer pattern implementations.
 * 
 * This abstract class defines the essential interface for all concrete observer types in the system.
 * It enforces implementation of the update method for responding to plant state changes.
 */
#ifndef PLANT_OBSERVER_H
#define PLANT_OBSERVER_H

class Plant;

/**
 * @class PlantObserver
 * @brief Abstract base class defining the interface for plant state observers.
 * 
 * This class forms the foundation of the Observer Pattern implementation.
 * Observers register with a plant, the subject, and are notified via the update()
 * method whenever the plant's state changes. This enables loose coupling between
 * the plant and its dependent objects.
 */
class PlantObserver {
public:
    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup of derived classes.
     */
    virtual ~PlantObserver() {}
    
    /**
     * @brief Notifies the observer of a state change in the observed plant.
     * 
     * This method is called by the subject (Plant) when its state changes.
     * Concrete observers must implement this method to respond appropriately
     * to the plant's state changes.
     * 
     * @param plant Pointer to the Plant object that triggered the notification.
     */
    virtual void update(Plant* plant) = 0;
};

#endif // PLANT_OBSERVER_H
