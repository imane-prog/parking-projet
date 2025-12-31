#ifndef GAME_H
#define GAME_H

#include <vector>
#include <map>
#include <string>
#include "raylib.h"
#include "Structs.hpp" 

class Game {
private:
    Music backgroundMusic;
public:
    Game();
    ~Game(); 

    void Init(); 
    bool ShouldClose(); 
    void Update(); 
    void Draw(); 
    
    void LoadAllTextures();
    void UnloadAllTextures();

private:
    GameScreen currentScreen; 
    bool soundOn;
    int currentCarIndex; 
    bool gameStarted;
    
    std::vector<Car> cars;
    std::vector<ParkingZone> zones;
    std::vector<ChargingSpot> chargingSpots;
    
    std::map<std::string, Texture2D> loadedTextures;

    Rectangle playButton;
    Rectangle restartButton;
    Rectangle exitButton;
    Rectangle volumeButton;
    Rectangle backButton;
    Rectangle pauseButton;
    Rectangle tabControlRec;
    Rectangle upButton;
    Rectangle downButton;
    Rectangle leftButton_Move;
    Rectangle rightButton_Move;
    Rectangle turnLeftButton;
    Rectangle turnRightButton;

    Car createCar(Vector2 pos, float rot, float energy, Color tint, FuelType type);
    int getZoneIndex(int spotIndex);
    void RestartGame();
    
    void DrawTextLeftAligned(const char* text, int startX, int startY, int fontSize, Color color);
    void DrawInfoTile(int x, int y, int width, int height, Color bgColor, const char* label, const char* statsLine, const char* costUnit);
};

#endif 