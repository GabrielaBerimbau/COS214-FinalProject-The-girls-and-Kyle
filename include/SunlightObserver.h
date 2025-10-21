#ifndef SUNLIGHT_OBSERVER_H
#define SUNLIGHT_OBSERVER_H

#include "PlantObserver.h"

class CareScheduler;
class Plant;

class SunlightObserver : public PlantObserver {
public:
    SunlightObserver(CareScheduler* scheduler, Plant* plant);
    virtual ~SunlightObserver() {}

    virtual void update();

private:
    CareScheduler* scheduler_; // not owned
    Plant* plant_;             // not owned
};

#endif // SUNLIGHT_OBSERVER_H