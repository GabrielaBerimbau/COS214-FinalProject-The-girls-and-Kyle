#include "raylib.h"
#include <vector>
#include <string>
#include <map>
#include "include/SalesFloor.h"
#include "include/Plant.h"

// External function to request screen transitions
extern void TransitionToScreen(int screen);

// Screen state
static const int CELL_SIZE = 100;
static int gridStartX;
static int gridStartY;
static int hoveredRow = -1;
static int hoveredCol = -1;
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

void InitSalesFloorScreen(SalesFloor* salesFloor) {
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
    
    // Calculate grid position (centered)
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int gridWidth = CELL_SIZE * salesFloor->getColumns();
    int gridHeight = CELL_SIZE * salesFloor->getRows();
    gridStartX = (screenWidth - gridWidth) / 2;
    gridStartY = (screenHeight - gridHeight) / 2 + 50;
    
    selectedPlant = nullptr;
}

void UpdateSalesFloorScreen(SalesFloor* salesFloor) {
    // Check for screen switch
    if (IsKeyPressed(KEY_TAB)) {
        TransitionToScreen(1); // Switch to greenhouse
    }
    
    // Handle mouse hover
    Vector2 mousePos = GetMousePosition();
    hoveredRow = -1;
    hoveredCol = -1;
    
    int rows = salesFloor->getRows();
    int cols = salesFloor->getColumns();
    
    if (mousePos.x >= gridStartX && mousePos.x < gridStartX + CELL_SIZE * cols &&
        mousePos.y >= gridStartY && mousePos.y < gridStartY + CELL_SIZE * rows) {
        hoveredCol = (mousePos.x - gridStartX) / CELL_SIZE;
        hoveredRow = (mousePos.y - gridStartY) / CELL_SIZE;
    }
    
    // Handle clicks
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hoveredRow >= 0) {
        selectedPlant = salesFloor->getPlantAt(hoveredRow, hoveredCol);
        if (selectedPlant != nullptr) {
            TraceLog(LOG_INFO, "Selected: %s - R%.2f - State: %s", 
                    selectedPlant->getName().c_str(), 
                    selectedPlant->getPrice(),
                    selectedPlant->getState()->getStateName().c_str());
        }
    }
}

void DrawSalesFloorScreen(SalesFloor* salesFloor) {
    int rows = salesFloor->getRows();
    int cols = salesFloor->getColumns();
    
    // Draw title
    const char* title = "SALES FLOOR - Customer Shopping Area";
    int titleWidth = MeasureText(title, 30);
    DrawText(title, (GetScreenWidth() - titleWidth) / 2, 80, 30, DARKBLUE);
    
    // Draw grid
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int x = gridStartX + col * CELL_SIZE;
            int y = gridStartY + row * CELL_SIZE;
            
            Color cellColor = LIGHTGRAY;
            
            // Highlight hovered cell
            if (row == hoveredRow && col == hoveredCol) {
                cellColor = SKYBLUE;
            }
            
            // Highlight selected cell
            Plant* plantAtPos = salesFloor->getPlantAt(row, col);
            if (plantAtPos == selectedPlant && selectedPlant != nullptr) {
                cellColor = GOLD;
            }
            
            DrawRectangle(x, y, CELL_SIZE - 4, CELL_SIZE - 4, cellColor);
            DrawRectangleLines(x, y, CELL_SIZE - 4, CELL_SIZE - 4, DARKGRAY);
            
            // Draw plant if present
            if (plantAtPos != nullptr) {
                std::string plantName = plantAtPos->getName();
                
                // Try to find texture for this plant
                if (plantTextures.find(plantName) != plantTextures.end()) {
                    Texture2D texture = plantTextures[plantName];
                    
                    // Scale texture to fit cell (leave some padding)
                    float scale = (CELL_SIZE - 40) / (float)texture.height;
                    int texWidth = texture.width * scale;
                    int texHeight = texture.height * scale;
                    int texX = x + (CELL_SIZE - texWidth) / 2;
                    int texY = y + (CELL_SIZE - texHeight) / 2 - 5;
                    
                    DrawTextureEx(texture, (Vector2){(float)texX, (float)texY}, 0.0f, scale, WHITE);
                } else {
                    // Fallback to colored circle if no texture
                    Color plantColor = GetPlantColor(plantName);
                    DrawCircle(x + CELL_SIZE / 2, y + CELL_SIZE / 2 - 10, 25, plantColor);
                }
                
                // Draw price tag
                char priceText[32];
                snprintf(priceText, sizeof(priceText), "R%.0f", plantAtPos->getPrice());
                int priceWidth = MeasureText(priceText, 14);
                
                // Draw price background
                DrawRectangle(x + (CELL_SIZE - priceWidth) / 2 - 4, y + CELL_SIZE - 28, 
                             priceWidth + 8, 20, Fade(DARKGREEN, 0.8f));
                DrawText(priceText, x + (CELL_SIZE - priceWidth) / 2, y + CELL_SIZE - 26, 14, GOLD);
            }
        }
    }
    
    // Draw info panel
    DrawRectangle(20, GetScreenHeight() - 220, 350, 200, Fade(DARKBLUE, 0.8f));
    DrawText("SALES FLOOR INFO:", 30, GetScreenHeight() - 210, 16, WHITE);
    
    char plantCount[64];
    snprintf(plantCount, sizeof(plantCount), "Plants on display: %d", salesFloor->getNumberOfPlants());
    DrawText(plantCount, 30, GetScreenHeight() - 185, 14, LIGHTGRAY);
    
    char capacity[64];
    snprintf(capacity, sizeof(capacity), "Capacity: %d/%d", 
             salesFloor->getNumberOfPlants(), salesFloor->getCapacity());
    DrawText(capacity, 30, GetScreenHeight() - 165, 14, LIGHTGRAY);
    
    DrawText("Click plants to view details", 30, GetScreenHeight() - 140, 14, LIGHTGRAY);
    
    // Show selected plant details
    if (selectedPlant != nullptr) {
        DrawText("SELECTED:", 30, GetScreenHeight() - 115, 14, YELLOW);
        DrawText(selectedPlant->getName().c_str(), 30, GetScreenHeight() - 95, 13, WHITE);
        
        char details[128];
        snprintf(details, sizeof(details), "ID: %s", selectedPlant->getID().c_str());
        DrawText(details, 30, GetScreenHeight() - 78, 11, LIGHTGRAY);
        
        snprintf(details, sizeof(details), "State: %s", selectedPlant->getState()->getStateName().c_str());
        DrawText(details, 30, GetScreenHeight() - 63, 11, LIGHTGRAY);
        
        snprintf(details, sizeof(details), "Health: %d%%", selectedPlant->getHealthLevel());
        DrawText(details, 30, GetScreenHeight() - 48, 11, LIGHTGRAY);
    }
    
    DrawText("Press TAB for Greenhouse", 30, GetScreenHeight() - 28, 12, YELLOW);
}

void UnloadSalesFloorScreen() {
    selectedPlant = nullptr;
    
    // Unload textures when screen is unloaded for the last time
    // (In a real app, you might want to keep them loaded)
    if (texturesLoaded) {
        for (auto& pair : plantTextures) {
            UnloadTexture(pair.second);
        }
        plantTextures.clear();
        texturesLoaded = false;
    }
}