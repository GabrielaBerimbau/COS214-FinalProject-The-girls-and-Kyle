#ifndef PLANT_FACTORY_H
#define PLANT_FACTORY_H

class Plant;
class CareScheduler;

/**
 * Base Creator (Factory Method).
 * Clients call buildPlant()
 */
class PlantFactory {
public:
    virtual ~PlantFactory() {}

    virtual Plant* buildPlant(CareScheduler* scheduler = nullptr) const = 0;

};

#endif // PLANT_FACTORY_H