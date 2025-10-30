#ifndef GREENHOUSE_H
#define GREENHOUSE_H

#include "Colleague.h"
#include <vector>
#include <string>

class Plant;

/**
 * @file Greenhouse.h
 * @brief Plant storage and growth Colleague in Mediator pattern
 *
 * The Greenhouse serves as the backend inventory and growing facility,
 * communicating with other components through the mediator:
 * - Responds to plant availability queries
 * - Manages plant growth and readiness
 * - Coordinates with SalesFloor for restocking
 * - Notifies when plants are ready for sale
 *
 * Implementation approach:
 * - Grid-based storage system for plants
 * - Plant lookup by name or position
 * - Capacity management
 * - Growth stage tracking
 *
 * @author Kahlan Hagerman
 * @date 2025-10-26
 */

/**
 * @class Greenhouse
 * @brief Manages the plant inventory and plant growing areas
 */
class Greenhouse: public Colleague{
    private:
        std::vector<std::vector<Plant*>> plantGrid;
        int currentNumberOfPlants;
        int capacity;
        int rows;
        int cols;

    public:
        /**
         * @brief Constructor
         * @param m Pointer to mediator
         * @param numRows Number of rows in greenhouse
         * @param numCols Number of columns in greenhouse
         */
        Greenhouse(NurseryMediator* m, int numRows, int numCols);

        /**
         * @brief Destructor
         */
        ~Greenhouse();

        /**
         * @brief Add plant to greenhouse
         * @param plant The plant to add
         * @param row Row position
         * @param col Column position
         * @return true if successful, false if at full capacity or position is occupied
         */
        bool addPlant(Plant* plant, int row, int col);

        /**
         * @brief Remove a specific plant from the grid
         * @param plant The plant to remove
         * @return true if found and removed
         */
        bool removePlant(Plant* plant);

        /**
         * @brief Remove plant from a position in the grid
         * @param row Row position
         * @param col Column position
         * @return Pointer to the removed plant, nullptr if empty
         */
        Plant* removePlantAt(int row, int col);

        /**
         * @brief Find a plant by name
         * @param plantName Name of the plant
         * @return Pointer to plant if found, nullptr if not
         */
        Plant* findPlant(std::string plantName);

        /**
         * @brief Get plant at specific position
         * @param row Row position
         * @param col Column position
         * @return Pointer to plant, nullptr if empty
         */
        Plant* getPlantAt(int row, int col)const;

        /**
         * @brief Check if greenhouse has a specific plant
         * @param plantName Name to search for
         * @return true if the plant exists
         */
        bool hasPlant(std::string plantName)const;

        /**
         * @brief Get all plants in greenhouse
         * Flattened from 2D to 1D here
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

        /**
         * @brief Get all plants in a specific row
         * @param row Row number
         * @return Vector of plants in that row
         */
        std::vector<Plant*> getPlantsInRow(int row) const;
        
        /**
         * @brief Get all plants in a specific column
         * @param col Column number
         * @return Vector of plants in that column
         */
        std::vector<Plant*> getPlantsInColumn(int col) const;
        
        /**
         * @brief Get number of rows
         * @return Number of rows
         */
        int getRows() const;
        
        /**
         * @brief Get number of columns
         * @return Number of columns
         */
        int getColumns() const;

        /**
         * @brief Checks if a position in the grid is empty
         * @param row Row position
         * @param col Column position
         * @return true if empty or out of bounds
         */
        bool isPositionEmpty(int row, int col) const;
        

        std::string toString() const;
};

#endif