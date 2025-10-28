#ifndef CONCRETEORDER_H
#define CONCRETEORDER_H

#include "Order.h"
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
class Iterator; 

class ConcreteOrder : public Order {
    public:
        ConcreteOrder(std::string orderN);
        virtual ~ConcreteOrder();

        virtual double getPrice() override;
        virtual std::string description() override;

        virtual void add(Order* order) override;
        virtual void remove(Order* order) override;
        // virtual Order* getChild(int index) override; //only for a composite

        virtual Order* clone() const override;
        virtual std::string getName() const override;

        virtual Iterator* createIterator() override;
        
        std::vector<Order*> getChildren() const;

    private:
        std::vector<Order*> plantList;
        std::string orderName;
        // std::string customerName; - in Person class
        // double totalPrice; - in final order
};

#endif //CONCRETEORDER_H

