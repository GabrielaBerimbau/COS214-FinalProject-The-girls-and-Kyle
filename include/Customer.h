#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
#include "Person.h"

class Plant;
class Request;

/**
 * @class Customer
 * @brief Abstract base class for customer types
 */
class Customer : public Person {
protected:
    std::vector<Plant*> cart;
    double budget;
    Request* currentRequest;
    
public:
    /**
     * @brief Constructor
     * @param m Pointer to mediator
     * @param name Customer name
     * @param id Customer ID
     * @param initialBudget Starting budget amount
     */
    Customer(NurseryMediator* m, const std::string& name, 
             const std::string& id, double initialBudget);
    
    /**
     * @brief Virtual destructor
     */
    virtual ~Customer();
    
    // ============ Cart Management ============
    
    /**
     * @brief Add plant to shopping cart
     * @param plant Pointer to plant to add
     */
    void addToCart(Plant* plant);
    
    /**
     * @brief Remove plant from shopping cart
     * @param plant Pointer to plant to remove
     */
    void removeFromCart(Plant* plant);
    
    /**
     * @brief Clear all items from cart
     */
    void clearCart();
    
    /**
     * @brief Get all plants in cart
     * @return Vector of plant pointers
     */
    std::vector<Plant*> getCart() const;
    
    /**
     * @brief Get number of items in cart
     * @return Size of cart
     */
    int getCartSize() const;
    
    // ============ Budget Operations ============
    
    /**
     * @brief Calculate total price of items in cart
     * @return Total cost
     */
    double calculateTotal() const;
    
    /**
     * @brief Check if customer can afford amount
     * @param amount Amount to check
     * @return true if budget >= amount
     */
    bool canAfford(double amount) const;
    
    /**
     * @brief Get current budget
     * @return Budget amount
     */
    double getBudget() const;
    
    /**
     * @brief Set customer budget
     * @param amount New budget amount
     */
    void setBudget(double amount);
    
    /**
     * @brief Deduct amount from budget (called by payment processor)
     * @param amount Amount to deduct
     * @return true if successful, false if insufficient funds
     */
    bool deductFromBudget(double amount);
    
    // ============ Request/Staff Interaction ============
    
    /**
     * @brief Create a new request with message
     * @param message Request text
     * @return Pointer to created request
     */
    Request* createRequest(const std::string& message);
    
    /**
     * @brief Get current active request
     * @return Pointer to current request, nullptr if none
     */
    Request* getCurrentRequest() const;
    
    /**
     * @brief Submit current request through mediator to staff
     */
    void submitRequest();
    
    /**
     * @brief Receive response from staff (callback)
     * @param response Response message from staff
     */
    void receiveResponse(const std::string& response);
    
    // ============ Plant Interaction (via Mediator) ============
    
    /**
     * @brief Request specific plant by name through staff
     * @param plantName Name of plant to request
     */
    void requestPlantByName(const std::string& plantName);
    
    /**
     * @brief Browse available plants on sales floor
     */
    void browseSalesFloor();
    
    // ============ Checkout ============
    
    /**
     * @brief Process checkout (delegates to PaymentProcessor)
     * Pure virtual - implemented by derived classes
     */
    virtual void checkOut() = 0;
    
    // ============ Legacy Methods (from original) ============
    
    /**
     * @brief Give a plant to the customer
     * @param plant Pointer to the plant
     */
    void giveCustomerPlant(Plant* plant);
    
    /**
     * @brief Get message when plant is not in stock
     * @return String message
     */
    std::string plantNotInStock();
};

#endif