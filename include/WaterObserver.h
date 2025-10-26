#ifndef WATER_OBSERVER_H
#define WATER_OBSERVER_H

#include "PlantObserver.h"

class CareScheduler;
class Plant;

class WaterObserver : public PlantObserver {
private:
    CareScheduler* scheduler_;
    Plant* plant_;
    
public:
    WaterObserver(CareScheduler* scheduler, Plant* plant);
    virtual ~WaterObserver();
    virtual void update(Plant* plant) override;
};

#endif // WATER_OBSERVER_H

