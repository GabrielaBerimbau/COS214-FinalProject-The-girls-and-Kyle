#ifndef SALESFLOOR_H
#define SALESFLOOR_H

#include "Colleague.h"
#include <vector>

class Plant;
class Customer;

/**
 * @class SalesFloor
 * @brief Represents the sales area where customers browse and purchase plants
 */
class SalesFloor: public Colleague{
    private:
        std::vector<std::vector<Plant*>> displayGrid;
        std::vector<Customer*> currentCustomers;
        int rows;
        int cols;
        int currentNumberOfPlants;
        int capacity;

    public:
        /**
         * @brief Constructor
         * @param m Pointer to mediator
         * @param numRows Number of rows in sales floor
         * @param numCols Number of columns in sales floor
         */
        SalesFloor(NurseryMediator* m, int numRows, int numCols);

        /**
         * @brief Destructor
         */
        ~SalesFloor();

        /**
         * @brief Add a plant to the sales floor display
         * @param plant The plant to display
         * @param row Row position
         * @param col Column position
         * @return true if successful, false if not
         */
        bool addPlantToDisplay(Plant* plant, int row, int col);

        /**
         * @brief Remove a plant from display
         * @param plant The plant to remove
         */
        void removePlantFromDisplay(Plant* plant);

        /**
         * @brief Remove plant from a position in grid
         * @param row Row position
         * @param col Column position
         * @return Pointer to the removed plant, nullptr if empty
         */
        Plant* removePlantAt(int row, int col);

        /**
         * @brief Get plant at specific position in grid
         * @param row Row position
         * @param col Column position
         * @return Pointer to plant, nullptr if empty
         */
        Plant* getPlantAt(int row, int col) const;

        /**
         * @brief Add a customer to the sales floor
         * @param customer The customer entering
         */
        void addCustomer(Customer* customer);

        /**
         * @brief Remove a customer from the sales floor
         * @param customer The customer leaving
         */
        void removeCustomer(Customer* customer);

        /**
         * @brief Get all plants on display
         * Flattened from 2D to 1D
         * @return Vector of plants
         */
        std::vector<Plant*> getDisplayPlants()const;

        /**
         * @brief Get current customers
         * @return Vector of customers
         */
        std::vector<Customer*> getCurrentCustomers()const;

        /**
         * @brief Get all plants in a specific row
         * @param row Row number
         * @return Vector of plants in that row
         */
        std::vector<Plant*> getPlantsInRow(int row)const;

        /**
         * @brief Get all plants in a specific column
         * @param col Column number
         * @return Vector of plants in that column
         */
        std::vector<Plant*> getPlantsInColumn(int col)const;

        /**
         * @brief Get number of plants on display
         * @return Number of plants
         */
        int getNumberOfPlants()const;

        /**
         * @brief Get capacity
         * @return Maximum capacity
         */
        int getCapacity()const;

        /**
         * @brief Check if sales floor is at capacity
         * @return true if full, false if not
         */
        bool isFull()const;

        /**
         * @brief Get number of rows
         * @return Number of rows
         */
        int getRows()const;

        /**
         * @brief Get number of columns
         * @return Number of columns
         */
        int getColumns()const;

        /**
         * @brief Check if a position is empty
         * @param row Row position
         * @param col Column position
         * @return true if empty
         */
        bool isPositionEmpty(int row, int col)const;
};

#endif