#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
#include "Person.h"

class Plant;
class Request;
class Order;
class ConcreteOrder;
class FinalOrder;

class Customer : public Person {
protected:
    std::vector<Plant*> cart;
    double budget;
    Request* currentRequest;
    ConcreteOrder* currentOrder;
    
public:
    Customer(NurseryMediator* m, const std::string& name, 
             const std::string& id, double initialBudget);
    
    virtual ~Customer();
    
    // Cart Management
    void addToCart(Plant* plant);
    void removeFromCart(Plant* plant);
    void clearCart();
    std::vector<Plant*> getCart() const;
    int getCartSize() const;
    
    // Plant Decoration
    Plant* decoratePlantWithRibbon(Plant* plant);
    Plant* decoratePlantWithGiftWrap(Plant* plant);
    Plant* decoratePlantWithPot(Plant* plant, std::string color);

    void decorateCartItemWithRibbon(int index);
    void decorateCartItemWithGiftWrap(int index);
    void decorateCartItemWithPot(int index, std::string color);
    Plant* getPlantFromCart(int index) const;
    
    // Order Building (Composite Pattern)
    void startNewOrder(const std::string& orderName);
    void addPlantToOrder(Plant* plant);
    ConcreteOrder* getCurrentOrder() const;
    void addCartItemToOrder(int index);
    void addEntireCartToOrder();
    
    // Final Order Creation (Prototype Pattern)
    FinalOrder* createFinalOrder();
    
    // Budget Operations
    double calculateTotal() const;
    bool canAfford(double amount) const;
    double getBudget() const;
    void setBudget(double amount);
    bool deductFromBudget(double amount);
    
    // Request/Staff Interaction
    Request* createRequest(const std::string& message);
    Request* getCurrentRequest() const;
    void submitRequest();
    void receiveResponse(const std::string& response);
    
    // Plant Interaction (via Mediator)
    void requestPlantByName(const std::string& plantName);
    void browseSalesFloor();
    
    // Checkout
    virtual void checkOut() = 0;
    
    // Legacy Methods
    void giveCustomerPlant(Plant* plant);
    std::string plantNotInStock();

    bool returnPlantToDisplay(Plant* plant);
};

#endif