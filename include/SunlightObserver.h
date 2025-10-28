#ifndef SUNLIGHT_OBSERVER_H
#define SUNLIGHT_OBSERVER_H

#include "PlantObserver.h"

class CareScheduler;
class Plant;

class SunlightObserver : public PlantObserver {
private:
    CareScheduler* scheduler_;
    Plant* plant_;
    
public:
    SunlightObserver(CareScheduler* scheduler, Plant* plant);
    virtual ~SunlightObserver();
    virtual void update(Plant* plant) override;
};

#endif // SUNLIGHT_OBSERVER_H
