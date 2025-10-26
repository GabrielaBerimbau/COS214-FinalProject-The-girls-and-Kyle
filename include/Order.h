#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>

class Order {
public:
    virtual ~Order() = default;
    virtual Order* clone() const = 0;
    virtual void display() const = 0;
    virtual std::string getName() const = 0;
    virtual double getPrice() const = 0;
};

#endif
