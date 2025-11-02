#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <sstream>
#include <limits>

// ==================== ALL REQUIRED HEADERS ====================
#include "include/NurseryCoordinator.h"
#include "include/NurseryMediator.h"
#include "include/Greenhouse.h"
#include "include/SalesFloor.h"
#include "include/Customer.h"
#include "include/DerivedCustomers.h"
#include "include/SalesAssistant.h"
#include "include/FloorManager.h"
#include "include/NurseryOwner.h"
#include "include/Request.h"
#include "include/CareScheduler.h"
#include "include/ConcreteOrder.h"
#include "include/Leaf.h"
#include "include/FinalOrder.h"
#include "include/PaymentProcessor.h"
#include "include/CashPayment.h"
#include "include/CreditCardPayment.h"
#include "include/Plant.h"
#include "include/RibbonDecorator.h"
#include "include/GiftWrapDecorator.h"
#include "include/DecorativePotDecorator.h"

// Factory headers
#include "include/RoseFactory.h"
#include "include/DaisyFactory.h"
#include "include/StrelitziaFactory.h"
#include "include/CactusFactory.h"
#include "include/AloeFactory.h"
#include "include/PotatoFactory.h"
#include "include/RadishFactory.h"
#include "include/CarrotFactory.h"
#include "include/MonsteraFactory.h"
#include "include/VenusFlyTrapFactory.h"

// State headers
#include "include/MatureState.h"
#include "include/GrowingState.h"
#include "include/SeedlingState.h"

using namespace std;

// ==================== ANSI COLOR CODES ====================
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

// ==================== GLOBAL SYSTEM COMPONENTS ====================
NurseryCoordinator* coordinator = nullptr;
SalesFloor* salesFloor = nullptr;
Greenhouse* greenhouse = nullptr;
CareScheduler* scheduler = nullptr;
SalesAssistant* assistant = nullptr;
FloorManager* manager = nullptr;
NurseryOwner* owner = nullptr;

