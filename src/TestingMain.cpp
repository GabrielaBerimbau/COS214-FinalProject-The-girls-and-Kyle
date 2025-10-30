#include <iostream>
#include "include/NurseryMediator.h"
#include "include/SalesFloor.h"
#include "include/Customer.h"
#include "include/DerivedCustomers.h"
#include "include/Plant.h"
#include "include/RoseFactory.h"
#include "include/DaisyFactory.h"
#include "include/CactusFactory.h"
#include "include/CareScheduler.h"
#include "include/Decorator.h"
#include "include/RibbonDecorator.h"
#include "include/GiftWrapDecorator.h"
#include "include/DecorativePotDecorator.h"

void printSeparator(const std::string& title) {
    std::cout << "\n========================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "========================================\n";
}

int main() {
    std::cout << "=== REFINED DECORATOR TESTING MAIN ===\n";
    
    // ============ SETUP ============
    printSeparator("SETUP");
    
    NurseryMediator* mediator = new NurseryMediator();
    SalesFloor* salesFloor = new SalesFloor(mediator, 3, 3);
    
    mediator->registerColleague(salesFloor);
    
    CareScheduler* scheduler = new CareScheduler();
    RoseFactory roseFactory;
    DaisyFactory daisyFactory;
    CactusFactory cactusFactory;
    
    Plant* rose = roseFactory.buildPlant(scheduler);
    Plant* daisy = daisyFactory.buildPlant(scheduler);
    Plant* cactus = cactusFactory.buildPlant(scheduler);
    
    rose->setPrice(50.0);
    daisy->setPrice(30.0);
    cactus->setPrice(40.0);
    
    rose->setReadyForSale(true);
    daisy->setReadyForSale(true);
    cactus->setReadyForSale(true);
    
    salesFloor->addPlantToDisplay(rose, 0, 0);
    salesFloor->addPlantToDisplay(daisy, 0, 1);
    salesFloor->addPlantToDisplay(cactus, 1, 0);
    
    RegularCustomer* customer = new RegularCustomer();
    customer->setMediator(mediator);
    customer->setName("Decorator Tester");
    customer->setBudget(1000.0);
    mediator->registerColleague(customer);
    
    std::cout << "✓ Created sales floor with 3 plants\n";
    std::cout << "✓ Rose (R50), Daisy (R30), Cactus (R40)\n";
    
    // ============ TEST 1: Basic Decoration Stripping ============
    printSeparator("TEST 1: Decoration Stripping on Return");
    
    std::cout << "Adding Rose to cart...\n";
    customer->addPlantFromSalesFloorPosition(0, 0);
    
    std::cout << "\nOriginal Rose price: R" << customer->getPlantFromCart(0)->getPrice() << "\n";
    
    customer->decorateCartItemWithPot(0, "blue");
    customer->decorateCartItemWithRibbon(0);
    std::cout << "Decorated Rose price: R" << customer->getPlantFromCart(0)->getPrice() << "\n";
    
    std::cout << "\nReturning decorated Rose to sales floor...\n";
    customer->returnPlantToSalesFloor(0);
    
    std::cout << "\nChecking plant on sales floor after return:\n";
    Plant* returnedPlant = salesFloor->getPlantAt(0, 0);
    if (returnedPlant) {
        std::cout << "  Price: R" << returnedPlant->getPrice() << "\n";
        std::cout << "  Is Decorated: " << (Decorator::isDecorated(returnedPlant) ? "YES" : "NO") << "\n";
        std::cout << "  Description:\n" << returnedPlant->description() << "\n";
        
        if (returnedPlant->getPrice() == 50.0) {
            std::cout << "✓ SUCCESS: Decorations were stripped! Plant back to original price.\n";
        } else {
            std::cout << "✗ FAILED: Plant still has inflated price.\n";
        }
    }
    
    // ============ TEST 2: Re-add Stripped Plant ============
    printSeparator("TEST 2: Re-adding Stripped Plant to Cart");
    
    customer->addPlantFromSalesFloorPosition(0, 0);
    Plant* readdedPlant = customer->getPlantFromCart(0);
    
    std::cout << "Re-added plant:\n";
    std::cout << "  Price: R" << readdedPlant->getPrice() << "\n";
    std::cout << "  Is Decorated: " << (Decorator::isDecorated(readdedPlant) ? "YES" : "NO") << "\n";
    std::cout << "  Description:\n" << readdedPlant->description() << "\n";
    
    if (readdedPlant->getPrice() == 50.0 && !Decorator::isDecorated(readdedPlant)) {
        std::cout << "✓ SUCCESS: Plant is clean and ready for new decorations!\n";
    }
    
    // ============ TEST 3: Multiple Decoration Layers ============
    printSeparator("TEST 3: Multiple Decoration Layer Stripping");
    
    customer->addPlantFromSalesFloorPosition(0, 1);  // Daisy
    
    std::cout << "Adding multiple decorations to Daisy:\n";
    std::cout << "  Original: R" << customer->getPlantFromCart(1)->getPrice() << "\n";
    
    customer->decorateCartItemWithRibbon(1);
    std::cout << "  + Ribbon: R" << customer->getPlantFromCart(1)->getPrice() << "\n";
    
    customer->decorateCartItemWithGiftWrap(1);
    std::cout << "  + Gift Wrap: R" << customer->getPlantFromCart(1)->getPrice() << "\n";
    
    customer->decorateCartItemWithPot(1, "red");
    std::cout << "  + Red Pot: R" << customer->getPlantFromCart(1)->getPrice() << "\n";
    
    customer->decorateCartItemWithRibbon(1);
    std::cout << "  + Another Ribbon: R" << customer->getPlantFromCart(1)->getPrice() << "\n";
    
    std::cout << "\nReturning heavily decorated Daisy...\n";
    customer->returnPlantToSalesFloor(1);
    
    Plant* strippedDaisy = salesFloor->getPlantAt(0, 1);
    if (strippedDaisy) {
        std::cout << "Stripped Daisy price: R" << strippedDaisy->getPrice() << "\n";
        std::cout << "Is Decorated: " << (Decorator::isDecorated(strippedDaisy) ? "YES" : "NO") << "\n";
        
        if (strippedDaisy->getPrice() == 30.0) {
            std::cout << "✓ SUCCESS: All 4 decorator layers stripped!\n";
        }
    }
    
    // ============ TEST 4: isDecorated() Static Method ============
    printSeparator("TEST 4: Static isDecorated() Method");
    
    customer->addPlantFromSalesFloorPosition(1, 0);  // Cactus
    Plant* basePlant = customer->getPlantFromCart(0);
    
    std::cout << "Base Cactus - isDecorated(): " 
              << (Decorator::isDecorated(basePlant) ? "YES" : "NO") << "\n";
    
    customer->decorateCartItemWithRibbon(0);
    Plant* decoratedPlant = customer->getPlantFromCart(0);
    
    std::cout << "Decorated Cactus - isDecorated(): " 
              << (Decorator::isDecorated(decoratedPlant) ? "YES" : "NO") << "\n";
    
    // ============ TEST 5: Direct stripDecorations() Testing ============
    printSeparator("TEST 5: Direct stripDecorations() Method");
    
    Plant* testRose = roseFactory.buildPlant(nullptr);
    testRose->setPrice(100.0);
    
    std::cout << "Creating decorated chain manually:\n";
    std::cout << "  Base: R" << testRose->getPrice() << "\n";
    
    Plant* decorated = new RibbonDecorator(testRose);
    std::cout << "  + Ribbon: R" << decorated->getPrice() << "\n";
    
    decorated = new GiftWrapDecorator(decorated);
    std::cout << "  + Gift Wrap: R" << decorated->getPrice() << "\n";
    
    decorated = new DecorativePotDecorator(decorated, "gold");
    std::cout << "  + Gold Pot: R" << decorated->getPrice() << "\n";
    
    std::cout << "\nStripping decorations...\n";
    Plant* stripped = Decorator::stripDecorations(decorated);
    
    if (stripped) {
        std::cout << "Stripped plant price: R" << stripped->getPrice() << "\n";
        std::cout << "Is Decorated: " << (Decorator::isDecorated(stripped) ? "YES" : "NO") << "\n";
        
        if (stripped->getPrice() == 100.0) {
            std::cout << "✓ SUCCESS: Manual stripping works correctly!\n";
        }
        
        delete stripped;
    }
    
    // ============ TEST 6: Attempting to Return Undecorated Plant ============
    printSeparator("TEST 6: Return Undecorated Plant");
    
    std::cout << "Clearing cart and adding fresh plant...\n";
    customer->clearCart();
    customer->addPlantFromSalesFloorPosition(0, 1);  // Daisy
    
    Plant* freshPlant = customer->getPlantFromCart(0);
    std::cout << "Plant in cart - isDecorated(): " 
              << (Decorator::isDecorated(freshPlant) ? "YES" : "NO") << "\n";
    
    std::cout << "\nReturning undecorated plant...\n";
    customer->returnPlantToSalesFloor(0);
    
    std::cout << "✓ Undecorated plant returned successfully\n";
    
    // ============ TEST 7: Edge Cases ============
    printSeparator("TEST 7: Edge Cases");
    
    std::cout << "Testing stripDecorations(nullptr):\n";
    Plant* nullResult = Decorator::stripDecorations(nullptr);
    std::cout << "  Result: " << (nullResult == nullptr ? "nullptr (correct)" : "ERROR") << "\n";
    
    std::cout << "\nTesting isDecorated(nullptr):\n";
    bool nullDecorated = Decorator::isDecorated(nullptr);
    std::cout << "  Result: " << (nullDecorated ? "ERROR" : "false (correct)") << "\n";
    
    Plant* basePlantTest = roseFactory.buildPlant(nullptr);
    std::cout << "\nTesting stripDecorations() on base plant:\n";
    Plant* baseResult = Decorator::stripDecorations(basePlantTest);
    std::cout << "  Same pointer returned: " << (baseResult == basePlantTest ? "YES (correct)" : "NO (error)") << "\n";
    delete basePlantTest;
    
    // ============ TEST 8: Price Verification After Stripping ============
    printSeparator("TEST 8: Complete Price Cycle Verification");
    
    customer->addPlantFromSalesFloorPosition(0, 0);  // Rose at R50
    
    std::cout << "Step 1 - Original: R" << customer->getPlantFromCart(0)->getPrice() << "\n";
    
    customer->decorateCartItemWithPot(0, "blue");
    customer->decorateCartItemWithRibbon(0);
    std::cout << "Step 2 - Decorated: R" << customer->getPlantFromCart(0)->getPrice() << "\n";
    
    customer->returnPlantToSalesFloor(0);
    Plant* cyclePlant = salesFloor->getPlantAt(0, 0);
    std::cout << "Step 3 - After Return: R" << cyclePlant->getPrice() << "\n";
    
    customer->addPlantFromSalesFloorPosition(0, 0);
    std::cout << "Step 4 - Re-added to Cart: R" << customer->getPlantFromCart(0)->getPrice() << "\n";
    
    customer->decorateCartItemWithRibbon(0);
    std::cout << "Step 5 - Decorated Again: R" << customer->getPlantFromCart(0)->getPrice() << "\n";
    
    if (customer->getPlantFromCart(0)->getPrice() == 65.0) {  // 50 + 15 ribbon
        std::cout << "✓ SUCCESS: Complete price cycle works correctly!\n";
    } else {
        std::cout << "✗ Price mismatch detected\n";
    }
    
    // ============ CLEANUP ============
    printSeparator("CLEANUP");
    
    delete customer;
    delete salesFloor;
    delete scheduler;
    delete mediator;
    
    std::cout << "✓ All resources cleaned up successfully\n";
    
    printSeparator("REFINED DECORATOR TESTING COMPLETED");
    
    return 0;
}