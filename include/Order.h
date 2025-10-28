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

    /**
     * @brief Prototype clone method for deep copying order objects.
     * @return A new dynamically allocated Order copy.
     */
    virtual Order* clone() const = 0;

    /**
     * @brief Get the name of this order component.
     * @return The name or label for this order.
     */
    virtual std::string getName() const = 0;
};

#endif

