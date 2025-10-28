#ifndef ITERATOR_H
#define ITERATOR_H

class Order;

class Iterator {
public:
    virtual ~Iterator() {}
    virtual void first() = 0;
    virtual void next() = 0;
    virtual bool isDone() const = 0;
    virtual Order* currentItem() const = 0;
};

#endif // ITERATOR_H