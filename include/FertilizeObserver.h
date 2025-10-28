#ifndef FERTILIZE_OBSERVER_H
#define FERTILIZE_OBSERVER_H

#include "PlantObserver.h"

class CareScheduler;
class Plant;

class FertilizeObserver : public PlantObserver {
private:
    CareScheduler* scheduler_;
    Plant* plant_;
    
public:
    FertilizeObserver(CareScheduler* scheduler, Plant* plant);
    virtual ~FertilizeObserver();
    virtual void update(Plant* plant) override;
};

#endif // FERTILIZE_OBSERVER_H