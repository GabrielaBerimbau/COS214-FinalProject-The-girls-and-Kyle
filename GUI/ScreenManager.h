#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "raylib.h"
#include <map>
#include <string>
#include <vector>

// Forward declarations for backend classes
class NurseryCoordinator;
class Customer;
class SalesFloor;
class Greenhouse;
class CareScheduler;
class SalesAssistant;
class FloorManager;
class NurseryOwner;
class Plant;
class ConcreteOrder;
class FinalOrder;
class PlantFactory;

// Screen enumeration
enum class GameScreen {
    START,
    SALES_FLOOR,
    CUSTOMER_GREENHOUSE,
    CART_VIEW,
    DECORATION,
    ORDER_CREATION,
    ORDER_CONFIRMATION,
    CHECKOUT,
    RECEIPT,
    STAFF_GREENHOUSE
};

class ScreenManager {
private:
    // Current screen state
    GameScreen currentScreen;
    GameScreen previousScreen;
    
    // Core backend objects (persist across screens)
    NurseryCoordinator* mediator;
    Customer* customer;
    SalesFloor* salesFloor;
    Greenhouse* greenhouse;
    CareScheduler* careScheduler;
    
    // Staff members (chain of responsibility)
    SalesAssistant* salesAssistant;
    FloorManager* floorManager;
    NurseryOwner* nurseryOwner;
    
    // Temporary state variables
    Plant* currentPlantBeingDecorated;
    int currentCartIndex;
    FinalOrder* finalOrder;
    
    // Time tracking for real-time updates
    float lastUpdateTime;
    int daysCounter;
    
    // Asset management
    std::map<std::string, Texture2D> plantTextures;
    std::map<std::string, Texture2D> potTextures;
    Texture2D ribbonTexture;
    Texture2D moneyIcon;
    Texture2D cartIcon;
    
    // Helper methods
    void PopulateInitialGreenhouse();
    void PopulateInitialSalesFloor();
    void LoadAssets();
    void UnloadAssets();

    bool isCleanedUp = false;
    
    
public:
    // Constructor/Destructor
    ScreenManager();
    ~ScreenManager();
    
    // Main loop functions
    void Initialize();
    void Update();
    void Draw();
    void Cleanup();
    
    // Screen management
    void SwitchScreen(GameScreen newScreen);
    GameScreen GetCurrentScreen() const;
    GameScreen GetPreviousScreen() const;
    
    // State getters (for screens to access)
    Customer* GetCustomer() const;
    SalesFloor* GetSalesFloor() const;
    Greenhouse* GetGreenhouse() const;
    CareScheduler* GetCareScheduler() const;
    NurseryCoordinator* GetMediator() const;
    
    // Staff getters
    SalesAssistant* GetSalesAssistant() const;
    FloorManager* GetFloorManager() const;
    NurseryOwner* GetNurseryOwner() const;
    
    // Temporary state management
    void SetCurrentPlantBeingDecorated(Plant* plant);
    Plant* GetCurrentPlantBeingDecorated() const;
    
    void SetCurrentCartIndex(int index);
    int GetCurrentCartIndex() const;
    
    void SetFinalOrder(FinalOrder* order);
    FinalOrder* GetFinalOrder() const;
    
    // Days counter
    int GetDaysCounter() const;
    
    // Asset getters
    Texture2D GetPlantTexture(const std::string& plantName) const;
    Texture2D GetPotTexture(const std::string& color) const;
    Texture2D GetRibbonTexture() const;
    Texture2D GetMoneyIcon() const;
    Texture2D GetCartIcon() const;
    
    // Customer creation (for start screen)
    void CreateNewCustomer(double budget);
    void DeleteCustomer();

    void PerformDailyUpdate();
};

#endif // SCREEN_MANAGER_H