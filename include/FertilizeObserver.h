/**
 * @file FertilizeObserver.h
 * @author Megan Azmanov
 * @brief Declares the FertilizeObserver concrete class for monitoring plant nutrient levels.
 * 
 * This class provides a concrete implementation of the PlantObserver interface
 * for responding to changes in a plant's nutrient status and scheduling fertilization tasks.
 */
#ifndef FERTILIZE_OBSERVER_H
#define FERTILIZE_OBSERVER_H

#include "PlantObserver.h"

class CareScheduler;
class Plant;

/**
 * @class FertilizeObserver
 * @brief Concrete observer for monitoring and responding to plant nutrient status changes.
 * 
 * This class implements the Observer Pattern to monitor a specific plant's
 * nutrient levels. When the plant's state changes and indicates a need for fertilization,
 * this observer can schedule appropriate fertilization commands through the CareScheduler.
 */
class FertilizeObserver : public PlantObserver {
private:
    CareScheduler* scheduler_;
    Plant* plant_;
    
public:
    /**
     * @brief Constructs a fertilize observer for a specific plant and scheduler.
     * 
     * @param scheduler Pointer to the CareScheduler for queuing fertilization commands.
     * @param plant Pointer to the Plant object to be monitored.
     */
    FertilizeObserver(CareScheduler* scheduler, Plant* plant);
    
    /**
     * @brief Destructor.
     * Cleans up the observer. Note that neither the scheduler nor plant are owned.
     */
    virtual ~FertilizeObserver();
    
    /**
     * @brief Responds to plant state changes related to nutrient levels.
     * 
     * Called when the observed plant's state changes. Evaluates the plant's
     * nutrient status and schedules fertilization commands if necessary.
     * 
     * @param plant Pointer to the Plant object that triggered the notification.
     */
    virtual void update(Plant* plant) override;
};

#endif // FERTILIZE_OBSERVER_H
