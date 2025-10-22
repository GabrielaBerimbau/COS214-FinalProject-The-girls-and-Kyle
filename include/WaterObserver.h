#ifndef WATER_OBSERVER_H
#define WATER_OBSERVER_H

#include "PlantObserver.h"

class CareScheduler;  // forward
class Plant;

class WaterObserver : public PlantObserver {
public:
    // Optionally pass collaborators via ctor; bodies in .cpp.
    WaterObserver(CareScheduler* scheduler, Plant* plant);
    virtual ~WaterObserver() {}

    virtual void update();
    void addTask();  // as per UML

private:
    CareScheduler* scheduler_; // not owned
    Plant* plant_;             // not owned
};

#endif // WATER_OBSERVER_H