#include "ScreenManager.h"
#include "raylib.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// Backend includes
#include "../include/NurseryCoordinator.h"
#include "../include/Customer.h"
#include "../include/DerivedCustomers.h"
#include "../include/SalesFloor.h"
#include "../include/Greenhouse.h"
#include "../include/CareScheduler.h"
#include "../include/SalesAssistant.h"
#include "../include/FloorManager.h"
#include "../include/NurseryOwner.h"
#include "../include/Plant.h"
#include "../include/FinalOrder.h"

// Factory includes
#include "../include/RoseFactory.h"
#include "../include/DaisyFactory.h"
#include "../include/CactusFactory.h"
#include "../include/AloeFactory.h"
#include "../include/PotatoFactory.h"
#include "../include/StrelitziaFactory.h"
#include "../include/RadishFactory.h"
#include "../include/MonsteraFactory.h"
#include "../include/VenusFlyTrapFactory.h"
#include "../include/CarrotFactory.h"
#include "../include/ConcreteOrder.h"
#include "../include/Leaf.h"

ScreenManager::ScreenManager()
    : currentScreen(GameScreen::START),
      previousScreen(GameScreen::START),
      mediator(nullptr),
      customer(nullptr),
      salesFloor(nullptr),
      greenhouse(nullptr),
      careScheduler(nullptr),
      salesAssistant(nullptr),
      floorManager(nullptr),
      nurseryOwner(nullptr),
      currentPlantBeingDecorated(nullptr),
      currentCartIndex(0),
      finalOrder(nullptr),
      previousOrder(nullptr),
      hasShownReorderNotification(false),
      lastUpdateTime(0.0f),
      daysCounter(0),
      useAlternativeColors(false) {

    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

ScreenManager::~ScreenManager() {
    Cleanup();
}

void ScreenManager::Initialize() {
    std::cout << "[ScreenManager] Initializing..." << std::endl;
    
    // Create mediator
    mediator = new NurseryCoordinator();
    std::cout << "[ScreenManager] Created NurseryCoordinator" << std::endl;
    
    // Create greenhouse (6x8 = 48 slots)
    greenhouse = new Greenhouse(mediator, 6, 8);
    mediator->setGreenhouse(greenhouse);
    mediator->registerColleague(greenhouse);
    std::cout << "[ScreenManager] Created Greenhouse (6x8)" << std::endl;
    
    // Create sales floor (5x5 = 25 slots)
    salesFloor = new SalesFloor(mediator, 5, 5);
    mediator->setSalesFloor(salesFloor);
    mediator->registerColleague(salesFloor);
    std::cout << "[ScreenManager] Created SalesFloor (5x5)" << std::endl;
    
    // Create care scheduler
    careScheduler = new CareScheduler();
    std::cout << "[ScreenManager] Created CareScheduler" << std::endl;
    
    // Create staff chain of responsibility
    salesAssistant = new SalesAssistant(mediator, "Sales Assistant", "SA-001");
    floorManager = new FloorManager(mediator, "Floor Manager", "FM-001");
    nurseryOwner = new NurseryOwner(mediator, "Nursery Owner", "NO-001");
    salesAssistant->setNext(floorManager);
    floorManager->setNext(nurseryOwner);
    std::cout << "[ScreenManager] Created Staff Chain of Responsibility" << std::endl;
    
    // Populate greenhouse with initial plants
    PopulateInitialGreenhouse();
    std::cout << "[ScreenManager] Populated initial greenhouse" << std::endl;
    
    // Populate sales floor with mature plants
    PopulateInitialSalesFloor();
    std::cout << "[ScreenManager] Populated initial sales floor" << std::endl;
    
    // Load assets
    LoadAssets();
    std::cout << "[ScreenManager] Loaded assets" << std::endl;
    
    // Initialize time tracking
    lastUpdateTime = GetTime();
    daysCounter = 0;

    // Create a sample previous order for Prototype pattern demonstration
    CreateSamplePreviousOrder();

    std::cout << "[ScreenManager] Initialization complete!" << std::endl;
}

void ScreenManager::PopulateInitialGreenhouse() {
    // Create factories
    RoseFactory roseFactory;
    DaisyFactory daisyFactory;
    CactusFactory cactusFactory;
    AloeFactory aloeFactory;
    PotatoFactory potatoFactory;
    StrelitziaFactory strelitziaFactory;
    RadishFactory radishFactory;
    MonsteraFactory monsteraFactory;
    VenusFlyTrapFactory vftFactory;
    CarrotFactory carrotFactory;
    
    std::vector<PlantFactory*> factories = {
        &roseFactory, &daisyFactory, &cactusFactory, 
        &aloeFactory, &potatoFactory, &strelitziaFactory,
        &radishFactory, &monsteraFactory, &vftFactory, &carrotFactory 
    };
    
    // Fill ~60% of greenhouse with plants
    int plantsToCreate = static_cast<int>((6 * 8) * 0.6); // ~29 plants
    
    for (int i = 0; i < plantsToCreate; i++) {
        // Random factory
        int factoryIndex = std::rand() % factories.size();
        PlantFactory* factory = factories[factoryIndex];
        
        // Create plant with scheduler
        Plant* plant = factory->buildPlant(careScheduler);
        
        // Give plants random starting ages (0-10 days)
        int startingAge = std::rand() % 11;
        for (int age = 0; age < startingAge; age++) {
            plant->dailyUpdate();
        }
        
        // Find empty spot in greenhouse
        bool placed = false;
        int attempts = 0;
        while (!placed && attempts < 100) {
            int row = std::rand() % 6;
            int col = std::rand() % 8;
            if (greenhouse->isPositionEmpty(row, col)) {
                greenhouse->addPlant(plant, row, col);
                placed = true;
            }
            attempts++;
        }
        
        if (!placed) {
            std::cout << "[ScreenManager] Warning: Could not place plant, greenhouse may be full" << std::endl;
            delete plant;
        }
    }
    
    std::cout << "[ScreenManager] Created " << plantsToCreate << " plants in greenhouse" << std::endl;
}

void ScreenManager::PopulateInitialSalesFloor() {
    // Create factories
    RoseFactory roseFactory;
    DaisyFactory daisyFactory;
    CactusFactory cactusFactory;
    AloeFactory aloeFactory;
    PotatoFactory potatoFactory;
    StrelitziaFactory strelitziaFactory;
    RadishFactory radishFactory;
    MonsteraFactory monsteraFactory;
    VenusFlyTrapFactory vftFactory;
    CarrotFactory carrotFactory;

    std::vector<PlantFactory*> factories = {
        &roseFactory, &daisyFactory, &cactusFactory,
        &aloeFactory, &potatoFactory, &strelitziaFactory,
        &radishFactory, &monsteraFactory, &vftFactory, &carrotFactory
    };

    // Create 10-12 mature plants for the sales floor
    int plantsToCreate = 10 + (std::rand() % 3); // 10-12 plants

    // Track which factories have been used to ensure variety
    std::vector<bool> factoryUsed(factories.size(), false);
    int plantsCreated = 0;

    // PHASE 1: Ensure at least one of each type (up to the total we need)
    std::vector<int> factoryOrder;
    for (size_t i = 0; i < factories.size(); i++) {
        factoryOrder.push_back(i);
    }

    // Shuffle the factory order for randomness
    for (size_t i = factoryOrder.size() - 1; i > 0; i--) {
        size_t j = std::rand() % (i + 1);
        std::swap(factoryOrder[i], factoryOrder[j]);
    }

    // Create at least one of each type (or until we reach our limit)
    for (size_t i = 0; i < factoryOrder.size() && plantsCreated < plantsToCreate; i++) {
        int factoryIndex = factoryOrder[i];
        PlantFactory* factory = factories[factoryIndex];
        factoryUsed[factoryIndex] = true;

        // Create plant without scheduler for sales floor (they're ready to sell)
        Plant* plant = factory->buildPlant(nullptr);

        // Age the plant to mature state (35+ days for mature, 50+ for flowering)
        int targetAge = 35 + (std::rand() % 20); // 35-54 days

        for (int age = 0; age < targetAge; age++) {
            // Keep plant healthy while aging
            plant->setWaterLevel(80);
            plant->setNutrientLevel(80);
            plant->setSunlightExposure(70);
            plant->updateHealth();
            plant->incrementAge();

            // Manually trigger state changes
            if (plant->getState() != nullptr) {
                plant->getState()->handleChange(plant);
            }
        }

        // Ensure plant is ready for sale
        plant->setReadyForSale(true);

        // Set a reasonable price if not already set
        if (plant->getPrice() < 10.0) {
            plant->setPrice(15.0 + (std::rand() % 35)); // R15-R50
        }

        // Find empty spot in sales floor
        bool placed = false;
        int attempts = 0;
        while (!placed && attempts < 100) {
            int row = std::rand() % 5;
            int col = std::rand() % 5;
            if (salesFloor->isPositionEmpty(row, col)) {
                salesFloor->addPlantToDisplay(plant, row, col);
                placed = true;
                plantsCreated++;
                std::cout << "[ScreenManager] Placed " << plant->getName()
                          << " (ID: " << plant->getID() << ") on sales floor at ("
                          << row << "," << col << ") - State: "
                          << plant->getState()->getStateName()
                          << ", Price: R" << plant->getPrice() << std::endl;
            }
            attempts++;
        }

        if (!placed) {
            std::cout << "[ScreenManager] Warning: Could not place plant on sales floor" << std::endl;
            delete plant;
        }
    }

    // PHASE 2: Fill remaining slots with random plants (including repeats)
    while (plantsCreated < plantsToCreate) {
        // Random factory (can be any, including ones already used)
        int factoryIndex = std::rand() % factories.size();
        PlantFactory* factory = factories[factoryIndex];

        // Create plant without scheduler for sales floor (they're ready to sell)
        Plant* plant = factory->buildPlant(nullptr);

        // Age the plant to mature state (35+ days for mature, 50+ for flowering)
        int targetAge = 35 + (std::rand() % 20); // 35-54 days

        for (int age = 0; age < targetAge; age++) {
            // Keep plant healthy while aging
            plant->setWaterLevel(80);
            plant->setNutrientLevel(80);
            plant->setSunlightExposure(70);
            plant->updateHealth();
            plant->incrementAge();

            // Manually trigger state changes
            if (plant->getState() != nullptr) {
                plant->getState()->handleChange(plant);
            }
        }

        // Ensure plant is ready for sale
        plant->setReadyForSale(true);

        // Set a reasonable price if not already set
        if (plant->getPrice() < 10.0) {
            plant->setPrice(15.0 + (std::rand() % 35)); // R15-R50
        }

        // Find empty spot in sales floor
        bool placed = false;
        int attempts = 0;
        while (!placed && attempts < 100) {
            int row = std::rand() % 5;
            int col = std::rand() % 5;
            if (salesFloor->isPositionEmpty(row, col)) {
                salesFloor->addPlantToDisplay(plant, row, col);
                placed = true;
                plantsCreated++;
                std::cout << "[ScreenManager] Placed " << plant->getName()
                          << " (ID: " << plant->getID() << ") on sales floor at ("
                          << row << "," << col << ") - State: "
                          << plant->getState()->getStateName()
                          << ", Price: R" << plant->getPrice() << std::endl;
            }
            attempts++;
        }

        if (!placed) {
            std::cout << "[ScreenManager] Warning: Could not place plant on sales floor" << std::endl;
            delete plant;
        }
    }

    std::cout << "[ScreenManager] Created " << plantsCreated << " mature plants for sales floor with variety" << std::endl;
}

void ScreenManager::CreateSamplePreviousOrder() {
    std::cout << "[ScreenManager] Creating sample previous order for Prototype demonstration..." << std::endl;

    // Create a sample final order
    previousOrder = new FinalOrder("Sample Customer");

    // Create factories for some popular plants
    RoseFactory roseFactory;
    DaisyFactory daisyFactory;
    MonsteraFactory monsteraFactory;

    // Create a composite order (suborder)
    ConcreteOrder* flowerBundle = new ConcreteOrder("Flower Bundle");

    // Create sample plants with realistic prices
    Plant* rose = roseFactory.buildPlant(nullptr);
    rose->setPrice(25.00);
    rose->setReadyForSale(true);
    Leaf* roseLeaf = new Leaf(rose, true);
    flowerBundle->add(roseLeaf);

    Plant* daisy = daisyFactory.buildPlant(nullptr);
    daisy->setPrice(18.00);
    daisy->setReadyForSale(true);
    Leaf* daisyLeaf = new Leaf(daisy, true);
    flowerBundle->add(daisyLeaf);

    // Add the flower bundle to the order
    previousOrder->addOrder(flowerBundle);

    // Add an individual plant (not in a suborder)
    Plant* monstera = monsteraFactory.buildPlant(nullptr);
    monstera->setPrice(35.00);
    monstera->setReadyForSale(true);
    Leaf* monsteraLeaf = new Leaf(monstera, true);
    previousOrder->addOrder(monsteraLeaf);

    std::cout << "[ScreenManager] Sample previous order created with total: R"
              << previousOrder->calculateTotalPrice() << std::endl;
    std::cout << "[ScreenManager] Order contents:" << std::endl;
    previousOrder->printInvoice();
}

void ScreenManager::LoadAssets() {
    std::cout << "[ScreenManager] Loading plant textures..." << std::endl;
    
    // Load plant textures
    plantTextures["Rose"] = LoadTexture("assets/rose.png");
    plantTextures["Daisy"] = LoadTexture("assets/daisy.png");
    plantTextures["Cactus"] = LoadTexture("assets/cactus.png");
    plantTextures["Aloe"] = LoadTexture("assets/aloe.png");
    plantTextures["Potato"] = LoadTexture("assets/potato.png");
    plantTextures["Strelitzia"] = LoadTexture("assets/strelitzia.png");
    plantTextures["Radish"] = LoadTexture("assets/radish.png");
    plantTextures["Monstera"] = LoadTexture("assets/monstera.png");
    plantTextures["Venus Fly Trap"] = LoadTexture("assets/VenusFlyTrap.png");
    plantTextures["Carrot"] = LoadTexture("assets/carrot.png");
    
    // Load pot textures
    std::vector<std::string> potColors = {
        "blue", "red", "terracotta", "green", "yellow",
        "pink", "purple", "orange", "black", "white"
    };
    
    for (const std::string& color : potColors) {
        std::string path = "assets/pots/pot_" + color + ".png";
        potTextures[color] = LoadTexture(path.c_str());
    }
    
    // Load ribbon texture
    ribbonTexture = LoadTexture("assets/ribbon.png");
    
    // Load UI icons (optional - will work even if files don't exist)
    moneyIcon = LoadTexture("assets/money_icon.png");
    cartIcon = LoadTexture("assets/cart_icon.png");
    
    std::cout << "[ScreenManager] Assets loaded successfully" << std::endl;
}

void ScreenManager::UnloadAssets() {
    std::cout << "[ScreenManager] Unloading assets..." << std::endl;

    auto safeUnload = [](Texture2D &tx) {
        if (tx.id != 0) {
            UnloadTexture(tx);
            tx.id = 0;
            tx.width = tx.height = 0;
        }
    };
    
    // Unload plant textures
    for (auto& pair : plantTextures) {
        safeUnload(pair.second);
    }
    plantTextures.clear();
    
    // Unload pot textures
    for (auto& pair : potTextures) {
        safeUnload(pair.second);
    }
    potTextures.clear();
    
    // Unload other textures
    safeUnload(ribbonTexture);
    safeUnload(moneyIcon);
    safeUnload(cartIcon);
}

void ScreenManager::Update() {
    // Real-time plant growth system
    float currentTime = GetTime();
    float deltaTime = currentTime - lastUpdateTime;
    
    // Daily update every 60 seconds (adjustable)
    const float DAILY_UPDATE_INTERVAL = 60.0f;
    
    if (deltaTime >= DAILY_UPDATE_INTERVAL) {
        PerformDailyUpdate();
        lastUpdateTime = currentTime;
    }
    
    // Screen-specific updates would be handled by individual screen classes
    // For now, this is a placeholder for the main update loop
}

void ScreenManager::PerformDailyUpdate() {
    std::cout << "\n[ScreenManager] ===== DAILY UPDATE =====" << std::endl;
    
    // Update all plants in greenhouse
    std::vector<Plant*> allPlants = greenhouse->getAllPlants();
    for (Plant* plant : allPlants) {
        if (plant != nullptr) {
            plant->dailyUpdate();
        }
    }
    
    // Increment days counter
    daysCounter++;
    std::cout << "[ScreenManager] Day " << daysCounter << " complete" << std::endl;
    
    // Auto-transfer mature plants to sales floor
    std::cout << "[ScreenManager] Checking for plant relocations..." << std::endl;
    mediator->checkPlantRelocation();
    
    std::cout << "[ScreenManager] ========================\n" << std::endl;
}

void ScreenManager::Draw() {
    // Drawing is handled by individual screen classes
    // This is a placeholder for the main draw loop
}

void ScreenManager::Cleanup() {
    if (isCleanedUp) {
        std::cout << "[ScreenManager] Cleanup already done, skipping...\n";
        return;
    }

    std::cout << "[ScreenManager] Cleaning up..." << std::endl;

    auto safeUnload = [](Texture2D &tx) {
        if (tx.id != 0) {
            UnloadTexture(tx);
            tx.id = 0;
            tx.width = tx.height = 0;
        }
    };
    
    // Delete customer if exists
    if (customer != nullptr) {
        delete customer;
        customer = nullptr;
    }
    
    // Delete final order if exists
    if (finalOrder != nullptr) {
        delete finalOrder;
        finalOrder = nullptr;
    }

    // Delete previous order if exists
    if (previousOrder != nullptr) {
        delete previousOrder;
        previousOrder = nullptr;
    }

    // Delete staff members
    if (salesAssistant != nullptr) {
        delete salesAssistant;
        salesAssistant = nullptr;
    }
    if (floorManager != nullptr) {
        delete floorManager;
        floorManager = nullptr;
    }
    if (nurseryOwner != nullptr) {
        delete nurseryOwner;
        nurseryOwner = nullptr;
    }
    
    // Delete care scheduler
    if (careScheduler != nullptr) {
        delete careScheduler;
        careScheduler = nullptr;
    }
    
// Delete sales floor (contains plants) - DON'T delete plants separately
if (salesFloor != nullptr) {
    delete salesFloor;  // Destructor handles plant deletion
    salesFloor = nullptr;
}

// Delete greenhouse (contains plants) - DON'T delete plants separately  
if (greenhouse != nullptr) {
    delete greenhouse;  // Destructor handles plant deletion
    greenhouse = nullptr;
}
    
    // Delete mediator
    if (mediator != nullptr) {
        delete mediator;
        mediator = nullptr;
    }
    
    // Unload assets
    UnloadAssets();
    
    std::cout << "[ScreenManager] Cleanup complete" << std::endl;
    isCleanedUp = true;
}

void ScreenManager::SwitchScreen(GameScreen newScreen) {
    previousScreen = currentScreen;
    currentScreen = newScreen;
    std::cout << "[ScreenManager] Switched from screen " 
              << static_cast<int>(previousScreen) 
              << " to screen " 
              << static_cast<int>(currentScreen) << std::endl;
}

GameScreen ScreenManager::GetCurrentScreen() const {
    return currentScreen;
}

GameScreen ScreenManager::GetPreviousScreen() const {
    return previousScreen;
}

// State getters
Customer* ScreenManager::GetCustomer() const {
    return customer;
}

SalesFloor* ScreenManager::GetSalesFloor() const {
    return salesFloor;
}

Greenhouse* ScreenManager::GetGreenhouse() const {
    return greenhouse;
}

CareScheduler* ScreenManager::GetCareScheduler() const {
    return careScheduler;
}

NurseryCoordinator* ScreenManager::GetMediator() const {
    return mediator;
}

SalesAssistant* ScreenManager::GetSalesAssistant() const {
    return salesAssistant;
}

FloorManager* ScreenManager::GetFloorManager() const {
    return floorManager;
}

NurseryOwner* ScreenManager::GetNurseryOwner() const {
    return nurseryOwner;
}

// Temporary state management
void ScreenManager::SetCurrentPlantBeingDecorated(Plant* plant) {
    currentPlantBeingDecorated = plant;
}

Plant* ScreenManager::GetCurrentPlantBeingDecorated() const {
    return currentPlantBeingDecorated;
}

void ScreenManager::SetCurrentCartIndex(int index) {
    currentCartIndex = index;
}

int ScreenManager::GetCurrentCartIndex() const {
    return currentCartIndex;
}

void ScreenManager::SetFinalOrder(FinalOrder* order) {
    if (finalOrder != nullptr && finalOrder != order) {
        delete finalOrder;
    }
    finalOrder = order;
}

FinalOrder* ScreenManager::GetFinalOrder() const {
    return finalOrder;
}

int ScreenManager::GetDaysCounter() const {
    return daysCounter;
}

// Asset getters
Texture2D ScreenManager::GetPlantTexture(const std::string& plantName) const {
    auto it = plantTextures.find(plantName);
    if (it != plantTextures.end()) {
        return it->second;
    }
    // Return empty texture if not found
    return Texture2D{0};
}

Texture2D ScreenManager::GetPotTexture(const std::string& color) const {
    auto it = potTextures.find(color);
    if (it != potTextures.end()) {
        return it->second;
    }
    return Texture2D{0};
}

Texture2D ScreenManager::GetRibbonTexture() const {
    return ribbonTexture;
}

Texture2D ScreenManager::GetMoneyIcon() const {
    return moneyIcon;
}

Texture2D ScreenManager::GetCartIcon() const {
    return cartIcon;
}

// Customer management
void ScreenManager::CreateNewCustomer(double budget) {
    if (customer != nullptr) {
        delete customer;
    }
    
    // Create regular customer with specified budget
    customer = new RegularCustomer();
    customer->setBudget(budget);
    customer->setMediator(mediator);
    mediator->registerColleague(customer);
    
    std::cout << "[ScreenManager] Created new customer with budget R" << budget << std::endl;
}

void ScreenManager::DeleteCustomer() {
    if (customer != nullptr) {
        std::cout << "[ScreenManager] Deleting customer, returning cart items to sales floor..." << std::endl;

        // Return all cart items to sales floor before deleting customer
        int cartSize = customer->getCartSize();
        for (int i = cartSize - 1; i >= 0; i--) {
            bool returned = customer->returnPlantToSalesFloor(i);
            if (returned) {
                std::cout << "[ScreenManager] Returned cart item " << i << " to sales floor" << std::endl;
            } else {
                std::cout << "[ScreenManager] Warning: Failed to return cart item " << i << std::endl;
            }
        }

        // Clean up customer's current order if it exists (prevents memory leak)
        // Note: The order will be deleted by the customer destructor, but we should
        // clear it to avoid potential issues

        mediator->removeColleague(customer);
        delete customer;
        customer = nullptr;
        std::cout << "[ScreenManager] Customer deleted successfully" << std::endl;
    }
}

// Previous order management (Prototype pattern)
FinalOrder* ScreenManager::GetPreviousOrder() const {
    return previousOrder;
}

bool ScreenManager::HasShownReorderNotification() const {
    return hasShownReorderNotification;
}

void ScreenManager::SetHasShownReorderNotification(bool shown) {
    hasShownReorderNotification = shown;
}

// Color scheme management
bool ScreenManager::IsAlternativeColors() const {
    return useAlternativeColors;
}

void ScreenManager::ToggleColorScheme() {
    useAlternativeColors = !useAlternativeColors;
    std::cout << "[ScreenManager] Color scheme toggled to: "
              << (useAlternativeColors ? "Dark Mode" : "Light Mode (Sage Green)")
              << std::endl;
}