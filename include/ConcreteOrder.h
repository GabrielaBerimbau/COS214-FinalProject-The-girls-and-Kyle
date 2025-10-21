#ifndef CONCRETEORDER_H
#define CONCRETEORDER_H
#include "Order.h"
#include <vector>
#include <string>

class ConcreteOrder : public Order {
private:
    std::vector<std::string> plantList;
    std::string customerName;
    double totalPrice;

public:
    ConcreteOrder(std::string name, std::vector<std::string> items, double price);
    ConcreteOrder(const ConcreteOrder& other);

    Order* clone() const override;
    void display() const override;

    void setCustomerName(const std::string& name);
    void addPlant(const std::string& plant);
    double getTotalPrice() const;
};

#endif
