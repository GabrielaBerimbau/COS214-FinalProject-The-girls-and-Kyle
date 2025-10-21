#ifndef FERTILIZE_OBSERVER_H
#define FERTILIZE_OBSERVER_H

#include "PlantObserver.h"

class CareScheduler;
class Plant;

class FertilizeObserver : public PlantObserver {
public:
    FertilizeObserver(CareScheduler* scheduler, Plant* plant);
    virtual ~FertilizeObserver() {}

    virtual void update();

private:
    CareScheduler* scheduler_; // not owned
    Plant* plant_;             // not owned
};

#endif // FERTILIZE_OBSERVER_H