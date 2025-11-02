#ifndef NURSERYMEDIATOR_H
#define NURSERYMEDIATOR_H

#include <string>
#include <vector>

class Colleague;
class Plant;
class Customer;

/**
 * @file NurseryMediator.h
 * @brief Mediator interface for coordinating nursery components (Mediator pattern)
 *
 * The NurseryMediator centralises interactions between colleagues such as
 * the `SalesFloor`, `Greenhouse`, `Customer` and staff members. This reduces
 * direct coupling between components and encapsulates cooperation logic:
 * - locating plants on the sales floor or greenhouse
 * - transferring plants between locations and customers
 * - registering and notifying colleagues
 *
 * Responsibilities:
 * - Provide lookup and transfer operations used by staff and customers.
 * - Maintain the list of registered colleagues and route notifications.
 *
 * To extend: implement new high-level coordination methods in the header
 * and keep implementation details in the corresponding .cpp file.
 *
 * @author Kahlan Hagerman
 * @date 2025-10-26
 */
class NurseryMediator{
    protected:
        std::vector<Colleague*> colleagues;

    public:
        /**
         * @brief Constructor
         */
        NurseryMediator();

        /**
         * @brief Destructor
         */
        virtual ~NurseryMediator();

        /**
         * @brief Notify mediator of an event from a colleague
         * @param colleague The colleague sending the notification
         */
        void notify(Colleague* colleague);

        /**
         * @brief Process a plant purchase request
         */
        void processPurchase();

        /**
         * @brief Request a plant from staff by name
         * @param plantName The name of the plant
         * @return Pointer to the plant if found, nullptr if not
         */
        Plant* requestPlantFromStaff(std::string plantName);

        /**
         * @brief Check if greenhouse has a specific plant
         * @param plantName The name of the plant
         * @return true if plant is available, false if not
         */
        bool staffChecksGreenHouse(std::string plantName);

        /**
         * @brief Get a notification when plant is unavailable
         * @return String describing unavailable plant
         */
        std::string plantUnavailable();

        /**
         * @brief Register a colleague with the mediator
         * @param colleague The colleague to register
         */
        void registerColleague(Colleague* colleague);

        /**
         * @brief Remove a colleague from the mediator
         * @param colleague The colleague to remove
         */
        void removeColleague(Colleague* colleague);

        /**
         * @brief Remove a plant from its current location and transfer it to the customer
         * @param plantName Name of the plant to transfer
         * @param customer The customer receiving the plant
         * @return true if successful, false if not
         */
        bool transferPlantToCustomer(std::string plantName, Customer* customer);
        
        /**
         * @brief Transfer plant from specific salesfloor position to customer
         * @param row Row position on salesfloor
         * @param col Column position on salesfloor
         * @param customer The customer receiving the plant
         * @return true if successful, false if not
         */
        bool transferPlantFromPosition(int row, int col, Customer* customer);
        
        /**
         * @brief Staff adds plant to customer cart (for request handling)
         * @param plantName Name of the plant to add
         * @param customer The customer receiving the plant
         * @return true if successful, false if not
         */
        bool staffAddPlantToCustomerCart(std::string plantName, Customer* customer);
        
        /**
         * @brief Return a plant from customer cart back to sales floor
         * @param plant The plant to return
         * @return true if successful, false if not
         */
        bool returnPlantToDisplay(Plant* plant);
};

#endif