#ifndef PLANT_OBSERVER_SUBJECT_H
#define PLANT_OBSERVER_SUBJECT_H

#include <vector>

class PlantObserver;

/**
 * Subject base (to be inherited by Plant).
 * Holds/owns the list of observers and notifies them.
 */
class PlantObserverSubject {
protected:
    std::vector<PlantObserver*> observerList;

public:
    virtual ~PlantObserverSubject() {}

    // Attach/detach do NOT take ownership of observers unless you implement it so in .cpp.
    void attach(PlantObserver* obs);
    void detach(PlantObserver* obs);
    void notify();
};

#endif