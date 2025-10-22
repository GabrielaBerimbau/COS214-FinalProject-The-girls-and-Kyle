#ifndef DERIVEDCUSTOMERS_H
#define DERIVEDCUSTOMERS_H

#include "Customer.h"

/**
 * @class CorporateCustomer
 * @brief Corporate customer type
 */
class CorporateCustomer : public Customer {
private:
    
public:
    /**
     * @brief Default constructor
     */
    CorporateCustomer();
    
    /**
     * @brief Destructor
     */
    ~CorporateCustomer();
    
    /**
     * @brief Process checkout for corporate customer
     */
    void checkOut() override;
};

/**
 * @class RegularCustomer
 * @brief Regular customer type
 */
class RegularCustomer : public Customer {
private:
    
public:
    /**
     * @brief Default constructor
     */
    RegularCustomer();
    
    /**
     * @brief Destructor
     */
    ~RegularCustomer();
    
    /**
     * @brief Process checkout for regular customer
     */
    void checkOut() override;
};

/**
 * @class WalkInCustomer
 * @brief Walk-in customer type
 */
class WalkInCustomer : public Customer {
private:
    
public:
    /**
     * @brief Default constructor
     */
    WalkInCustomer();
    
    /**
     * @brief Destructor
     */
    ~WalkInCustomer();
    
    /**
     * @brief Process checkout for walk-in customer
     */
    void checkOut() override;
};

#endif