#include "raylib.h"
#include <iostream>

// Include your actual classes
#include "include/Greenhouse.h"
#include "include/SalesFloor.h"
#include "include/NurseryMediator.h"
#include "include/Plant.h"
#include "include/RoseFactory.h"
#include "include/DaisyFactory.h"
#include "include/CactusFactory.h"
#include "include/AloeFactory.h"
#include "include/PotatoFactory.h"
#include "include/StrelitziaFactory.h"
#include "include/CareScheduler.h"

// Forward declarations for screen functions
void InitSalesFloorScreen(SalesFloor* salesFloor);
void UpdateSalesFloorScreen(SalesFloor* salesFloor);
void DrawSalesFloorScreen(SalesFloor* salesFloor);
void UnloadSalesFloorScreen();

void InitGreenhouseScreen(Greenhouse* greenhouse);
void UpdateGreenhouseScreen(Greenhouse* greenhouse);
void DrawGreenhouseScreen(Greenhouse* greenhouse);
void UnloadGreenhouseScreen();

// Screen enum
enum GameScreen {
    SALES_FLOOR = 0,
    GREENHOUSE = 1
};

// Global screen state
GameScreen currentScreen = SALES_FLOOR;
GameScreen nextScreen = SALES_FLOOR;
bool transitionToScreen = false;

// Global nursery objects
NurseryMediator* mediator = nullptr;
Greenhouse* greenhouse = nullptr;
SalesFloor* salesFloor = nullptr;
CareScheduler* scheduler = nullptr;

