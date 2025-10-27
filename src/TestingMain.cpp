#include <iostream>
#include <vector>

// Plant hierarchy
#include "include/Plant.h"
#include "include/Flower.h"
#include "include/Rose.h"
#include "include/Daisy.h"
#include "include/Strelitzia.h"
#include "include/Vegetable.h"
#include "include/Potato.h"
#include "include/Radish.h"
#include "include/Succulent.h"
#include "include/Aloe.h"
#include "include/Cactus.h"
#include "include/OtherPlant.h"
#include "include/VenusFlyTrap.h"

// Factories
#include "include/RoseFactory.h"
#include "include/DaisyFactory.h"
//#include "include/StrelitziaFactory.h"
#include "include/PotatoFactory.h"
#include "include/RadishFactory.h"
#include "include/AloeFactory.h"
#include "include/CactusFactory.h"
#include "include/VenusFlyTrapFactory.h"

// Care system
#include "include/CareScheduler.h"
#include "include/CareStrategy.h"
#include "include/FlowerCareStrategy.h"
#include "include/VegetableCareStrategy.h"
#include "include/SucculentCareStrategy.h"

// States
#include "include/PlantState.h"
#include "include/SeedlingState.h"
#include "include/GrowingState.h"
#include "include/MatureState.h"
#include "include/FloweringState.h"

// Mediator and colleagues
#include "include/NurseryMediator.h"
#include "include/NurseryCoordinator.h"
#include "include/Greenhouse.h"
#include "include/SalesFloor.h"

// People
#include "include/Customer.h"
#include "include/DerivedCustomers.h"
#include "include/StaffMembers.h"
#include "include/SalesAssistant.h"
#include "include/FloorManager.h"
#include "include/NurseryOwner.h"

// Requests
#include "include/Request.h"

// Orders
#include "include/Order.h"
#include "include/ConcreteOrder.h"
#include "include/FinalOrder.h"

// Payment
#include "include/PaymentProcessor.h"
#include "include/CashPayment.h"
#include "include/CreditCardPayment.h"

// Decorators
#include "include/Decorator.h"
#include "include/RibbonDecorator.h"
#include "include/GiftWrapDecorator.h"
#include "include/DecorativePotDecorator.h"

using namespace std;

void printSeparator() {
    cout << "\n========================================\n";
}

