#ifndef ABSTRACTFINALORDER_H
#define ABSTRACTFINALORDER_H

#include <vector>
#include <string>
#include "Order.h"

class AbstractFinalOrder {
public:
    virtual ~AbstractFinalOrder() = default;
    virtual AbstractFinalOrder* clone() const = 0;
    virtual double calculateTotalPrice() const = 0;
    virtual void addOrder(Order* order) = 0;
    virtual std::string getSummary() const = 0;
};

#endif