// ==================== DISPLAY HELPER FUNCTIONS ====================
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause(const string& message = "\nPress Enter to continue...") {
    cout << CYAN << message << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int getMenuChoice(int min, int max) {
    int choice;
    while (true) {
        cout << YELLOW << "\nEnter choice (" << min << "-" << max << "): " << RESET;
        if (cin >> choice && choice >= min && choice <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "Invalid input! Please try again." << RESET << endl;
    }
}

string getStringInput(const string& prompt) {
    string input;
    cout << CYAN << prompt << RESET;
    getline(cin, input);
    return input;
}

double getDoubleInput(const string& prompt) {
    double value;
    while (true) {
        cout << CYAN << prompt << RESET;
        if (cin >> value && value >= 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "Invalid input! Please enter a positive number." << RESET << endl;
    }
}

void printHeader(const string& title) {
    cout << "\n" << BOLD << CYAN;
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "   " << setw(55) << left << title << "   \n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";
    cout << RESET << endl;
}

void printSeparator() {
    cout << BLUE << "────────────────────────────────────────────────────────────" << RESET << endl;
}

string formatPrice(double amount) {
    ostringstream oss;
    oss << "R" << fixed << setprecision(2) << amount;
    return oss.str();
}

// ==================== ENHANCED DISPLAY FUNCTIONS ====================

void displayHealthBar(const string& label, int level, const string& color) {
    cout << label << ": [";
    for (int i = 0; i < 10; i++) {
        if (i < level/10) cout << color << "█" << RESET;
        else cout << "░";
    }
    cout << "] " << level << "%" << endl;
}

void displayPlantDetailed(Plant* plant) {
    if (!plant) return;
    
    cout << BOLD << plant->getName() << " (" << plant->getID() << ")" << RESET << endl;
    cout << "State: " << GREEN << plant->getState()->getStateName() << RESET << endl;
    cout << "Age: " << plant->getAge() << " days" << endl;
    
    displayHealthBar("Water    ", plant->getWaterLevel(),   BLUE);
    displayHealthBar("Nutrients", plant->getNutrientLevel(), GREEN);
    displayHealthBar("Sunlight ", plant->getSunlightExposure(), YELLOW);
    displayHealthBar("Health   ", plant->getHealthLevel(), MAGENTA);
    
    cout << "Price: " << GREEN << formatPrice(plant->getPrice()) << RESET;
    if (plant->isReadyForSale()) {
        cout << GREEN << " ✔️  Ready for sale" << RESET << endl;
    } else {
        cout << YELLOW << " ⚠️  Still growing" << RESET << endl;
    }
}

void displayGreenhouseGrid() {
    cout << BOLD << "\n🏡 GREENHOUSE STATUS:\n" << RESET;
    cout << "Grid Size: " << greenhouse->getRows() << "x" << greenhouse->getColumns() << endl;
    cout << "Plants: " << greenhouse->getNumberOfPlants() << "/" 
         << greenhouse->getCapacity() << endl << endl;
    
    // Display grid
    for (int i = 0; i < greenhouse->getRows(); i++) {
        for (int j = 0; j < greenhouse->getColumns(); j++) {
            Plant* plant = greenhouse->getPlantAt(i, j);
            if (plant) {
                //use health level to change plants display colour
                int health = plant->getHealthLevel();
                string color = health > 70 ? GREEN : (health > 40 ? YELLOW : RED);
                
                string name = plant->getName();
                if (name.length() > 6) name = name.substr(0, 6);
                
                cout << "[" << color << setw(6) << left << name << RESET << "]";
            } else {
                cout << "[" << "______" << "]";
            }
        }
        cout << endl;
    }
}

void displaySalesFloorGrid() {
    cout << BOLD << "\n🪴  SALES FLOOR STATUS:\n" << RESET;
    cout << "Grid Size: " << salesFloor->getRows() << "x" << salesFloor->getColumns() << endl;
    cout << "Plants: " << salesFloor->getNumberOfPlants() << "/" 
         << salesFloor->getCapacity() << endl << endl;
    
    for (int i = 0; i < salesFloor->getRows(); i++) {
        for (int j = 0; j < salesFloor->getColumns(); j++) {
            Plant* plant = salesFloor->getPlantAt(i, j);
            if (plant) {
                string name = plant->getName();
                if (name.length() > 6) name = name.substr(0, 6);
                cout << "[" << GREEN << setw(6) << left << name << RESET << "]";
            } else {
                cout << "[" << "______" << "]";
            }
        }
        cout << endl;
    }
}

void displayCartWithNumbers(Customer* customer) {
    printHeader("🛒 SHOPPING CART");
    
    cout << "Customer: " << BOLD << customer->getName() << RESET << "\n";
    cout << "Budget: " << GREEN << formatPrice(customer->getBudget()) << RESET << "\n";
    printSeparator();
    
    int cartSize = customer->getCartSize();
    
    if (cartSize == 0) {
        cout << YELLOW << "Your cart is empty.\n" << RESET;
        printSeparator();
        return;
    }
    
    double totalPrice = 0.0;
    
    // displaying items in cart
    for (int i = 0; i < cartSize; i++) {
        Plant* plant = customer->getPlantFromCart(i);
        cout << CYAN << "[" << setw(2) << right << (i + 1) << "] " << RESET
             << setw(20) << left << plant->getName()
             << GREEN << setw(10) << right << formatPrice(plant->getPrice()) << RESET << "\n";
        
        string desc = plant->description();
        if (desc.find("Decoration") != string::npos || 
            desc.find("ribbon") != string::npos ||
            desc.find("gift") != string::npos ||
            desc.find("pot") != string::npos) {
            cout << "     " << MAGENTA << "🎨 Decorated" << RESET << "\n";
        }
        
        totalPrice += plant->getPrice();
    }
    
    printSeparator();
    cout << BOLD << "Total Items: " << cartSize << RESET << "\n";
    cout << BOLD << "Total Price: " << GREEN << formatPrice(totalPrice) << RESET << "\n";
    cout << "Budget Remaining: " << YELLOW << formatPrice(customer->getBudget()) << RESET << "\n";
    printSeparator();
}

// ==================== SETTING UP SYSTEM ====================
void systemSetUp() {
    printHeader("🌿 SETTING UP NURSERY SYSTEM");
    
    coordinator = new NurseryCoordinator();
    salesFloor = new SalesFloor(coordinator, 5, 5);
    greenhouse = new Greenhouse(coordinator, 6, 6);
    scheduler = new CareScheduler();
    
    coordinator->setSalesFloor(salesFloor);
    coordinator->setGreenhouse(greenhouse);

    // connecting salesFloor and greenhouse to mediator
    coordinator->registerColleague(salesFloor);
    coordinator->registerColleague(greenhouse);
    
    // cout << CYAN << "✓ Sales Floor created (5x5 grid)\n" << RESET;
    // cout << CYAN << "✓ Greenhouse created (6x6 grid)\n" << RESET;
    // cout << CYAN << "✓ Care Scheduler initialized\n\n" << RESET;
    std::cout << std::endl; 

    cout << YELLOW << "Creating staff...\n" << RESET;
    assistant = new SalesAssistant(coordinator, "Carlos Sainz", "SA-001");
    manager = new FloorManager(coordinator, "Charles Leclerc", "FM-001");
    owner = new NurseryOwner(coordinator, "Lando Norris", "NO-001");
    
    assistant->setNext(manager);
    manager->setNext(owner);
    
    // connecting staff to mediator
    coordinator->registerColleague(assistant);
    coordinator->registerColleague(manager);
    coordinator->registerColleague(owner);
    
    cout << GREEN << "-Sales Assistant: Carlos Sainz\n" << RESET;
    cout << GREEN << "-Floor Manager: Charles Leclerc\n" << RESET;
    cout << GREEN << "-Owner: Lando Norris\n\n" << RESET;
    
    cout << MAGENTA << "Growing plants in greenhouse...\n\n" << RESET;
    
    RoseFactory roseFactory;
    DaisyFactory daisyFactory;
    CactusFactory cactusFactory;
    AloeFactory aloeFactory;
    PotatoFactory potatoFactory;
    RadishFactory radishFactory;
    CarrotFactory carrotFactory;
    MonsteraFactory monsteraFactory;
    StrelitziaFactory strelitziaFactory;
    VenusFlyTrapFactory vftFactory;
    
    vector<Plant*> initialPlants;
    
    // MATURE PLANTS (Ready for Sale) - 15 plants
    // Roses (4)
    for (int i = 0; i < 4; i++) {
        Plant* rose = roseFactory.buildPlant(scheduler);
        rose->setState(new MatureState());
        rose->setPrice(65.0);
        initialPlants.push_back(rose);
    }

    // Daisies (3)
    for (int i = 0; i < 3; i++) {
        Plant* daisy = daisyFactory.buildPlant(scheduler);
        daisy->setState(new MatureState());
        daisy->setPrice(45.0);
        initialPlants.push_back(daisy);
    }

    // Cacti (2)
    for (int i = 0; i < 2; i++) {
        Plant* cactus = cactusFactory.buildPlant(scheduler);
        cactus->setState(new MatureState());
        cactus->setPrice(40.0);
        initialPlants.push_back(cactus);
    }

    // Strelitzias (3)
    for (int i = 0; i < 3; i++) {
        Plant* strelitzia = strelitziaFactory.buildPlant(scheduler);
        strelitzia->setState(new MatureState());
        strelitzia->setPrice(90.0);
        initialPlants.push_back(strelitzia);
    }

    // Venus Fly Traps (3)
    for (int i = 0; i < 3; i++) {
        Plant* vft = vftFactory.buildPlant(scheduler);
        vft->setState(new MatureState());
        vft->setPrice(55.0);
        initialPlants.push_back(vft);
    }

    // GROWING PLANTS (Almost Ready) - 12 plants
    // Aloe (3)
    for (int i = 0; i < 3; i++) {
        Plant* aloe = aloeFactory.buildPlant(scheduler);
        aloe->setState(new GrowingState());
        aloe->setWaterLevel(60 + (i * 5));
        aloe->setNutrientLevel(55 + (i * 5));
        aloe->setSunlightExposure(65 + (i * 3));
        aloe->updateHealth();
        initialPlants.push_back(aloe);
    }

    // Potatoes (3)
    for (int i = 0; i < 3; i++) {
        Plant* potato = potatoFactory.buildPlant(scheduler);
        potato->setState(new GrowingState());
        potato->setWaterLevel(50 + (i * 7));
        potato->setNutrientLevel(45 + (i * 8));
        potato->setSunlightExposure(60 + (i * 5));
        potato->updateHealth();
        initialPlants.push_back(potato);
    }

    // Carrots (3)
    for (int i = 0; i < 3; i++) {
        Plant* carrot = carrotFactory.buildPlant(scheduler);
        carrot->setState(new GrowingState());
        carrot->setWaterLevel(55 + (i * 6));
        carrot->setNutrientLevel(50 + (i * 7));
        carrot->setSunlightExposure(58 + (i * 4));
        carrot->updateHealth();
        initialPlants.push_back(carrot);
    }

    // Monsteras (3)
    for (int i = 0; i < 3; i++) {
        Plant* monstera = monsteraFactory.buildPlant(scheduler);
        monstera->setState(new GrowingState());
        monstera->setWaterLevel(58 + (i * 5));
        monstera->setNutrientLevel(53 + (i * 6));
        monstera->setSunlightExposure(62 + (i * 4));
        monstera->updateHealth();
        initialPlants.push_back(monstera);
    }

    // SEEDLING PLANTS (Young Plants) - 9 plants
    // Radishes (3)
    for (int i = 0; i < 3; i++) {
        Plant* radish = radishFactory.buildPlant(scheduler);
        radish->setState(new SeedlingState());
        radish->setWaterLevel(80 + (i * 2));
        radish->setNutrientLevel(75 + (i * 3));
        radish->setSunlightExposure(70 + (i * 2));
        radish->updateHealth();
        initialPlants.push_back(radish);
    }

    // More Monsteras (3)
    for (int i = 0; i < 3; i++) {
        Plant* monstera = monsteraFactory.buildPlant(scheduler);
        monstera->setState(new SeedlingState());
        monstera->setWaterLevel(85 + i);
        monstera->setNutrientLevel(80 + i);
        monstera->setSunlightExposure(75 + i);
        monstera->updateHealth();
        initialPlants.push_back(monstera);
    }

    // Carrots (3)
    for (int i = 0; i < 3; i++) {
        Plant* carrot = carrotFactory.buildPlant(scheduler);
        carrot->setState(new SeedlingState());
        carrot->setWaterLevel(82 + i);
        carrot->setNutrientLevel(77 + i);
        carrot->setSunlightExposure(72 + i);
        carrot->updateHealth();
        initialPlants.push_back(carrot);
    }
    
    // Set proper health levels for mature plants
    for (Plant* plant : initialPlants) {
        if (plant->getState()->getStateName() == "Mature") {
            plant->setWaterLevel(70);
            plant->setNutrientLevel(70);
            plant->setSunlightExposure(70);
            plant->updateHealth();
            plant->setReadyForSale(true);
        }
    }
    
    // Adding plants to greenhouse 
    int row = 0, col = 0;
    for (Plant* plant : initialPlants) {
        greenhouse->addPlant(plant, row, col);
        cout << CYAN << "  - " << plant->getName() << " (" 
             << plant->getState()->getStateName() << ")\n" << RESET;
        
        col++;
        if (col >= 6) {
            col = 0;
            row++;
        }
    }
    
    cout << "\n" << GREEN << "✓ " << initialPlants.size() 
         << " plants added to greenhouse\n\n" << RESET;
    
    cout << BLUE << "Moving mature plants to sales floor...\n" << RESET;
    coordinator->checkPlantRelocation();
    
    cout << "\n" << BOLD << GREEN << "✓ System initialized successfully!\n" << RESET;
    pause();
}

void cleanupSystem() {
    delete assistant;
    delete manager;
    delete owner;
    delete scheduler;
    delete salesFloor;
    delete greenhouse;
    delete coordinator;
}

// ==================== EMPLOYEE VIEW ====================

// employee looks at greenhouse
void employeeViewPlants() {
    clearScreen();
    printHeader("🏡 GREENHOUSE INSPECTION");
    
    displayGreenhouseGrid();
    
    vector<Plant*> plants = greenhouse->getAllPlants();
    
    if (plants.empty()) {
        cout << YELLOW << "\nNo plants in greenhouse.\n" << RESET;
        pause();
        return;
    }
    
    cout << "\n" << BOLD << "Select a plant to inspect (1-" << plants.size() << ", 0 to go back): " << RESET;
    int choice = getMenuChoice(0, plants.size());
    
    if (choice == 0) return;
    
    clearScreen();
    printHeader("🔍 PLANT DETAILS");
    displayPlantDetailed(plants[choice - 1]);

    cout << "\n" << BOLD << "Select an option (0 = back to greenhouse display, 1 = back to employee menu): " << RESET;
    int choice2 = getMenuChoice(0,1);

    if (choice2 == 0) employeeViewPlants();
    return;
}

// employee performs care duties on plants
void employeeCarePlant() {
    clearScreen();
    printHeader("💧 PLANT CARE SYSTEM");
    
    vector<Plant*> plants = greenhouse->getAllPlants();
    
    if (plants.empty()) {
        cout << YELLOW << "\nNo plants in greenhouse.\n" << RESET;
        pause();
        return;
    }
    
    // creating a list of all plants in order to interact and perform care
    cout << "\n" << BOLD << "Available Plants:\n" << RESET;
    for (size_t i = 0; i < plants.size(); i++) {
        Plant* p = plants[i];
        int health = p->getHealthLevel();
        string healthColor = health > 70 ? GREEN : (health > 40 ? YELLOW : RED);
        
        cout << "  " << (i + 1) << ". " << setw(15) << left << p->getName()
             << " - Health: " << healthColor << health << "%" << RESET << "\n";
    }
    
    cout << "\n" << BOLD << "Select plant to care for (1-" << plants.size() << ", 0 to cancel): " << RESET;
    int choice = getMenuChoice(0, plants.size());
    
    if (choice == 0) return;
    
    Plant* selectedPlant = plants[choice - 1];
    
    clearScreen();
    printHeader("🌱 CARING FOR PLANT");
    //displays selected plant information with bars
    displayPlantDetailed(selectedPlant);
    
    // displaying care options
    cout << "\n" << BOLD << "Care Options:\n" << RESET;
    cout << "1. " << BLUE << "Water Plant\n" << RESET;
    cout << "2. " << GREEN << "Fertilize Plant\n" << RESET;
    cout << "3. " << YELLOW << "Adjust Sunlight\n" << RESET;
    cout << "4. " << MAGENTA << "Prune Plant\n" << RESET;
    cout << "5. " << CYAN << "Perform Full Care Routine\n" << RESET;
    cout << "6. " << RED << "Back\n" << RESET;
    
    int careChoice = getMenuChoice(1, 6);
    
    if (careChoice == 6) return;
    
    cout << "\n" << GREEN << "Performing care...\n" << RESET;
    
    // switch statement for selected choice
    switch(careChoice) {
        case 1:
            selectedPlant->getStrategy()->water(selectedPlant); //get type of strategy so you can perform correct care for plant type
            break;
        case 2:
            selectedPlant->getStrategy()->fertilize(selectedPlant);
            break;
        case 3:
            selectedPlant->getStrategy()->adjustSunlight(selectedPlant);
            break;
        case 4:
            selectedPlant->getStrategy()->prune(selectedPlant);
            break;
        case 5:
            selectedPlant->performCare();
            break;
    }
    
    // after care is performed, need to update health
    selectedPlant->updateHealth();
    
    cout << "\n" << GREEN << "✓ Care completed!\n\n" << RESET;
    displayPlantDetailed(selectedPlant);
    
    cout << "\n" << BOLD << "Select an option (0 = back to plant list, 1 = back to employee menu): " << RESET;
    int choice2 = getMenuChoice(0,1);

    if (choice2 == 0) employeeCarePlant();
    return;
}

// employee views sales floor
void employeeViewSalesFloor() {
    clearScreen();
    printHeader("🪴  SALES FLOOR VIEW");
    
    displaySalesFloorGrid();
    
    vector<Plant*> plants = salesFloor->getDisplayPlants();
    
    // creating list of plants for sale
    if (!plants.empty()) {
        cout << "\n" << BOLD << "Plants for Sale:\n" << RESET;
        for (size_t i = 0; i < plants.size(); i++) {
            Plant* p = plants[i];
            cout << "  " << (i + 1) << ". " << setw(15) << left << p->getName()
                 << " - " << GREEN << formatPrice(p->getPrice()) << RESET << "\n";
        }
    }
    
    pause();
}

// employee handling a customer request - employee's pov
void employeeHandleRequest() {
    clearScreen();
    printHeader("💬 HANDLE CUSTOMER REQUEST");
    
    cout << YELLOW << "Enter customer's request: " << RESET;
    string requestText = getStringInput("");
    
    // Create a temporary customer for this request
    RegularCustomer* tempCustomer = new RegularCustomer();
    tempCustomer->setMediator(coordinator);
    tempCustomer->setName("Walk-in Customer");
    coordinator->registerColleague(tempCustomer);
    
    Request* req = tempCustomer->createRequest(requestText);
    
    cout << "\n" << CYAN << "Request Level: " << RESET;
    switch (req->getLevel()) {
        case RequestLevel::LOW: cout << GREEN << "LOW" << RESET; break;
        case RequestLevel::MEDIUM: cout << YELLOW << "MEDIUM" << RESET; break;
        case RequestLevel::HIGH: cout << RED << "HIGH" << RESET; break;
    }
    cout << "\n\n";
    
    // giving request to staff
    cout << BLUE << "Routing through staff chain...\n\n" << RESET;
    // tempCustomer asks sales assistant - first link in chain
    tempCustomer->submitRequestToStaff(assistant);
    
    coordinator->removeColleague(tempCustomer);
    delete tempCustomer;
    
    pause();
}

// employee menu
void employeeView() {
    string employeeName = getStringInput("\nEnter your name: ");
    
    while (true) {
        clearScreen();
        printHeader("👨‍🌾 EMPLOYEE MODE");
        
        cout << GREEN << "Welcome, " << BOLD << employeeName << RESET << "!\n\n";
        
        cout << "1. " << CYAN << "🏡 View Greenhouse Plants\n" << RESET;
        cout << "2. " << GREEN << "💧 Care for Plants\n" << RESET;
        cout << "3. " << YELLOW << "🪴  View Sales Floor\n" << RESET;
        cout << "4. " << MAGENTA << "💬 Handle Customer Request\n" << RESET;
        cout << "5. " << BLUE << "🔄 Move Mature Plants to Sales Floor\n" << RESET;
        cout << "6. " << RED << "🔙 Return to Main Menu\n" << RESET;
        
        int choice = getMenuChoice(1, 6);
        
        switch (choice) {
            case 1:
                employeeViewPlants();
                break;
            case 2:
                employeeCarePlant();
                break;
            case 3:
                employeeViewSalesFloor();
                break;
            case 4:
                employeeHandleRequest();
                break;
            case 5:
                clearScreen();
                cout << BLUE << "Checking for mature plants...\n" << RESET;
                coordinator->checkPlantRelocation();
                pause();
                break;
            case 6:
                return;
        }
    }
}

// ==================== CUSTOMER VIEW ====================

// helper functions first

// customer view's sales floor
void customerBrowsePlants(Customer* customer) {
    clearScreen();
    printHeader("🛍️ BROWSE PLANTS");
    
    displaySalesFloorGrid();
    
    vector<Plant*> plants = salesFloor->getDisplayPlants();
    
    if (plants.empty()) {
        cout << YELLOW << "\nNo plants available for sale right now.\n" << RESET;
        pause();
        return;
    }
    
    // creating list of available plants to select from
    cout << "\n" << BOLD << "Available Plants:\n" << RESET;
    for (size_t i = 0; i < plants.size(); i++) {
        Plant* p = plants[i];
        cout << "  " << (i + 1) << ". " << setw(15) << left << p->getName()
             << " - " << GREEN << formatPrice(p->getPrice()) << RESET << "\n";
    }
    
    cout << "\n" << BOLD << "Select plant to view details (1-" << plants.size() << ", 0 to go back): " << RESET;
    int choice = getMenuChoice(0, plants.size());
    
    if (choice == 0) return;
    
    Plant* selectedPlant = plants[choice - 1];
    
    clearScreen();
    printHeader("🌱 PLANT DETAILS");
    displayPlantDetailed(selectedPlant);
    
    cout << "\n" << CYAN << "Add this plant to your cart? (1 = Yes, 2 = No): " << RESET;
    int addChoice = getMenuChoice(1, 2);
    
    if (addChoice == 1) {
        // customer adds selected plant to cart - talks to mediator and mediator transfers the plant to the customer
        bool success = customer->addPlantFromSalesFloor(selectedPlant->getName());
        if (success) {
            cout << GREEN << "\n✓ Plant added to cart!\n" << RESET;
        } else {
            cout << RED << "\n✗ Failed to add plant to cart.\n" << RESET;
        }
    }

    cout << "\n" << CYAN << "Select an option (1 = continue browsing plants, 2 = back to customer menu): " << RESET;
    int choice2 = getMenuChoice(1,2);

    if (choice2 == 1) customerBrowsePlants(customer);
    return;
}

void customerViewCart(Customer* customer) {
    clearScreen();
    displayCartWithNumbers(customer);
    pause();
}

void customerDecorateItem(Customer* customer) {
    clearScreen();
    printHeader("🎨 DECORATE PLANTS");
    
    if (customer->getCartSize() == 0) {
        cout << YELLOW << "Your cart is empty!\n" << RESET;
        pause();
        return;
    }
    
    displayCartWithNumbers(customer);
    
    cout << "\n" << BOLD << "Select item to decorate (1-" << customer->getCartSize() << ", 0 to cancel): " << RESET;
    int choice = getMenuChoice(0, customer->getCartSize());
    
    if (choice == 0) return;
    
    // getting index of plant to decorate
    int itemIndex = choice - 1;
    Plant* plant = customer->getPlantFromCart(itemIndex);
    
    cout << "\n" << BOLD << "Decoration Options:\n" << RESET;
    cout << "1. " << MAGENTA << "🎀  Add Ribbon (+R15.00)\n" << RESET;
    cout << "2. " << CYAN << "🎁  Add Gift Wrap (+R20.00)\n" << RESET;
    cout << "3. " << GREEN << "🪴   Add Decorative Pot (+R80.00)\n" << RESET;
    cout << "4. " << RED << "❌  Cancel\n" << RESET;
    
    int decorChoice = getMenuChoice(1, 4);
    
    if (decorChoice == 4) return;
    
    double originalPrice = plant->getPrice();
    
    switch(decorChoice) {
        case 1:
            customer->decorateCartItemWithRibbon(itemIndex);
            cout << GREEN << "\n✓ Ribbon added!\n" << RESET;
            break;
        case 2:
            customer->decorateCartItemWithGiftWrap(itemIndex);
            cout << GREEN << "\n✓ Gift wrap added!\n" << RESET;
            break;
        case 3: {
            string color = getStringInput("\nEnter pot color (e.g., blue, red, terracotta): ");
            customer->decorateCartItemWithPot(itemIndex, color);
            cout << GREEN << "\n✓ " << color << " pot added!\n" << RESET;
            break;
        }
        case 4:
            customerDecorateItem(customer);
            break;
    }
    
    cout << "\nOriginal Price: " << formatPrice(originalPrice) << "\n";
    cout << "New Price: " << GREEN << formatPrice(customer->getPlantFromCart(itemIndex)->getPrice()) << RESET << "\n";
    
    // decorate more items in cart
    cout << "\n" << CYAN << "Select an option (1 = decorate more items in cart, 2 = back to customer menu): " << RESET;
    int choice2 = getMenuChoice(1,2);

    if (choice2 == 1) customerDecorateItem(customer);
    return;
}

// customer removes item from cart
void customerRemoveFromCart(Customer* customer) {
    clearScreen();
    printHeader("🗑️ REMOVE FROM CART");
    
    if (customer->getCartSize() == 0) {
        cout << YELLOW << "Your cart is empty!\n" << RESET;
        pause();
        return;
    }
    
    displayCartWithNumbers(customer);
    
    cout << "\n" << BOLD << "Select item to remove (1-" << customer->getCartSize() << ", 0 to cancel): " << RESET;
    int choice = getMenuChoice(0, customer->getCartSize());
    
    if (choice == 0) return;
    
    bool success = customer->returnPlantToSalesFloor(choice - 1);
    
    if (success) {
        cout << GREEN << "\n✓ Item returned to sales floor!\n" << RESET;
    } else {
        cout << RED << "\n✗ Failed to return item.\n" << RESET;
    }
    
    cout << "\n" << CYAN << "Select an option (1 = remove another item from cart, 2 = back to customer menu): " << RESET;
    int choice2 = getMenuChoice(1,2);

    if (choice2 == 1) customerRemoveFromCart(customer);
    return;
}

// customer checkout
void customerCheckout(Customer* customer) {
    clearScreen();
    printHeader("💳 CHECKOUT");
    
    // check cart is not empty
    if (customer->getCartSize() == 0) {
        cout << YELLOW << "Your cart is empty! Add some plants first.\n" << RESET;
        pause();
        return;
    }
    
    displayCartWithNumbers(customer);
    
    // build order
    string orderName = customer->getName() + "'s Order";
    customer->startNewOrder(orderName);
    customer->addEntireCartToOrder();
    
    FinalOrder* finalOrder = customer->createFinalOrder();
    
    if (!finalOrder) {
        cout << RED << "\n✗ Failed to create order.\n" << RESET;
        pause();
        return;
    }
    
    double total = finalOrder->calculateTotalPrice();
    
    cout << "\n" << BOLD << "Order Summary:\n" << RESET;
    printSeparator();
    finalOrder->printInvoice();
    printSeparator();
    
    cout << "\nYour Budget: " << formatPrice(customer->getBudget()) << "\n";
    cout << "Total Cost: " << formatPrice(total) << "\n";
    
    if (!customer->canAfford(total)) {
        cout << RED << "\n✗ Insufficient funds! You need " 
             << formatPrice(total - customer->getBudget()) << " more.\n" << RESET;
        delete finalOrder;
        pause();
        return;
    }
    
    cout << "\n" << BOLD << "Payment Methods:\n" << RESET;
    cout << "1. " << GREEN << "💵 Cash Payment\n" << RESET;
    cout << "2. " << BLUE << "💳 Credit Card Payment\n" << RESET;
    cout << "3. " << RED << "❌ Cancel\n" << RESET;
    
    int payChoice = getMenuChoice(1, 3);
    
    if (payChoice == 3) {
        delete finalOrder;
        return;
    }
    
    // template method - creating payment process types
    PaymentProcessor* processor = nullptr;
    
    if (payChoice == 1) {
        processor = new CashPayment();
    } else {
        processor = new CreditCardPayment();
    }
    
    cout << "\n";
    processor->processTransaction(finalOrder);
    
    customer->deductFromBudget(total);
    customer->clearCart();
    
    cout << "\n" << GREEN << BOLD << "✓ Purchase complete!\n" << RESET;
    cout << "Remaining budget: " << GREEN << formatPrice(customer->getBudget()) << RESET << "\n";
    
    delete processor;
    delete finalOrder;
    
    pause();
}

// customer makes a request to a sales assistant
void customerMakeRequest(Customer* customer) {
    clearScreen();
    printHeader("💬 MAKE A REQUEST");
    
    cout << CYAN << "What would you like help with?\n" << RESET;
    cout << YELLOW << "(Examples: \"I need help finding a Rose\", \"I want 50 plants for wedding\", \"I want a refund\")\n\n" << RESET;
    
    string requestText = getStringInput("Your request: ");
    
    Request* req = customer->createRequest(requestText);
    
    cout << "\n" << CYAN << "Request Level: " << RESET;
    switch (req->getLevel()) {
        case RequestLevel::LOW: cout << GREEN << "LOW" << RESET; break;
        case RequestLevel::MEDIUM: cout << YELLOW << "MEDIUM" << RESET; break;
        case RequestLevel::HIGH: cout << RED << "HIGH" << RESET; break;
    }
    cout << "\n\n";
    
    cout << BLUE << "Submitting to staff...\n\n" << RESET;
    customer->submitRequestToStaff(assistant);
    
    pause();
}

// customer menu
void customerView() {
    clearScreen();

    // have to create customer first before the can use menu
    printHeader("👤 CUSTOMER REGISTRATION");
    
    string name = getStringInput("Enter your name: ");
    
    cout << "\n" << BOLD << "Customer Type:\n" << RESET;
    cout << "1. " << GREEN << "Regular Customer (Budget: R300)\n" << RESET;
    cout << "2. " << CYAN << "Walk-In Customer (Budget: R150)\n" << RESET;
    cout << "3. " << YELLOW << "Corporate Customer (Budget: R1000)\n" << RESET;
    
    int typeChoice = getMenuChoice(1, 3);
    
    Customer* customer = nullptr;
    
    switch(typeChoice) {
        case 1:
            customer = new RegularCustomer();
            break;
        case 2:
            customer = new WalkInCustomer();
            break;
        case 3:
            customer = new CorporateCustomer();
            break;
    }
    
    // customer uses correct mediator
    customer->setMediator(coordinator);
    customer->setName(name);
    // register customer with mediator
    coordinator->registerColleague(customer);
    
    while (true) {
        clearScreen();
        printHeader("🛍️ CUSTOMER MODE");
        
        cout << GREEN << "Welcome, " << BOLD << name << RESET << "!\n";
        cout << "Budget: " << GREEN << formatPrice(customer->getBudget()) << RESET << "\n";
        cout << "Cart: " << CYAN << customer->getCartSize() << " items" << RESET << "\n\n";
        
        cout << "1. " << CYAN << "🛍️  Browse Plants\n" << RESET;
        cout << "2. " << GREEN << "🛒 View Cart\n" << RESET;
        cout << "3. " << MAGENTA << "🎨 Decorate Plant\n" << RESET;
        cout << "4. " << YELLOW << "🗑️  Remove from Cart\n" << RESET;
        cout << "5. " << BLUE << "💬 Make a Request to Staff\n" << RESET;
        cout << "6. " << CYAN << "💳 Checkout\n" << RESET;
        cout << "7. " << RED << "❌ Leave Store\n" << RESET;
        
        int choice = getMenuChoice(1, 7);
        
        switch (choice) {
            case 1:
                customerBrowsePlants(customer);
                break;
            case 2:
                customerViewCart(customer);
                break;
            case 3:
                customerDecorateItem(customer);
                break;
            case 4:
                customerRemoveFromCart(customer);
                break;
            case 5:
                customerMakeRequest(customer);
            break;
            case 6:
                customerCheckout(customer);
                break;
            case 7:
                coordinator->removeColleague(customer);
                delete customer;
                return;
        }
    }
}

// ==================== MAIN MENU ====================

void mainMenu() {
    while (true) {
        clearScreen();
        
        cout << GREEN << BOLD;
        cout << "╔════════════════════════════════════════════════════════════╗\n";
        cout << "║                                                            ║\n";
        cout << "║              🌱 PLANT NURSERY SIMULATOR 🌱                 ║\n";
        cout << "║                                                            ║\n";
        cout << "║                     The Girls + Kyle                       ║\n";
        cout << "║                                                            ║\n";
        cout << "╚════════════════════════════════════════════════════════════╝\n";
        cout << RESET << endl;
        
        cout << "  🏡 Greenhouse: " << greenhouse->getNumberOfPlants() << " plants\n";
        cout << "  🪴 Sales Floor: " << salesFloor->getNumberOfPlants() << " plants\n\n";
        
        printSeparator();
        
        cout << BOLD << "\nSelect Your Role:\n\n" << RESET;
        
        cout << "1. " << GREEN << "🧍 Employee Mode\n" << RESET;
        cout << "   " << CYAN << "Care for plants, handle requests, manage inventory\n" << RESET;
        
        cout << "\n2. " << YELLOW << "🛍️  Customer Mode\n" << RESET;
        cout << "   " << CYAN << "Browse, shop, decorate plants, checkout\n" << RESET;
        
        cout << "\n3. " << BLUE << "📊 View System Status\n" << RESET;
        cout << "   " << CYAN << "See greenhouse and sales floor overview\n" << RESET;
        
        cout << "\n4. " << RED << "❌ Exit Program\n" << RESET;
        
        int choice = getMenuChoice(1, 4);
        
        switch (choice) {
            case 1:
                employeeView();
                break;
            case 2:
                customerView();
                break;
            case 3: {
                clearScreen();
                printHeader("📊 SYSTEM STATUS");
                displayGreenhouseGrid();
                cout << "\n";
                displaySalesFloorGrid();
                pause();
                break;
            }
            case 4:
                clearScreen();
                cout << GREEN << BOLD << "\n🌱 Thank you for using our Nursery System! 🌱\n\n" << RESET;
                return;
        }
    }
}

// ==================== AUTOMATED DEMO ====================

void runAutomatedDemo() {
    cout << CYAN << "\n🤖 Running Automated Demo for Coverage Testing...\n\n" << RESET;

    try {
        // Test 1: Employee views greenhouse
        cout << GREEN << "Test 1: Employee viewing greenhouse plants...\n" << RESET;
        vector<Plant*> greenhousePlants = greenhouse->getAllPlants();
        if (!greenhousePlants.empty()) {
            displayPlantDetailed(greenhousePlants[0]);
        }

        // Test 2: Employee cares for plants
        cout << GREEN << "\nTest 2: Employee caring for plants...\n" << RESET;
        if (!greenhousePlants.empty()) {
            Plant* testPlant = greenhousePlants[0];
            testPlant->getStrategy()->water(testPlant);
            testPlant->getStrategy()->fertilize(testPlant);
            testPlant->getStrategy()->adjustSunlight(testPlant);
            testPlant->getStrategy()->prune(testPlant);
            testPlant->performCare();
            testPlant->updateHealth();
        }

        // Test 3: View sales floor
        cout << GREEN << "\nTest 3: Viewing sales floor...\n" << RESET;
        vector<Plant*> salesPlants = salesFloor->getDisplayPlants();
        displaySalesFloorGrid();

        // Test 4: Customer shopping experience
        cout << GREEN << "\nTest 4: Customer shopping experience...\n" << RESET;
        RegularCustomer* autoCustomer = new RegularCustomer();
        autoCustomer->setMediator(coordinator);
        autoCustomer->setName("Automated Test Customer");
        coordinator->registerColleague(autoCustomer);

        // Browse and add plants to cart
        if (!salesPlants.empty()) {
            autoCustomer->addPlantFromSalesFloor(salesPlants[0]->getName());
            if (salesPlants.size() > 1) {
                autoCustomer->addPlantFromSalesFloor(salesPlants[1]->getName());
            }
        }

        displayCartWithNumbers(autoCustomer);

        // Test 5: Decorate plants
        cout << GREEN << "\nTest 5: Decorating plants...\n" << RESET;
        if (autoCustomer->getCartSize() > 0) {
            autoCustomer->decorateCartItemWithRibbon(0);
            if (autoCustomer->getCartSize() > 1) {
                autoCustomer->decorateCartItemWithGiftWrap(1);
                autoCustomer->decorateCartItemWithPot(1, "terracotta");
            }
        }

        displayCartWithNumbers(autoCustomer);

        // Test 6: Customer requests
        cout << GREEN << "\nTest 6: Testing customer requests...\n" << RESET;
        Request* lowReq = autoCustomer->createRequest("I need help finding a Rose");
        autoCustomer->submitRequestToStaff(assistant);

        Request* medReq = autoCustomer->createRequest("I want 50 plants for wedding");
        autoCustomer->submitRequestToStaff(assistant);

        Request* highReq = autoCustomer->createRequest("I want a refund");
        autoCustomer->submitRequestToStaff(assistant);

        // Test 7: Checkout with different payment methods
        cout << GREEN << "\nTest 7: Testing checkout process...\n" << RESET;
        if (autoCustomer->getCartSize() > 0) {
            string orderName = autoCustomer->getName() + "'s Order";
            autoCustomer->startNewOrder(orderName);
            autoCustomer->addEntireCartToOrder();

            FinalOrder* finalOrder = autoCustomer->createFinalOrder();

            if (finalOrder) {
                double total = finalOrder->calculateTotalPrice();
                finalOrder->printInvoice();

                if (autoCustomer->canAfford(total)) {
                    // Test cash payment
                    CashPayment* cashProc = new CashPayment();
                    cashProc->processTransaction(finalOrder);
                    delete cashProc;

                    autoCustomer->deductFromBudget(total);
                }

                delete finalOrder;
            }
        }

        autoCustomer->clearCart();

        // Test 8: Another customer with credit card
        cout << GREEN << "\nTest 8: Testing credit card payment...\n" << RESET;
        if (!salesPlants.empty() && salesPlants.size() > 2) {
            autoCustomer->addPlantFromSalesFloor(salesPlants[2]->getName());

            if (autoCustomer->getCartSize() > 0) {
                autoCustomer->startNewOrder("Credit Card Order");
                autoCustomer->addEntireCartToOrder();
                FinalOrder* ccOrder = autoCustomer->createFinalOrder();

                if (ccOrder) {
                    CreditCardPayment* ccProc = new CreditCardPayment();
                    ccProc->processTransaction(ccOrder);
                    delete ccProc;
                    delete ccOrder;
                }
            }
        }

        // Test 9: Remove from cart
        cout << GREEN << "\nTest 9: Testing cart operations...\n" << RESET;
        if (!salesPlants.empty()) {
            autoCustomer->addPlantFromSalesFloor(salesPlants[0]->getName());
            if (autoCustomer->getCartSize() > 0) {
                autoCustomer->returnPlantToSalesFloor(0);
            }
        }

        // Test 10: Plant relocation
        cout << GREEN << "\nTest 10: Testing plant relocation...\n" << RESET;
        coordinator->checkPlantRelocation();

        // Test 11: Different customer types
        cout << GREEN << "\nTest 11: Testing different customer types...\n" << RESET;
        WalkInCustomer* walkIn = new WalkInCustomer();
        walkIn->setMediator(coordinator);
        walkIn->setName("Walk-In Test");
        coordinator->registerColleague(walkIn);

        CorporateCustomer* corporate = new CorporateCustomer();
        corporate->setMediator(coordinator);
        corporate->setName("Corporate Test");
        coordinator->registerColleague(corporate);

        // Cleanup test customers
        coordinator->removeColleague(autoCustomer);
        coordinator->removeColleague(walkIn);
        coordinator->removeColleague(corporate);
        delete autoCustomer;
        delete walkIn;
        delete corporate;

        cout << GREEN << BOLD << "\n✓ Automated demo completed successfully!\n" << RESET;

    } catch (const exception& e) {
        cout << RED << "Error in automated demo: " << e.what() << RESET << endl;
    }
}

// ==================== MAIN FUNCTION ====================

int main() {
    cout << fixed << setprecision(2);

    // Check if running in CI environment
    if (getenv("CI") != nullptr) {
        cout << CYAN << "🤖 CI Environment Detected - Running Automated Demo\n" << RESET;

        try {
            systemSetUp();
            runAutomatedDemo();
            cleanupSystem();
        } catch (const exception& e) {
            cout << RED << "\n❌ Error: " << e.what() << RESET << endl;
            return 1;
        }

        return 0;
    }

    // Interactive mode for normal execution
    cout << GREEN << BOLD;
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║                                                            ║\n";
    cout << "║              🌱 PLANT NURSERY SIMULATOR 🌱                 ║\n";
    cout << "║                                                            ║\n";
    cout << "║                    The Girls + Kyle                        ║\n";
    cout << "║                                                            ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";
    cout << RESET << endl;

    pause("\nPress Enter to initialize system...");

    try {
        systemSetUp();
        mainMenu();
        cleanupSystem();
    } catch (const exception& e) {
        cout << RED << "\n❌ Error: " << e.what() << RESET << endl;
        return 1;
    }

    return 0;
}