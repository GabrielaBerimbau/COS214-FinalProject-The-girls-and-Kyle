#ifndef GREENHOUSE_H
#define GREENHOUSE_H

#include "Colleague.h"
#include <vector>
#include <string>

class Plant;

/**
 * @class Greenhouse
 * @brief Manages the plant inventory and plant growing areas
 */
class Greenhouse: public Colleague{
    private:
        std::vector<Plant*> plants;
        int currentNumberOfPlants;
        int capacity;

    public:
        /**
         * @brief Constructor
         * @param m Pointer to mediator
         * @param maxCap Maximum plant capacity
         */
        Greenhouse(NurseryMediator* m, int maxCap);

        /**
         * @brief Destructor
         */
        ~Greenhouse();

        /**
         * @brief Add plant to greenhouse
         * @param plant The plant to add
         * @return true if successful, false if at full capacity
         */
        bool addPlant(Plant* plant);

        /**
         * @brief Remove plant from greenhouse
         * @param plant The plant to remove
         * @return true if successful, false if not found
         */
        bool removePlant(Plant* plant);

        /**
         * @brief Find a plant by name
         * @param plantName Name of the plant
         * @return Pointer to plant if found, nullptr if not
         */
        Plant* findPlant(std::string plantName);

        /**
         * @brief Check if greenhouse has a specific plant
         * @param plantName Name to search for
         * @return true if the plant exists
         */
        bool hasPlant(std::string plantName)const;

        /**
         * @brief Get all plants in greenhouse
         * @return Vector of plants
         */
        std::vector<Plant*> getAllPlants()const;

        /**
         * @brief Get current occupancy
         * @return Number of plants
         */
        int getNumberOfPlants()const;

        /**
         * @brief Get capacity
         * @return Maximum capacity
         */
        int getCapacity()const;

        /**
         * @brief Check if greenhouse is at capacity
         * @return true if full
         */
        bool isFull()const;
};

#endif