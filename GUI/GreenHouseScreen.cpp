#include "raylib.h"
#include <vector>
#include <string>
#include <map>
#include "include/Greenhouse.h"
#include "include/Plant.h"

// External function to request screen transitions
extern void TransitionToScreen(int screen);

// Screen state
static const int CELL_SIZE = 90;
static int gridStartX;
static int gridStartY;
static int hoveredRow = -1;
static int hoveredCol = -1;
static int selectedRow = -1;
static int selectedCol = -1;
static Plant* selectedPlant = nullptr;

// Textures
static std::map<std::string, Texture2D> plantTextures;
static bool texturesLoaded = false;

static Color GetPlantColor(const std::string& plantName) {
    if (plantName == "Rose") return RED;
    if (plantName == "Daisy") return YELLOW;
    if (plantName == "Cactus") return GREEN;
    if (plantName == "Aloe") return DARKGREEN;
    if (plantName == "Potato") return BROWN;
    if (plantName == "Radish") return PINK;
    if (plantName == "Strelitzia") return ORANGE;
    if (plantName == "Monstera") return DARKGREEN;
    if (plantName == "Venus Fly Trap") return PURPLE;
    return GRAY;
}

void InitGreenhouseScreen(Greenhouse* greenhouse) {
    // Load textures if not already loaded
    if (!texturesLoaded) {
        plantTextures["Cactus"] = LoadTexture("assets/cactus2.png");
        plantTextures["Potato"] = LoadTexture("assets/carrot.png"); // Using carrot for potato
        plantTextures["Rose"] = LoadTexture("assets/rose.png");
        plantTextures["Daisy"] = LoadTexture("assets/flower.png");
        plantTextures["Strelitzia"] = LoadTexture("assets/strelitzia.png");
        plantTextures["Radish"] = LoadTexture("assets/radish.png");
        plantTextures["Aloe"] = LoadTexture("assets/cactus.png");
        plantTextures["Venus Fly Trap"] = LoadTexture("assets/VenusFlyTrap.png");
        plantTextures["Monstera"] = LoadTexture("assets/plant.png");
        plantTextures["Tulip"] = LoadTexture("assets/tulip.png");
        
        // Set texture filter for better quality
        for (auto& pair : plantTextures) {
            SetTextureFilter(pair.second, TEXTURE_FILTER_BILINEAR);
        }
        
        texturesLoaded = true;
    }
    
    // Calculate grid position
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int gridWidth = CELL_SIZE * greenhouse->getColumns();
    int gridHeight = CELL_SIZE * greenhouse->getRows();
    gridStartX = (screenWidth - gridWidth) / 2;
    gridStartY = (screenHeight - gridHeight) / 2 + 30;
    
    selectedRow = -1;
    selectedCol = -1;
    selectedPlant = nullptr;
}

void UpdateGreenhouseScreen(Greenhouse* greenhouse) {
    // Check for screen switch
    if (IsKeyPressed(KEY_TAB)) {
        TransitionToScreen(0); // Switch to sales floor
    }
    
    // Handle mouse hover
    Vector2 mousePos = GetMousePosition();
    hoveredRow = -1;
    hoveredCol = -1;
    
    int rows = greenhouse->getRows();
    int cols = greenhouse->getColumns();
    
    if (mousePos.x >= gridStartX && mousePos.x < gridStartX + CELL_SIZE * cols &&
        mousePos.y >= gridStartY && mousePos.y < gridStartY + CELL_SIZE * rows) {
        hoveredCol = (mousePos.x - gridStartX) / CELL_SIZE;
        hoveredRow = (mousePos.y - gridStartY) / CELL_SIZE;
    }
    
    // Handle clicks
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hoveredRow >= 0) {
        selectedRow = hoveredRow;
        selectedCol = hoveredCol;
        selectedPlant = greenhouse->getPlantAt(hoveredRow, hoveredCol);
        
        if (selectedPlant != nullptr) {
            TraceLog(LOG_INFO, "Selected: %s - Age: %d days, Health: %d%%, State: %s", 
                    selectedPlant->getName().c_str(), 
                    selectedPlant->getAge(),
                    selectedPlant->getHealthLevel(),
                    selectedPlant->getState()->getStateName().c_str());
        }
    }
}

