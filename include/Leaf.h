#ifndef LEAF_H
#define LEAF_H

#include "Order.h"
#include "Plant.h"

#include <string>
#include <iostream>

class Iterator;

class Leaf : public Order {
    public:
        Leaf(Plant* p, int q, bool ownsPlant = true);
        virtual ~Leaf();

        virtual double getPrice() override;
        virtual std::string description() override;

        virtual void add(Order* order) override;
        virtual void remove(Order* order) override;
        
        Order* clone() const override;
        std::string getName() const override;
        
        virtual Iterator* createIterator() override;

    private:
        Plant* plant;
        int quantity;
        bool ownsPlant;
        
};

#endif //LEAF_H