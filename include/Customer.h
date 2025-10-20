#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "Person.h"

class Plant;

/**
 * @class Customer
 * @brief Abstract base class for customer types
 */
class Customer : public Person {
protected:
    
public:
    /**
     * @brief Default constructor
     */
    Customer();
    
    /**
     * @brief Virtual destructor
     */
    virtual ~Customer();
    
    /**
     * @brief Process customer checkout
     */
    virtual void checkOut() = 0;
    
    /**
     * @brief Give a plant to the customer
     * @param plant Pointer to the plant
     */
    void giveCustomerPlant(Plant* plant);
    
    /**
     * @brief Get message when plant is not in stock
     * @return String message
     */
    std::string plantNotInStock();
};

#endif