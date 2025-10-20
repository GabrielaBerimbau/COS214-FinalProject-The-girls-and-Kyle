#ifndef PLANT_FACTORY_H
#define PLANT_FACTORY_H

class Plant;

/**
 * Base Creator (Factory Method).
 * Clients call buildPlant()
 */
class PlantFactory {
public:
    virtual ~PlantFactory() {}

    // Factory Method
    virtual Plant* buildPlant() const = 0;
};

#endif // PLANT_FACTORY_H