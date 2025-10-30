/**
 * @file WaterObserver.h
 * @author Megan Azmanov
 * @brief Declares the WaterObserver concrete class for monitoring plant water levels.
 * 
 * This class provides a concrete implementation of the PlantObserver interface
 * for responding to changes in a plant's water status and scheduling watering tasks.
 */
#ifndef WATER_OBSERVER_H
#define WATER_OBSERVER_H

#include "PlantObserver.h"

class CareScheduler;
class Plant;

/**
 * @class WaterObserver
 * @brief Concrete observer for monitoring and responding to plant water status changes.
 * 
 * This class implements the Observer Pattern to monitor a specific plant's
 * water levels. When the plant's state changes and indicates a need for water,
 * this observer can schedule appropriate watering commands through the CareScheduler.
 */
class WaterObserver : public PlantObserver {
private:
    CareScheduler* scheduler_; 
    Plant* plant_; 
    
public:
    /**
     * @brief Constructs a water observer for a specific plant and scheduler.
     * 
     * @param scheduler Pointer to the CareScheduler for queuing watering commands.
     * @param plant Pointer to the Plant object to be monitored.
     */
    WaterObserver(CareScheduler* scheduler, Plant* plant);
    
    /**
     * @brief Destructor.
     * Cleans up the observer. Note that neither the scheduler nor plant are owned.
     */
    virtual ~WaterObserver();
    
    /**
     * @brief Responds to plant state changes related to water levels.
     * 
     * Called when the observed plant's state changes. Evaluates the plant's
     * water status and schedules watering commands if necessary.
     * 
     * @param plant Pointer to the Plant object that triggered the notification.
     */
    virtual void update(Plant* plant) override;
};

#endif // WATER_OBSERVER_H