int main() {
    // Initialize window
    const int screenWidth = 1200;
    const int screenHeight = 800;
    
    InitWindow(screenWidth, screenHeight, "Plant Nursery Management System");
    SetTargetFPS(60);
    
    std::cout << "=== Plant Nursery GUI Started ===" << std::endl;
    
    // Initialize nursery system
    mediator = new NurseryMediator();
    scheduler = new CareScheduler();
    greenhouse = new Greenhouse(mediator, 6, 6);
    salesFloor = new SalesFloor(mediator, 5, 5);
    
    mediator->registerColleague(greenhouse);
    mediator->registerColleague(salesFloor);
    
    std::cout << "Creating plants for greenhouse..." << std::endl;
    
    // Create some plants using factories
    RoseFactory roseFactory;
    DaisyFactory daisyFactory;
    CactusFactory cactusFactory;
    AloeFactory aloeFactory;
    PotatoFactory potatoFactory;
    StrelitziaFactory strelitziaFactory;
    
    // Add plants to greenhouse
    Plant* rose1 = roseFactory.buildPlant(scheduler);
    rose1->setPrice(25.0);
    greenhouse->addPlant(rose1, 0, 1);
    
    Plant* daisy1 = daisyFactory.buildPlant(scheduler);
    daisy1->setPrice(15.0);
    greenhouse->addPlant(daisy1, 1, 0);
    
    Plant* cactus1 = cactusFactory.buildPlant(scheduler);
    cactus1->setPrice(20.0);
    greenhouse->addPlant(cactus1, 1, 3);
    
    Plant* aloe1 = aloeFactory.buildPlant(scheduler);
    aloe1->setPrice(18.0);
    greenhouse->addPlant(aloe1, 2, 2);
    
    Plant* potato1 = potatoFactory.buildPlant(scheduler);
    potato1->setPrice(12.0);
    greenhouse->addPlant(potato1, 3, 4);
    
    Plant* strelitzia1 = strelitziaFactory.buildPlant(scheduler);
    strelitzia1->setPrice(30.0);
    greenhouse->addPlant(strelitzia1, 4, 1);
    
    // Age some plants to make them mature/ready for sale
    std::cout << "Aging plants to maturity..." << std::endl;
    for (int i = 0; i < 40; i++) {
        rose1->dailyUpdate();
        daisy1->dailyUpdate();
        cactus1->dailyUpdate();
    }
    
    // Add some plants directly to sales floor
    Plant* rose2 = roseFactory.buildPlant(scheduler);
    rose2->setPrice(25.0);
    for (int i = 0; i < 40; i++) rose2->dailyUpdate();
    salesFloor->addPlantToDisplay(rose2, 0, 0);
    
    Plant* cactus2 = cactusFactory.buildPlant(scheduler);
    cactus2->setPrice(20.0);
    for (int i = 0; i < 40; i++) cactus2->dailyUpdate();
    salesFloor->addPlantToDisplay(cactus2, 0, 2);
    
    Plant* daisy2 = daisyFactory.buildPlant(scheduler);
    daisy2->setPrice(15.0);
    for (int i = 0; i < 40; i++) daisy2->dailyUpdate();
    salesFloor->addPlantToDisplay(daisy2, 1, 1);
    
    Plant* aloe2 = aloeFactory.buildPlant(scheduler);
    aloe2->setPrice(18.0);
    for (int i = 0; i < 40; i++) aloe2->dailyUpdate();
    salesFloor->addPlantToDisplay(aloe2, 2, 3);
    
    std::cout << "Greenhouse plants: " << greenhouse->getNumberOfPlants() << std::endl;
    std::cout << "Sales floor plants: " << salesFloor->getNumberOfPlants() << std::endl;
    std::cout << "Press TAB to switch between Sales Floor and Greenhouse" << std::endl;
    
    // Initialize first screen
    InitSalesFloorScreen(salesFloor);
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Handle screen transitions
        if (transitionToScreen) {
            // Unload current screen
            switch (currentScreen) {
                case SALES_FLOOR:
                    UnloadSalesFloorScreen();
                    break;
                case GREENHOUSE:
                    UnloadGreenhouseScreen();
                    break;
            }
            
            // Load next screen
            currentScreen = nextScreen;
            switch (currentScreen) {
                case SALES_FLOOR:
                    InitSalesFloorScreen(salesFloor);
                    break;
                case GREENHOUSE:
                    InitGreenhouseScreen(greenhouse);
                    break;
            }
            
            transitionToScreen = false;
        }
        
        // Update current screen
        switch (currentScreen) {
            case SALES_FLOOR:
                UpdateSalesFloorScreen(salesFloor);
                break;
            case GREENHOUSE:
                UpdateGreenhouseScreen(greenhouse);
                break;
        }
        
        // Draw current screen
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        switch (currentScreen) {
            case SALES_FLOOR:
                DrawSalesFloorScreen(salesFloor);
                break;
            case GREENHOUSE:
                DrawGreenhouseScreen(greenhouse);
                break;
        }
        
        // Draw screen indicator
        DrawText(currentScreen == SALES_FLOOR ? "SALES FLOOR" : "GREENHOUSE", 
                 10, 10, 20, DARKGRAY);
        DrawText("Press TAB to switch screens", 10, 40, 16, GRAY);
        
        EndDrawing();
    }
    
    // Unload current screen before closing
    switch (currentScreen) {
        case SALES_FLOOR:
            UnloadSalesFloorScreen();
            break;
        case GREENHOUSE:
            UnloadGreenhouseScreen();
            break;
    }
    
    // Cleanup
    std::cout << "Cleaning up nursery system..." << std::endl;
    
    // Delete all plants from greenhouse
    std::vector<Plant*> ghPlants = greenhouse->getAllPlants();
    for (Plant* p : ghPlants) {
        greenhouse->removePlant(p);
        delete p;
    }
    
    // Delete all plants from sales floor
    std::vector<Plant*> sfPlants = salesFloor->getDisplayPlants();
    for (Plant* p : sfPlants) {
        salesFloor->removePlantFromDisplay(p);
        delete p;
    }
    
    delete greenhouse;
    delete salesFloor;
    delete scheduler;
    delete mediator;
    
    CloseWindow();
    
    std::cout << "=== Plant Nursery GUI Closed ===" << std::endl;
    
    return 0;
}

// Helper function to request screen change
void TransitionToScreen(int screen) {
    transitionToScreen = true;
    nextScreen = static_cast<GameScreen>(screen);
}