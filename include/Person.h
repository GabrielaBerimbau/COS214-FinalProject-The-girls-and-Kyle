#ifndef PERSON_H
#define PERSON_H

#include "Colleague.h"
#include <string>

/**
 * @file Person.h
 * @brief Base class for human actors in the Mediator pattern
 *
 * Extends Colleague to represent human participants in the nursery system.
 * Person adds identity management (name, ID) to the base Colleague 
 * communication capabilities. This class serves as the foundation for:
 *
 * Staff chain:
 * - SalesAssistant (entry-level requests)
 * - FloorManager (medium complexity)
 * - NurseryOwner (final decisions)
 *
 * Customers:
 * - Regular customers
 * - Premium members
 * - Corporate accounts
 *
 * @author Kahlan Hagerman
 * @date 2025-10-26
 */

/**
 * @class Person
 * @brief Abstract base class for people in the nursery (customers and staff)
 */
class Person: public Colleague{
    protected:
        std::string name;
        std::string id;

    public:
        /**
         * @brief Constructor
         * @param m Pointer to mediator
         * @param personName Name of the person
         * @param personId ID of the person
         */
        Person(NurseryMediator* m, std::string personName, std::string personId);

        /**
         * @brief Virtual destructor
         */
        virtual ~Person();

        /**
         * @brief Get person's name
         * @return The name
         */
        std::string getName()const;

        /**
         * @brief Get person's ID
         * @return The ID
         */
        std::string getId()const;

        /**
         * @brief Set person's name
         * @param personName The new name
         */
        void setName(std::string personName);

        /**
         * @brief Set person's ID
         * @param personId The new ID
         */
        void setId(std::string personId);
};

#endif