#include <iostream>

// Plant hierarchy
#include "include/Plant.h"
#include "include/Rose.h"
#include "include/Daisy.h"
#include "include/Cactus.h"

// Factories
#include "include/RoseFactory.h"
#include "include/DaisyFactory.h"
#include "include/CactusFactory.h"

// Care system
#include "include/CareScheduler.h"
#include "include/SeedlingState.h"

// Orders and Iterator
#include "include/Order.h"
#include "include/ConcreteOrder.h"
#include "include/Leaf.h"
#include "include/FinalOrder.h"
#include "include/Iterator.h"

// Decorators
#include "include/RibbonDecorator.h"
#include "include/GiftWrapDecorator.h"
#include "include/DecorativePotDecorator.h"

using namespace std;

void printSeparator() {
    cout << "\n========================================\n";
}

int main() {
    cout << "=== ITERATOR PATTERN DEMO ===" << endl;
    
    printSeparator();
    cout << "STEP 1: Creating plants with factories" << endl;
    
    CareScheduler* scheduler = new CareScheduler();
    
    RoseFactory roseFactory;
    DaisyFactory daisyFactory;
    CactusFactory cactusFactory;
    
    Plant* rose1 = roseFactory.buildPlant(scheduler);
    Plant* rose2 = roseFactory.buildPlant(scheduler);
    Plant* daisy = daisyFactory.buildPlant(scheduler);
    Plant* cactus = cactusFactory.buildPlant(scheduler);
    
    rose1->setPrice(25.0);
    rose2->setPrice(25.0);
    daisy->setPrice(15.0);
    cactus->setPrice(20.0);
    
    cout << "✓ Created 4 plants:" << endl;
    cout << "  - Rose #1: R" << rose1->getPrice() << endl;
    cout << "  - Rose #2: R" << rose2->getPrice() << endl;
    cout << "  - Daisy: R" << daisy->getPrice() << endl;
    cout << "  - Cactus: R" << cactus->getPrice() << endl;
    
    printSeparator();
    cout << "STEP 2: Decorating plants" << endl;
    
    Plant* decoratedRose1 = new GiftWrapDecorator(new RibbonDecorator(rose1));
    cout << "✓ Rose #1 decorated with ribbon and gift wrap" << endl;
    cout << "  Price: R" << decoratedRose1->getPrice() << endl;
    
    Plant* decoratedDaisy = new DecorativePotDecorator(daisy, "red");
    cout << "✓ Daisy decorated with red pot" << endl;
    cout << "  Price: R" << decoratedDaisy->getPrice() << endl;
    
    cout << "✓ Rose #2 and Cactus remain undecorated" << endl;
    
    printSeparator();
    cout << "STEP 3: Building customer order" << endl;
    
    ConcreteOrder* mainOrder = new ConcreteOrder("Customer Shopping Cart");
    
    // Leaf now owns and will delete the plants
    Leaf* item1 = new Leaf(decoratedRose1, true);
    mainOrder->add(item1);
    cout << "✓ Added decorated Rose #1 to cart" << endl;
    
    Leaf* item2 = new Leaf(rose2, true);
    mainOrder->add(item2);
    cout << "✓ Added plain Rose #2 to cart" << endl;
    
    ConcreteOrder* giftBasket = new ConcreteOrder("Gift Basket");
    Leaf* item3 = new Leaf(decoratedDaisy,  true);
    Leaf* item4 = new Leaf(cactus,  true);
    giftBasket->add(item3);
    giftBasket->add(item4);
    cout << "✓ Created nested Gift Basket with decorated Daisy and Cactus" << endl;
    
    mainOrder->add(giftBasket);
    cout << "✓ Added Gift Basket to main cart" << endl;
    
    printSeparator();
    cout << "STEP 4: Creating Final Order" << endl;
    
    FinalOrder* finalOrder = new FinalOrder("John Doe");
    finalOrder->addOrder(mainOrder);
    
    cout << "✓ Final order created for John Doe" << endl;
    
    printSeparator();
    cout << "STEP 5: Calculating total price using Iterator" << endl;
    
    double total = finalOrder->calculateTotalPrice();
    cout << "\n💰 Total Price: R" << total << endl;
    
    printSeparator();
    cout << "STEP 6: Generating order summary using Iterator" << endl;
    
    cout << "\n" << finalOrder->getSummary() << endl;
    
    printSeparator();
    cout << "STEP 7: Printing invoice using Iterator" << endl;
    
    finalOrder->printInvoice();
    
    printSeparator();
    cout << "STEP 8: Cloning Final Order using Iterator" << endl;
    
    FinalOrder* clonedOrder = finalOrder->clone();
    cout << "\n✓ Order cloned successfully!" << endl;
    cout << "\nCloned order invoice:" << endl;
    clonedOrder->printInvoice();
    
    printSeparator();
    cout << "STEP 9: Direct Iterator traversal demonstration" << endl;
    
    cout << "\nIterating through all items in main order:" << endl;
    Iterator* it = mainOrder->createIterator();
    int itemCount = 0;
    it->first();
    while (!it->isDone()) {
        Order* item = it->currentItem();
        if (item) {
            itemCount++;
            cout << "  Item " << itemCount << ": " << item->getName() 
                 << " - R" << item->getPrice() << endl;
        }
        it->next();
    }
    delete it;
    
    cout << "\n✓ Iterator found " << itemCount << " leaf items in composite structure" << endl;
    
    printSeparator();
    cout << "STEP 10: Cleanup" << endl;
    
    // FinalOrder deletion will cascade delete everything including plants via Leaf
    delete finalOrder;
    delete clonedOrder;
    delete scheduler;
    
    cout << "✓ All resources cleaned up" << endl;
    
    printSeparator();
    cout << "\n=== DEMO COMPLETED SUCCESSFULLY ===" << endl;
    
    return 0;
}