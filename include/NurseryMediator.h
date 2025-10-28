#ifndef NURSERYMEDIATOR_H
#define NURSERYMEDIATOR_H

#include <string>
#include <vector>

class Colleague;
class Plant;
class Customer;

/**
 * @class NurseryMediator
 * @brief Mediator that coordinates communication between different nursery components
 */
class NurseryMediator{
    protected: // changed to protected for NurseryCoordinator
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
         * @brief Give a plant to a customer
         * @param plant The plant to give to the customer
         */
        void giveCustomerPlant(Plant* plant);

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
         * @brief Return a plant from customers cart back to sales floor
         * @param plant The plant to return
         * @return true if successful, false if not
         */
        bool returnPlantToDisplay(Plant* plant);
};

#endif