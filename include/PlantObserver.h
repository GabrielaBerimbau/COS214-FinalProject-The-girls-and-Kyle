#ifndef PLANT_OBSERVER_H
#define PLANT_OBSERVER_H

class Plant;

/**
 * Observer base.
 * update() is called by the subject when plant state changes.
 */
class PlantObserver {
public:
    virtual ~PlantObserver() {}
    virtual void update(Plant* plant) = 0;
};

#endif // PLANT_OBSERVER_H
