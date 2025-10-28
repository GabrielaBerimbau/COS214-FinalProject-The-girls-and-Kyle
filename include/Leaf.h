#ifndef LEAF_H
#define LEAF_H

#include "Order.h"
#include "Plant.h"

#include <string>
#include <iostream>

class Iterator;

/**
 * @class Leaf
 * @brief Represents a single physical plant in an order (Composite Pattern - Leaf)
 * 
 * In a real nursery, you purchase actual physical plants, not "quantities" of plants.
 * Each Leaf represents ONE unique plant object from your inventory.
 * If a customer wants multiple plants, you add multiple Leaf objects to the order.
 */
class Leaf : public Order {
    public:
        /**
         * @brief Construct a Leaf for a single physical plant
         * @param p The actual plant object from inventory
         * @param ownsPlant Whether this Leaf owns and should delete the plant
         */
        Leaf(Plant* p, bool ownsPlant = true);
        virtual ~Leaf();

        virtual double getPrice() override;
        virtual std::string description() override;

        virtual void add(Order* order) override;
        virtual void remove(Order* order) override;
        
        Order* clone() const override;
        std::string getName() const override;
        
        virtual Iterator* createIterator() override;

    private:
        Plant* plant;  ///< The single physical plant this Leaf represents
        bool ownsPlant; ///< Whether we own and should delete this plant
        
};

#endif //LEAF_H