int main() {
    cout << "=== NURSERY SYSTEM TEST ===" << endl;
    
    // ============================================
    // STEP 1: Setup the nursery infrastructure
    // ============================================
    printSeparator();
    cout << "STEP 1: Setting up nursery infrastructure" << endl;
    
    NurseryCoordinator* coordinator = new NurseryCoordinator();
    Greenhouse* greenhouse = new Greenhouse(coordinator, 3, 3);
    SalesFloor* salesFloor = new SalesFloor(coordinator, 3, 3);
    
    coordinator->setGreenhouse(greenhouse);
    coordinator->setSalesFloor(salesFloor);
    coordinator->registerColleague(greenhouse);
    coordinator->registerColleague(salesFloor);
    
    cout << "✓ Nursery infrastructure created" << endl;
    
    // ============================================
    // STEP 2: Create staff members (Chain of Responsibility)
    // ============================================
    printSeparator();
    cout << "STEP 2: Hiring staff members" << endl;
    
    SalesAssistant* assistant = new SalesAssistant(coordinator, "Alice", "SA-001");
    FloorManager* manager = new FloorManager(coordinator, "Bob", "FM-001");
    NurseryOwner* owner = new NurseryOwner(coordinator, "Carol", "NO-001");
    
    // Setup chain of responsibility
    assistant->setNext(manager);
    manager->setNext(owner);
    
    coordinator->registerColleague(assistant);
    coordinator->registerColleague(manager);
    coordinator->registerColleague(owner);
    
    cout << "✓ Staff members hired and chain established" << endl;
    
   // ============================================
// STEP 3: Create plants using factories
// ============================================
printSeparator();
cout << "STEP 3: Growing plants in greenhouse" << endl;

CareScheduler* scheduler = new CareScheduler();

// Create different types of plants
RoseFactory roseFactory;
DaisyFactory daisyFactory;
PotatoFactory potatoFactory;
AloeFactory aloeFactory;
CactusFactory cactusFactory;

// Pass scheduler to factories
Plant* rose = roseFactory.buildPlant(scheduler);
Plant* daisy = daisyFactory.buildPlant(scheduler);
Plant* potato = potatoFactory.buildPlant(scheduler);
Plant* aloe = aloeFactory.buildPlant(scheduler);
Plant* cactus = cactusFactory.buildPlant(scheduler);
    
    cout << "✓ Created 5 different plants" << endl;
    
    // Add plants to greenhouse
    greenhouse->addPlant(rose, 0, 0);
    greenhouse->addPlant(daisy, 0, 1);
    greenhouse->addPlant(potato, 1, 0);
    greenhouse->addPlant(aloe, 1, 1);
    greenhouse->addPlant(cactus, 2, 0);
    
    cout << "✓ Plants added to greenhouse" << endl;
    
    // ============================================
    // STEP 4: Age plants and perform care
    // ============================================
    printSeparator();
    cout << "STEP 4: Aging and caring for plants" << endl;
    
    // Age plants through multiple days
    for (int day = 1; day <= 25; day++) {
        if (day % 5 == 0) {
            cout << "Day " << day << ": Performing daily care..." << endl;
        }
        
        rose->dailyUpdate();
        daisy->dailyUpdate();
        potato->dailyUpdate();
        aloe->dailyUpdate();
        cactus->dailyUpdate();
        
        // Perform care to keep plants healthy
        if (day % 3 == 0) {
            rose->performCare();
            daisy->performCare();
            potato->performCare();
            aloe->performCare();
            cactus->performCare();
        }
    }
    
    cout << "✓ Plants aged 25 days" << endl;
    cout << "Rose state: " << rose->getState()->getStateName() << endl;
    cout << "Rose ready for sale: " << (rose->isReadyForSale() ? "Yes" : "No") << endl;
    
    // ============================================
    // STEP 5: Move mature plants to sales floor
    // ============================================
    printSeparator();
    cout << "STEP 5: Moving mature plants to sales floor" << endl;
    
    if (rose->isReadyForSale()) {
        greenhouse->removePlant(rose);
        salesFloor->addPlantToDisplay(rose, 0, 0);
        cout << "✓ Rose moved to sales floor" << endl;
    }
    
    if (daisy->isReadyForSale()) {
        greenhouse->removePlant(daisy);
        salesFloor->addPlantToDisplay(daisy, 0, 1);
        cout << "✓ Daisy moved to sales floor" << endl;
    }
    
    // ============================================
    // STEP 6: Create customers
    // ============================================
    printSeparator();
    cout << "STEP 6: Customers entering nursery" << endl;
    
    RegularCustomer* customer1 = new RegularCustomer();
    customer1->setMediator(coordinator);
    customer1->setName("John Doe");
    customer1->setId("CUST-001");
    customer1->setBudget(500.0);
    
    WalkInCustomer* customer2 = new WalkInCustomer();
    customer2->setMediator(coordinator);
    customer2->setName("Jane Smith");
    customer2->setId("CUST-002");
    customer2->setBudget(200.0);
    
    coordinator->registerColleague(customer1);
    coordinator->registerColleague(customer2);
    
    salesFloor->addCustomer(customer1);
    salesFloor->addCustomer(customer2);
    
    cout << "✓ 2 customers entered the sales floor" << endl;
    
    // ============================================
    // STEP 7: Customer requests (Chain of Responsibility)
    // ============================================
    printSeparator();
    cout << "STEP 7: Processing customer requests" << endl;
    
    // Low-level request (handled by Sales Assistant)
    Request* simpleRequest = customer1->createRequest("I would like to buy a Rose");
    cout << "\nCustomer 1 request level: " << (int)simpleRequest->getLevel() << endl;
    assistant->handleRequest(simpleRequest);
    
    // Medium-level request (handled by Floor Manager)
    Request* bulkRequest = customer2->createRequest("I need 50 plants for a wedding");
    cout << "\nCustomer 2 request level: " << (int)bulkRequest->getLevel() << endl;
    assistant->handleRequest(bulkRequest);
    
    cout << "✓ Requests processed through chain of responsibility" << endl;
    
    // ============================================
    // STEP 8: Customer shopping
    // ============================================
    printSeparator();
    cout << "STEP 8: Customers adding plants to cart" << endl;
    
    // Customer 1 adds rose to cart
    if (rose != nullptr && rose->isReadyForSale()) {
        customer1->addToCart(rose);
        cout << "✓ Customer 1 added Rose to cart" << endl;
    }
    
    // Customer 2 adds daisy to cart
    if (daisy != nullptr && daisy->isReadyForSale()) {
        customer2->addToCart(daisy);
        cout << "✓ Customer 2 added Daisy to cart" << endl;
    }
    
    // ============================================
    // STEP 9: Decorate plants (Decorator pattern)
    // ============================================
    printSeparator();
    cout << "STEP 9: Decorating plants" << endl;
    
    cout << "\nOriginal Rose:" << endl;
    cout << "Price: R" << rose->getPrice() << endl;
    
    // Wrap rose with decorators (note: in real system, you'd need to manage this carefully)
    cout << "\nApplying decorations (conceptually):" << endl;
    cout << "- Adding ribbon (+R15)" << endl;
    cout << "- Adding gift wrap (+R20)" << endl;
    cout << "- Adding red pot (+R80)" << endl;
    
    double decoratedPrice = rose->getPrice() + 15.0 + 20.0 + 80.0;
    cout << "Total decorated price: R" << decoratedPrice << endl;
    
    // ============================================
    // STEP 10: Checkout (Template Method + Prototype)
    // ============================================
    printSeparator();
    cout << "STEP 10: Customers checking out" << endl;
    
    cout << "\n--- Customer 1 Checkout ---" << endl;
    cout << "Cart size: " << customer1->getCartSize() << endl;
    cout << "Total: R" << customer1->calculateTotal() << endl;
    customer1->checkOut();
    
    cout << "\n--- Customer 2 Checkout ---" << endl;
    cout << "Cart size: " << customer2->getCartSize() << endl;
    cout << "Total: R" << customer2->calculateTotal() << endl;
    customer2->checkOut();
    
    // ============================================
    // STEP 11: Cleanup
    // ============================================
    printSeparator();
    cout << "STEP 11: Closing nursery for the day" << endl;
    
    // Delete customers
    delete customer1;
    delete customer2;
    
    // Delete staff
    delete assistant;
    delete manager;
    delete owner;
    
    // Delete plants
    delete rose;
    delete daisy;
    delete potato;
    delete aloe;
    delete cactus;
    
    // Delete facilities
    delete salesFloor;
    delete greenhouse;
    delete coordinator;
    
    // Delete scheduler
    delete scheduler;
    
    cout << "✓ All resources cleaned up" << endl;
    
    printSeparator();
    cout << "\n=== TEST COMPLETED SUCCESSFULLY ===" << endl;
    cout << "No segfaults or crashes detected!" << endl;
    
    return 0;
}