#ifndef CONCRETEITERATOR_H
#define CONCRETEITERATOR_H

#include "Iterator.h"
#include <vector>

class Order;

class ConcreteIterator : public Iterator {
private:
    std::vector<Order*> items;
    int currentIndex;
    
    void collectLeaves(Order* order);
    
public:
    explicit ConcreteIterator(Order* root);
    virtual ~ConcreteIterator();
    
    void first() override;
    void next() override;
    bool isDone() const override;
    Order* currentItem() const override;
};

#endif // CONCRETEITERATOR_H