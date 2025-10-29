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
class StaffMembers;

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
    
    // ============ CART MANAGEMENT (via Mediator) ============
    bool addPlantFromSalesFloor(const std::string& plantName);
    bool addPlantFromSalesFloorPosition(int row, int col);
    bool returnPlantToSalesFloor(int cartIndex);
    void clearCart();
    std::vector<Plant*> getCart() const;
    int getCartSize() const;
    
    // ============ PLANT DECORATION (operates on cart) ============
    void decorateCartItemWithRibbon(int index);
    void decorateCartItemWithGiftWrap(int index);
    void decorateCartItemWithPot(int index, std::string color);
    Plant* getPlantFromCart(int index) const;
    
    // ============ ORDER BUILDING (Composite Pattern) ============
    void startNewOrder(const std::string& orderName);
    void addCartItemToOrder(int index);
    void addEntireCartToOrder();
    ConcreteOrder* getCurrentOrder() const;
    
    // ============ ORDER FINALIZATION (Prototype Pattern) ============
    FinalOrder* createFinalOrder();
    double calculateTotal() const;
    
    // ============ STAFF INTERACTION (Chain of Responsibility) ============
    Request* createRequest(const std::string& message);
    void submitRequestToStaff(StaffMembers* firstHandler);
    void receiveResponse(const std::string& response);
    Request* getCurrentRequest() const;
    
    // ============ BUDGET OPERATIONS ============
    bool canAfford(double amount) const;
    double getBudget() const;
    void setBudget(double amount);
    bool deductFromBudget(double amount);
    
    // ============ CHECKOUT ============
    virtual void checkOut() = 0;

private:
    // Internal cart operations (only mediator and friends can call)
    void addToCart(Plant* plant);
    void removeFromCart(Plant* plant);
    
    friend class NurseryMediator;
    friend class NurseryCoordinator;
};

#endif