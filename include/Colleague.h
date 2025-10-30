#ifndef COLLEAGUE_H
#define COLLEAGUE_H

#include <string>
#include "NurseryMediator.h"

/**
 * @file Colleague.h
 * @brief Abstract colleague interface for Mediator pattern
 *
 * Defines the base interface for all components that participate in
 * mediated communication within the nursery system. Each Colleague:
 * - Maintains a reference to its mediator
 * - Can only communicate through the mediator
 * - Must not have direct references to other colleagues
 *
 * Classes that inherit from Colleague include:
 * - SalesFloor (display and plant management)
 * - Greenhouse (plant storage and growth)
 * - StaffMembers (request handling chain)
 * - Customers (browsing and purchasing)
 *
 * @author Kahlan Hagerman
 * @date 2025-10-26
 */

/**
 * @class Colleague
 * @brief Abstract base class for all entities that communicate through the mediator
 */
class Colleague{
    protected:
        NurseryMediator* mediator;

    public:
        /**
         * @brief Constructor
         * @param m Pointer to the mediator
         */
        Colleague(NurseryMediator* m);

        /**
         * @brief Virtual destructor
         */
        virtual ~Colleague();

        /**
         * @brief Set or change the mediator
         * @param m Pointer to the new mediator
         */
        void setMediator(NurseryMediator* m);

        /**
         * @brief Get the current mediator
         * @return Pointer to the mediator
         */
        NurseryMediator* getMediator()const;
};

#endif