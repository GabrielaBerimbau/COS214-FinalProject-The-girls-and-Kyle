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
        std::vector<Plant*> displayPlants;
        std::vector<Customer*> currentCustomers;

    public:
        /**
         * @brief Constructor
         * @param m Pointer to mediator
         */
        SalesFloor(NurseryMediator* m);

        /**
         * @brief Destructor
         */
        ~SalesFloor();

        /**
         * @brief Add a plant to the sales floor display
         * @param plant The plant to display
         */
        void addPlantToDisplay(Plant* plant);

        /**
         * @brief Remove a plant from display
         * @param plant The plant to remove
         */
        void removePlantFromDisplay(Plant* plant);

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
         * @return Vector of plants
         */
        std::vector<Plant*> getDisplayPlants()const;

        /**
         * @brief Get current customers
         * @return Vector of customers
         */
        std::vector<Customer*> getCurrentCustomers()const;
};

#endif