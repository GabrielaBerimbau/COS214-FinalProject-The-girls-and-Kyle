#ifndef NURSERYCOORDINATOR_H
#define NURSERYCOORDINATOR_H

#include "NurseryMediator.h"
#include <string>

class SalesFloor;
class Greenhouse;
class Person;

/**
 * @file NurseryCoordinator.h
 * @brief Extended Mediator with high-level workflows (Mediator pattern)
 *
 * The NurseryCoordinator extends the base NurseryMediator to provide
 * higher-level coordination of complex workflows:
 * - Plant lifecycle management (greenhouse to sales floor)
 * - Customer service coordination (staff assignment)
 * - Purchase workflow orchestration
 * - Inventory management between areas
 *
 * Design approach:
 * - Maintains direct references to key areas (sales floor, greenhouse)
 * - Implements complex multi-step operations
 * - Coordinates staff-customer interactions
 * - Handles complete purchase workflows
 *
 * @author Kahlan Hagerman
 * @date 2025-10-26
 */

/**
 * @class NurseryCoordinator
 * @brief Extended mediator with additional coordination capabilities
 * @note Coordinates plant relocation and nursery operations
 */
class NurseryCoordinator: public NurseryMediator{
    private:
        SalesFloor* salesFloorRef;
        Greenhouse* greenhouseRef;

    public:
        /**
         * @brief Constructor
         */
        NurseryCoordinator();

        /**
         * @brief Destructor
         */
        ~NurseryCoordinator();

        /**
         * @brief Set reference to sales floor
         * @param sf Pointer to sales floor
         */
        void setSalesFloor(SalesFloor* sf);

        /**
         * @brief Set reference to greenhouse
         * @param gh Pointer to greenhouse
         */
        void setGreenhouse(Greenhouse* gh);

        /**
         * @brief Check if mature plants need to be moved to sales floor
         */
        void checkPlantRelocation();

        /**
         * @brief Coordinates the transfer of plant from greenhouse to sales floor
         * @param plantName Name of plant to transfer
         * @return true if successful, false if not
         */
        bool coordinatePlantTransfer(std::string plantName);

        /**
         * @brief Assign a staff member to handle a customer
         * @param customerId Customer needing assistance
         * @return Assigned staff member ID
         */
        std::string assignStaffToCustomer(std::string customerId);

        /**
         * @brief Get available staff member
         * @return Pointer to the available staff, or nullptr
         */
        Person* getAvailableStaff();

        /**
         * @brief Coordinate the entire purchase workflow
         * @param customerId Customer making purchase
         * @param plantName Plant being purchased
         * @return true if purchase completed successfully, false if not
         */
        bool coordinatePurchaseWorkflow(std::string customerId, std::string plantName);

        bool transferPlantToCustomer(std::string plantName, Customer* customer);
        bool returnPlantToDisplay(Plant* plant);
};

#endif