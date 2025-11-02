/**
 * @file Customer.h
 * @author Kyle McCalgan
 * @brief Declares the Customer class for managing customer interactions in the nursery.
 *
 * This class represents a customer who can browse plants, build orders using the
 * Composite pattern, decorate plants with the Decorator pattern, and interact
 * with staff through the Chain of Responsibility pattern.
 */
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

/**
 * @class Customer
 * @brief Abstract base class representing a customer in the nursery system.
 *
 * This class manages customer shopping cart, order creation, plant decoration,
 * staff interactions, and budget management. It serves as the base for specific
 * customer types with different checkout behaviors.
 */
class Customer : public Person {
protected:
    std::vector<Plant*> cart;
    double budget;
    Request* currentRequest;
    ConcreteOrder* currentOrder;
    
public:
    /**
     * @brief Constructs a new Customer object.
     * @param m Pointer to the nursery mediator for communication.
     * @param name The customer's name.
     * @param id Unique identifier for the customer.
     * @param initialBudget The customer's initial budget.
     */
    Customer(NurseryMediator* m, const std::string& name,
             const std::string& id, double initialBudget);

    /**
     * @brief Virtual destructor.
     * Ensures proper cleanup of customer resources.
     */
    virtual ~Customer();

    // ============ CART MANAGEMENT (via Mediator) ============
    /**
     * @brief Adds a plant from sales floor to cart by name.
     * @param plantName The name of the plant to add.
     * @return True if plant was successfully added, false otherwise.
     */
    bool addPlantFromSalesFloor(const std::string& plantName);

    /**
     * @brief Adds a plant from sales floor to cart by position.
     * @param row The row position on the sales floor.
     * @param col The column position on the sales floor.
     * @return True if plant was successfully added, false otherwise.
     */
    bool addPlantFromSalesFloorPosition(int row, int col);

    /**
     * @brief Returns a plant from cart back to sales floor.
     * @param cartIndex Index of the item in the cart.
     * @return True if plant was successfully returned, false otherwise.
     */
    bool returnPlantToSalesFloor(int cartIndex);

    /**
     * @brief Clears all items from the cart.
     */
    void clearCart();

    /**
     * @brief Gets the current shopping cart.
     * @return Vector of Plant pointers in the cart.
     */
    std::vector<Plant*> getCart() const;

    /**
     * @brief Gets the number of items in the cart.
     * @return The cart size.
     */
    int getCartSize() const;

    // ============ PLANT DECORATION (operates on cart) ============
    /**
     * @brief Decorates a cart item with ribbon.
     * @param index Index of the cart item to decorate.
     */
    void decorateCartItemWithRibbon(int index);

    /**
     * @brief Decorates a cart item with gift wrap.
     * @param index Index of the cart item to decorate.
     */
    void decorateCartItemWithGiftWrap(int index);

    /**
     * @brief Decorates a cart item with a decorative pot.
     * @param index Index of the cart item to decorate.
     * @param color Color of the decorative pot.
     */
    void decorateCartItemWithPot(int index, std::string color);

    /**
     * @brief Gets a plant from the cart.
     * @param index Index of the plant in the cart.
     * @return Pointer to the Plant object.
     */
    Plant* getPlantFromCart(int index) const;

    // ============ ORDER BUILDING (Composite Pattern) ============
    /**
     * @brief Starts a new order with the given name.
     * @param orderName Name for the new order.
     */
    void startNewOrder(const std::string& orderName);

    /**
     * @brief Adds a cart item to the current order.
     * @param index Index of the cart item to add to order.
     */
    void addCartItemToOrder(int index);

    /**
     * @brief Adds all cart items to the current order.
     */
    void addEntireCartToOrder();

    /**
     * @brief Gets the current order being built.
     * @return Pointer to the ConcreteOrder object.
     */
    ConcreteOrder* getCurrentOrder() const;

    // ============ ORDER FINALIZATION (Prototype Pattern) ============
    /**
     * @brief Creates a final order from the current order.
     * @return Pointer to the FinalOrder object.
     */
    FinalOrder* createFinalOrder();

    /**
     * @brief Calculates the total cost of current order.
     * @return The total cost as a double.
     */
    double calculateTotal() const;

    // ============ STAFF INTERACTION (Chain of Responsibility) ============
    /**
     * @brief Creates a request with the given message.
     * @param message The request message content.
     * @return Pointer to the created Request object.
     */
    Request* createRequest(const std::string& message);

    /**
     * @brief Submits a request to staff for handling.
     * @param firstHandler Pointer to the first staff handler in the chain.
     */
    void submitRequestToStaff(StaffMembers* firstHandler);

    /**
     * @brief Receives a response from staff.
     * @param response The response message from staff.
     */
    void receiveResponse(const std::string& response);

    /**
     * @brief Gets the current request.
     * @return Pointer to the current Request object.
     */
    Request* getCurrentRequest() const;

    // ============ BUDGET OPERATIONS ============
    /**
     * @brief Checks if customer can afford the given amount.
     * @param amount The amount to check.
     * @return True if affordable, false otherwise.
     */
    bool canAfford(double amount) const;

    /**
     * @brief Gets the customer's current budget.
     * @return The budget amount.
     */
    double getBudget() const;

    /**
     * @brief Sets the customer's budget.
     * @param amount The new budget amount.
     */
    void setBudget(double amount);

    /**
     * @brief Deducts an amount from the customer's budget.
     * @param amount The amount to deduct.
     * @return True if deduction successful, false if insufficient funds.
     */
    bool deductFromBudget(double amount);

    // ============ REMOVAL OPERATIONS ============
    /**
     * @brief Removes ribbon decoration from a cart item.
     * @param index Index of the cart item.
     */
    void removeRibbonFromCartItem(int index);

    /**
     * @brief Removes pot decoration from a cart item.
     * @param index Index of the cart item.
     */
    void removePotFromCartItem(int index);

    /**
     * @brief Clears all decorations from a cart item.
     * @param index Index of the cart item.
     */
    void clearDecorationsForCartItem(int index);

    // ============ CHECKOUT ============
    /**
     * @brief Pure virtual checkout method to be implemented by derived classes.
     * Defines the specific checkout behavior for different customer types.
     */
    virtual void checkOut() = 0;

private:
    /**
     * @brief Internal method to add a plant to cart.
     * Only accessible by mediator and friends.
     * @param plant Pointer to the plant to add.
     */
    void addToCart(Plant* plant);

    /**
     * @brief Internal method to remove a plant from cart.
     * Only accessible by mediator and friends.
     * @param plant Pointer to the plant to remove.
     */
    void removeFromCart(Plant* plant);

    friend class NurseryMediator;
    friend class NurseryCoordinator;
};

#endif