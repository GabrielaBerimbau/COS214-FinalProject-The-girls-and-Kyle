#include <iostream>
#include "include/NurseryMediator.h"
#include "include/NurseryCoordinator.h"
#include "include/SalesFloor.h"
#include "include/Greenhouse.h"
#include "include/Customer.h"
#include "include/DerivedCustomers.h"
#include "include/StaffMembers.h"
#include "include/SalesAssistant.h"
#include "include/FloorManager.h"
#include "include/NurseryOwner.h"
#include "include/Plant.h"
#include "include/RoseFactory.h"
#include "include/DaisyFactory.h"
#include "include/CactusFactory.h"
#include "include/CareScheduler.h"
#include "include/FinalOrder.h"

void printSeparator(const std::string& title = "") {
    std::cout << "\n";
    std::cout << "================================================\n";
    if (!title.empty()) {
        std::cout << "  " << title << "\n";
        std::cout << "================================================\n";
    }
}

int main() {
    std::cout << "=== SIMPLIFIED CUSTOMER CLASS DEMO ===\n";
    std::cout << "Testing new customer interface with proper mediator usage\n";
    
    // ============ SETUP ============
    printSeparator("SETUP: Creating Nursery Infrastructure");
    
    NurseryMediator* mediator = new NurseryMediator();
    SalesFloor* salesFloor = new SalesFloor(mediator, 3, 3);
    Greenhouse* greenhouse = new Greenhouse(mediator, 2, 2);
    
    mediator->registerColleague(salesFloor);
    mediator->registerColleague(greenhouse);
    
    // Create staff chain
    SalesAssistant* assistant = new SalesAssistant(mediator, "Alice", "SA-001");
    FloorManager* manager = new FloorManager(mediator, "Bob", "FM-001");
    NurseryOwner* owner = new NurseryOwner(mediator, "Carol", "NO-001");
    
    assistant->setNext(manager);
    manager->setNext(owner);
    
    mediator->registerColleague(assistant);
    mediator->registerColleague(manager);
    mediator->registerColleague(owner);
    
    // Create plants
    CareScheduler* scheduler = new CareScheduler();
    RoseFactory roseFactory;
    DaisyFactory daisyFactory;
    CactusFactory cactusFactory;
    
    Plant* rose1 = roseFactory.buildPlant(scheduler);
    Plant* rose2 = roseFactory.buildPlant(scheduler);
    Plant* daisy1 = daisyFactory.buildPlant(scheduler);
    Plant* cactus1 = cactusFactory.buildPlant(scheduler);
    
    rose1->setPrice(50.0);
    rose2->setPrice(50.0);
    daisy1->setPrice(30.0);
    cactus1->setPrice(40.0);
    
    // Mark plants as ready for sale
    rose1->setReadyForSale(true);
    rose2->setReadyForSale(true);
    daisy1->setReadyForSale(true);
    cactus1->setReadyForSale(true);
    
    // Place on sales floor
    salesFloor->addPlantToDisplay(rose1, 0, 0);
    salesFloor->addPlantToDisplay(daisy1, 0, 1);
    salesFloor->addPlantToDisplay(cactus1, 1, 0);
    salesFloor->addPlantToDisplay(rose2, 1, 1);
    
    std::cout << "✓ Created 3x3 sales floor with 4 plants\n";
    std::cout << "✓ Created staff chain: Assistant → Manager → Owner\n";
    
    // Create customer
    RegularCustomer* customer = new RegularCustomer();
    customer->setMediator(mediator);
    customer->setName("John Doe");
    customer->setId("CUST-001");
    customer->setBudget(500.0);
    mediator->registerColleague(customer);
    
    // ============ TEST 1: Add plant by name ============
    printSeparator("TEST 1: Add Plant from Sales Floor by Name");
    
    std::cout << "Customer requesting 'Rose' by name...\n";
    bool success = customer->addPlantFromSalesFloor("Rose");
    
    std::cout << "\nResult: " << (success ? "SUCCESS" : "FAILED") << "\n";
    std::cout << "Cart size: " << customer->getCartSize() << "\n";
    
    // ============ TEST 2: Add plant by position ============
    printSeparator("TEST 2: Add Plant from Sales Floor by Position");
    
    std::cout << "Customer requesting plant at position (0,1)...\n";
    success = customer->addPlantFromSalesFloorPosition(0, 1);
    
    std::cout << "\nResult: " << (success ? "SUCCESS" : "FAILED") << "\n";
    std::cout << "Cart size: " << customer->getCartSize() << "\n";
    
    // ============ TEST 3: Decorate plants in cart ============
    printSeparator("TEST 3: Decorate Plants in Cart");
    
    std::cout << "Original price of item 0: R" << customer->getPlantFromCart(0)->getPrice() << "\n";
    
    customer->decorateCartItemWithRibbon(0);
    std::cout << "After ribbon: R" << customer->getPlantFromCart(0)->getPrice() << "\n";
    
    customer->decorateCartItemWithGiftWrap(0);
    std::cout << "After gift wrap: R" << customer->getPlantFromCart(0)->getPrice() << "\n";
    
    customer->decorateCartItemWithPot(1, "blue");
    std::cout << "Item 1 with blue pot: R" << customer->getPlantFromCart(1)->getPrice() << "\n";
    
    // ============ TEST 4: Return plant to sales floor ============
    printSeparator("TEST 4: Return Plant to Sales Floor");
    
    std::cout << "Current cart size: " << customer->getCartSize() << "\n";
    std::cout << "Returning plant at cart index 1...\n";
    
    success = customer->returnPlantToSalesFloor(1);
    
    std::cout << "\nResult: " << (success ? "SUCCESS" : "FAILED") << "\n";
    std::cout << "New cart size: " << customer->getCartSize() << "\n";
    std::cout << "Plants on sales floor: " << salesFloor->getNumberOfPlants() << "\n";
    
    // ============ TEST 5: Staff request handling ============
    printSeparator("TEST 5: Staff Request Handling");
    
    std::cout << "Customer creating request for 'Cactus'...\n";
    Request* request = customer->createRequest("I want a Cactus plant please");
    
    std::cout << "\nSubmitting request to staff chain...\n";
    customer->submitRequestToStaff(assistant);
    
    std::cout << "\nCart size after staff handled request: " << customer->getCartSize() << "\n";
    
    // ============ TEST 6: Build order from cart ============
    printSeparator("TEST 6: Build and Finalize Order");
    
    std::cout << "Current cart contents:\n";
    for (int i = 0; i < customer->getCartSize(); i++) {
        Plant* p = customer->getPlantFromCart(i);
        std::cout << "  [" << i << "] " << p->getName() << " - R" << p->getPrice() << "\n";
    }
    
    std::cout << "\nStarting new order...\n";
    customer->startNewOrder("John's Order");
    
    std::cout << "Adding entire cart to order...\n";
    customer->addEntireCartToOrder();
    
    std::cout << "\nCreating final order...\n";
    FinalOrder* finalOrder = customer->createFinalOrder();
    
    if (finalOrder) {
        std::cout << "\n" << finalOrder->getSummary() << "\n";
        
        double total = finalOrder->calculateTotalPrice();
        std::cout << "Can customer afford R" << total << "? " 
                  << (customer->canAfford(total) ? "YES" : "NO") << "\n";
        
        delete finalOrder;
    }
    
    // ============ TEST 7: Clear cart ============
    printSeparator("TEST 7: Clear Cart");
    
    std::cout << "Cart size before clear: " << customer->getCartSize() << "\n";
    customer->clearCart();
    std::cout << "Cart size after clear: " << customer->getCartSize() << "\n";
    
    // ============ TEST 8: High-level request (escalation) ============
    printSeparator("TEST 8: Request Escalation Through Chain");
    
    std::cout << "Customer creating HIGH priority complaint...\n";
    Request* complaint = customer->createRequest("I want a refund immediately!");
    
    std::cout << "\nSubmitting complaint to staff chain...\n";
    customer->submitRequestToStaff(assistant);
    
    std::cout << "\nRequest handled: " << (complaint->isHandled() ? "YES" : "NO") << "\n";
    
    // ============ CLEANUP ============
    printSeparator("CLEANUP");
    
    delete assistant;
    delete manager;
    delete owner;
    delete customer;
    delete salesFloor;
    delete greenhouse;
    delete scheduler;
    delete mediator;
    
    std::cout << "✓ All resources cleaned up\n";
    
    printSeparator("DEMO COMPLETED SUCCESSFULLY");
    
    return 0;
}