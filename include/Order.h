#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>

class Order { //composite pattern - Component
    public:
        virtual ~Order() = default;

        virtual double getPrice() = 0;
        virtual std::string description() = 0;
        
        virtual void add(Order* order) = 0;
        virtual void remove(Order* order) = 0;
        // virtual Order* getChild(int index) = 0; //only for a composite
};

#endif
