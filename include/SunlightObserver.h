/**
 * @file SunlightObserver.h
 * @author Megan Azmanov
 * @brief Declares the SunlightObserver concrete class for monitoring plant sunlight exposure.
 * 
 * This class provides a concrete implementation of the PlantObserver interface
 * for responding to changes in a plant's sunlight status and scheduling sunlight adjustment tasks.
 */
#ifndef SUNLIGHT_OBSERVER_H
#define SUNLIGHT_OBSERVER_H

#include "PlantObserver.h"

class CareScheduler;
class Plant;

/**
 * @class SunlightObserver
 * @brief Concrete observer for monitoring and responding to plant sunlight status changes.
 * 
 * This class implements the Observer Pattern to monitor a specific plant's
 * sunlight exposure levels. When the plant's state changes and indicates a need for
 * sunlight adjustment, this observer can schedule appropriate adjustment commands
 * through the CareScheduler.
 */
class SunlightObserver : public PlantObserver {
private:
    CareScheduler* scheduler_;
    Plant* plant_;
    
public:
    /**
     * @brief Constructs a sunlight observer for a specific plant and scheduler.
     * 
     * @param scheduler Pointer to the CareScheduler for queuing sunlight adjustment commands.
     * @param plant Pointer to the Plant object to be monitored.
     */
    SunlightObserver(CareScheduler* scheduler, Plant* plant);
    
    /**
     * @brief Destructor.
     * Cleans up the observer. Note that neither the scheduler nor plant are owned.
     */
    virtual ~SunlightObserver();
    
    /**
     * @brief Responds to plant state changes related to sunlight exposure.
     * 
     * Called when the observed plant's state changes. Evaluates the plant's
     * sunlight exposure status and schedules adjustment commands if necessary.
     * 
     * @param plant Pointer to the Plant object that triggered the notification.
     */
    virtual void update(Plant* plant) override;
};

#endif // SUNLIGHT_OBSERVER_H
