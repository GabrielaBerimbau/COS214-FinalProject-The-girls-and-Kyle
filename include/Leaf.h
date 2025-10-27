#ifndef LEAF_H
#define LEAF_H

#include "Order.h"
#include "Plant.h"

#include <string>
#include <iostream>

class Leaf : public Order { //composite pattern - leaf
    public:
        Leaf(Plant* p, int q);
        virtual ~Leaf();

        virtual double getPrice() override;
        virtual std::string description() override;

        virtual void add(Order* order) override;
        virtual void remove(Order* order) override;
        // virtual Order* getChild(int index) override;  //only for a composite


    private:
        Plant* plant;
        int quantity;
        
};

#endif //LEAF_H