void DrawGreenhouseScreen(Greenhouse* greenhouse) {
    int rows = greenhouse->getRows();
    int cols = greenhouse->getColumns();
    
    // Draw title
    const char* title = "GREENHOUSE - Plant Growing Area";
    int titleWidth = MeasureText(title, 30);
    DrawText(title, (GetScreenWidth() - titleWidth) / 2, 70, 30, DARKGREEN);
    
    // Draw grid
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int x = gridStartX + col * CELL_SIZE;
            int y = gridStartY + row * CELL_SIZE;
            
            Color cellColor = Fade(GREEN, 0.2f);
            
            // Highlight hovered cell
            if (row == hoveredRow && col == hoveredCol) {
                cellColor = Fade(SKYBLUE, 0.5f);
            }
            
            // Highlight selected cell
            if (row == selectedRow && col == selectedCol) {
                cellColor = Fade(YELLOW, 0.4f);
            }
            
            DrawRectangle(x, y, CELL_SIZE - 4, CELL_SIZE - 4, cellColor);
            DrawRectangleLines(x, y, CELL_SIZE - 4, CELL_SIZE - 4, DARKGREEN);
            
            // Draw plant if present
            Plant* plant = greenhouse->getPlantAt(row, col);
            if (plant != nullptr) {
                std::string plantName = plant->getName();
                std::string state = plant->getState()->getStateName();
                
                // Try to find texture for this plant
                if (plantTextures.find(plantName) != plantTextures.end()) {
                    Texture2D texture = plantTextures[plantName];
                    
                    // Scale based on plant state
                    float baseScale = (CELL_SIZE - 30) / (float)texture.height;
                    float stateMultiplier = 0.5f; // Seedling
                    if (state == "Growing") stateMultiplier = 0.7f;
                    if (state == "Mature" || state == "Flowering") stateMultiplier = 1.0f;
                    
                    float scale = baseScale * stateMultiplier;
                    int texWidth = texture.width * scale;
                    int texHeight = texture.height * scale;
                    int texX = x + (CELL_SIZE - texWidth) / 2;
                    int texY = y + (CELL_SIZE - texHeight) / 2;
                    
                    DrawTextureEx(texture, (Vector2){(float)texX, (float)texY}, 0.0f, scale, WHITE);
                } else {
                    // Fallback to colored circle if no texture
                    Color plantColor = GetPlantColor(plantName);
                    int size = 15;
                    if (state == "Growing") size = 20;
                    if (state == "Mature" || state == "Flowering") size = 25;
                    DrawCircle(x + CELL_SIZE / 2, y + CELL_SIZE / 2 - 5, size, plantColor);
                }
                
                // Draw state badge
                const char* stateName = state.c_str();
                int stateWidth = MeasureText(stateName, 8);
                DrawRectangle(x + (CELL_SIZE - stateWidth) / 2 - 3, y + CELL_SIZE - 18, 
                             stateWidth + 6, 14, Fade(BLACK, 0.7f));
                DrawText(stateName, x + (CELL_SIZE - stateWidth) / 2, y + CELL_SIZE - 16, 8, WHITE);
            }
        }
    }
    
    // Draw info panel on the left
    DrawRectangle(20, 150, 300, 280, Fade(DARKGREEN, 0.9f));
    DrawText("GREENHOUSE INFO:", 30, 160, 16, WHITE);
    
    char plantCount[64];
    snprintf(plantCount, sizeof(plantCount), "Growing plants: %d", greenhouse->getNumberOfPlants());
    DrawText(plantCount, 30, 190, 14, LIGHTGRAY);
    
    char capacity[64];
    snprintf(capacity, sizeof(capacity), "Capacity: %d/%d", 
             greenhouse->getNumberOfPlants(), greenhouse->getCapacity());
    DrawText(capacity, 30, 210, 14, LIGHTGRAY);
    
    // Show selected plant details
    if (selectedPlant != nullptr) {
        DrawText("SELECTED:", 30, 240, 14, YELLOW);
        DrawText(selectedPlant->getName().c_str(), 30, 260, 13, WHITE);
        
        char details[128];
        snprintf(details, sizeof(details), "ID: %s", selectedPlant->getID().c_str());
        DrawText(details, 30, 278, 11, LIGHTGRAY);
        
        snprintf(details, sizeof(details), "State: %s", selectedPlant->getState()->getStateName().c_str());
        DrawText(details, 30, 293, 11, LIGHTGRAY);
        
        snprintf(details, sizeof(details), "Age: %d days", selectedPlant->getAge());
        DrawText(details, 30, 308, 11, LIGHTGRAY);
        
        snprintf(details, sizeof(details), "Health: %d%%", selectedPlant->getHealthLevel());
        DrawText(details, 30, 323, 11, LIGHTGRAY);
        
        snprintf(details, sizeof(details), "Water: %d%%", selectedPlant->getWaterLevel());
        DrawText(details, 30, 338, 11, LIGHTGRAY);
        
        snprintf(details, sizeof(details), "Nutrients: %d%%", selectedPlant->getNutrientLevel());
        DrawText(details, 30, 353, 11, LIGHTGRAY);
        
        snprintf(details, sizeof(details), "Ready: %s", selectedPlant->isReadyForSale() ? "Yes" : "No");
        DrawText(details, 30, 368, 11, selectedPlant->isReadyForSale() ? GREEN : RED);
        
        char price[64];
        snprintf(price, sizeof(price), "Price: R%.2f", selectedPlant->getPrice());
        DrawText(price, 30, 388, 12, GOLD);
    } else if (selectedRow >= 0) {
        DrawText("SELECTED:", 30, 240, 14, YELLOW);
        DrawText("Empty spot", 30, 260, 12, GRAY);
    } else {
        DrawText("Click to select", 30, 240, 12, GRAY);
    }
    
    DrawText("Press TAB for Sales Floor", 30, 410, 12, YELLOW);
}

void UnloadGreenhouseScreen() {
    selectedRow = -1;
    selectedCol = -1;
    selectedPlant = nullptr;
    
    // Unload textures
    if (texturesLoaded) {
        for (auto& pair : plantTextures) {
            UnloadTexture(pair.second);
        }
        plantTextures.clear();
        texturesLoaded = false;
    }
}