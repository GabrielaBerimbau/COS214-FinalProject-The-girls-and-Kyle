/**
 * @file GreenHouseStaff.h
 * @author Kyle McCalgan
 * @brief Declares the GreenHouseStaff class for greenhouse staff members.
 *
 * This class represents greenhouse staff who handle requests related to
 * plant care and greenhouse operations as part of the Chain of Responsibility pattern.
 */
#ifndef GREEN_HOUSE_STAFF_H
#define GREEN_HOUSE_STAFF_H

#include "StaffMembers.h"

/**
 * @class GreenHouseStaff
 * @brief Represents greenhouse staff in the nursery system.
 *
 * This class extends StaffMembers to handle greenhouse-specific requests
 * in the Chain of Responsibility pattern for customer service.
 */
class GreenHouseStaff : public StaffMembers{
    public:
        /**
         * @brief Constructs a new GreenHouseStaff object.
         */
        GreenHouseStaff();

        /**
         * @brief Virtual destructor.
         * Ensures proper cleanup of greenhouse staff resources.
         */
        virtual ~GreenHouseStaff();

        /**
         * @brief Handles customer requests related to greenhouse operations.
         * Part of the Chain of Responsibility pattern.
         */
        virtual void handleRequest();
};

#endif //GREEN_HOUSE_STAFF_H