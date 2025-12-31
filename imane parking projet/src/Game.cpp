#include "Game.hpp"
#include "Structs.hpp" 
#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream> 

bool operator==(const Color& a, const Color& b) {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

void Game::LoadAllTextures() {
    loadedTextures["menu_bg"] = LoadTexture("images/background.png"); 
    loadedTextures["speaker_on"] = LoadTexture("images/speaker_on.png");
    loadedTextures["speaker_off"] = LoadTexture("images/speaker_off.png");
    loadedTextures["back"] = LoadTexture("images/back.png");
    loadedTextures["pause"] = LoadTexture("images/pause.png");
    loadedTextures["pause_bg"] = LoadTexture("images/pause_bg.png");
    loadedTextures["parking"] = LoadTexture("images/parking.png"); 
    loadedTextures["car"] = LoadTexture("images/car.png"); 
    
    loadedTextures["restart"] = LoadTexture("images/restart.png"); 
    
    if (loadedTextures["car"].id == 0) std::cerr << "Erreur: car.png non trouvé." << std::endl;

    // --- CHARGEMENT DE LA MUSIQUE ---
    // Assurez-vous d'avoir un fichier background.mp3 dans un dossier "audio/" ou à la racine
    backgroundMusic = LoadMusicStream("audio/background.mp3");
    PlayMusicStream(backgroundMusic);
    SetMusicVolume(backgroundMusic, 1.0f);
}

void Game::UnloadAllTextures() {
    // --- DÉCHARGEMENT DE LA MUSIQUE ---
    StopMusicStream(backgroundMusic);
    UnloadMusicStream(backgroundMusic);
    
    for (auto const& [key, val] : loadedTextures) UnloadTexture(val);
}

Car Game::createCar(Vector2 pos, float rot, float energy, Color tint, FuelType type) {
    Texture2D carTex = loadedTextures.count("car") ? loadedTextures.at("car") : LoadTexture("car.png");
    return {carTex, pos, 0.0f, rot, false, energy, type, false, tint, tint};
}

int Game::getZoneIndex(int spotIndex) {
    if (spotIndex >= 1 && spotIndex <= 3) return EV_ZONE_INDEX_1; 
    if (spotIndex >= 4 && spotIndex <= 6) return EV_ZONE_INDEX_2;
    return -1;
}

void Game::DrawTextLeftAligned(const char* text, int startX, int startY, int fontSize, Color color) {
    const char* start = text;
    const char* current = text;
    int lineY = startY;
    int lineHeight = fontSize;

    while (*current) {
        if (*current == '\n' || *(current + 1) == '\0') {
            char line[256];
            int len = current - start;
            if (*current == '\n') len = current - start; 
            else len = (int)strlen(start);

            strncpy(line, start, len);
            line[len] = '\0';
            
            DrawText(line, startX, lineY, fontSize, color);
            lineY += lineHeight + 5; 

            if (*current == '\n') start = current + 1;
        }
        current++;
    }
}

void Game::DrawInfoTile(int x, int y, int width, int height, Color bgColor, const char* label, const char* statsLine, const char* costUnit) {
    DrawRectangle(x, y, width, height, Fade(bgColor, 0.7f)); 
    DrawRectangle(x + 5, y + 5, 30, 30, WHITE);
    DrawText(label, x + 40, y + 5, 12, WHITE);
    DrawText(statsLine, x + 40, y + 20, 12, WHITE);
    DrawText(costUnit, x + 5, y + 40, 10, WHITE);
}

void Game::RestartGame() {
    currentScreen = GAMEPLAY; 
    gameStarted = true;
    cars.clear();
    
    cars.push_back(createCar({50, 500}, 0.0f, 50.0f, LIME, ELECTRIC)); 
    cars.push_back(createCar({100, 500}, 0.0f, 25.0f, ORANGE, GASOLINE)); 
    cars.push_back(createCar({150, 500}, 0.0f, 95.0f, SKYBLUE, ELECTRIC)); 
    cars.push_back(createCar({200, 500}, 0.0f, 5.0f, RED, GASOLINE)); 
    
    currentCarIndex = 0;
    cars[currentCarIndex].tint = YELLOW;
    
    for (auto& spot : chargingSpots) { spot.occupied = false; spot.occupiedByCarIndex = -1; }
    for (auto& zone : zones) { zone.occupiedSlots = 0; }
}

Game::Game() :
    currentScreen(MENU), 
    soundOn(true),
    currentCarIndex(0), 
    gameStarted(false)
{
    playButton = {screenWidth/2.0f - 75, screenHeight/2.0f + 100, 150, 50};
    exitButton = {screenWidth/2.0f - 75, screenHeight/2.0f + 160, 150, 50};
    
    // --- DIMENSIONS DES BOUTONS RÉDUITES ---
    const int HEADER_BTN_SIZE = 40; 
    const int HEADER_BTN_GAP = 5;
    
    volumeButton = {10, 10, HEADER_BTN_SIZE, HEADER_BTN_SIZE};
    backButton   = {10 + HEADER_BTN_SIZE + HEADER_BTN_GAP, 10, HEADER_BTN_SIZE, HEADER_BTN_SIZE};
    pauseButton  = {10 + (HEADER_BTN_SIZE + HEADER_BTN_GAP) * 2, 10, HEADER_BTN_SIZE, HEADER_BTN_SIZE};
    
    // Restart : Positionné juste en dessous du volume (10 + 40 + 5 = 55)
    restartButton = {10, 55, 80, 40};
    
    tabControlRec = {screenWidth - 350.0f, screenHeight - 20.0f, 340, 15}; 
    
    // --- DIMENSIONS DES CONTRÔLES RÉDUITES ---
    const int D_PAD_SIZE = 40; 
    const int D_PAD_X = screenWidth - (D_PAD_SIZE * 3) - 40; 
    const int D_PAD_Y = screenHeight - 170; 

    upButton      = {D_PAD_X + D_PAD_SIZE, D_PAD_Y, D_PAD_SIZE, D_PAD_SIZE};
    downButton    = {D_PAD_X + D_PAD_SIZE, D_PAD_Y + D_PAD_SIZE, D_PAD_SIZE, D_PAD_SIZE};
    leftButton_Move = {D_PAD_X, D_PAD_Y + D_PAD_SIZE, D_PAD_SIZE, D_PAD_SIZE};
    rightButton_Move = {D_PAD_X + D_PAD_SIZE * 2.0f, D_PAD_Y + D_PAD_SIZE, D_PAD_SIZE, D_PAD_SIZE};

    const int ROT_BUTTON_SIZE = 40; 
    const int ROT_BAR_X = screenWidth - (ROT_BUTTON_SIZE * 2) - 40; 
    const int ROT_BAR_Y = screenHeight - 220; 
    
    turnLeftButton  = {ROT_BAR_X, ROT_BAR_Y, ROT_BUTTON_SIZE, ROT_BUTTON_SIZE};
    turnRightButton = {ROT_BAR_X + ROT_BUTTON_SIZE + 10.0f, ROT_BAR_Y, ROT_BUTTON_SIZE, ROT_BUTTON_SIZE};
}

Game::~Game() {
    UnloadAllTextures();
}

void Game::Init() {
    InitAudioDevice(); // Important : Initialiser l'audio AVANT les autres Init
    
    InitWindow(screenWidth, screenHeight, "Smart Parking (EV STATION Only) - Final V6");
    SetTargetFPS(60);
    srand((unsigned int)time(0));
    LoadAllTextures(); 

    ParkingZone z1;
    z1.rect = {100, 80, 200, 200};
    z1.totalSlots = 3;
    z1.costPerHour = 1.50f;
    z1.color = BLUE;
    z1.label = "PARKING 1";
    zones.push_back(z1);

    Rectangle evZoneRect1 = {550, 80, 200, 200}; 
    ParkingZone z2;
    z2.rect = evZoneRect1;
    z2.totalSlots = 3;
    z2.costPerHour = 5.00f;
    z2.color = RED;
    z2.label = "PARKING 2 ";
    zones.push_back(z2);

    Rectangle evZoneRect2 = {280, 420, 300, 150}; 
    ParkingZone z3;
    z3.rect = evZoneRect2;
    z3.totalSlots = 3;
    z3.costPerHour = 0.45f;
    z3.color = GREEN;
    z3.label = "EV STATION";
    zones.push_back(z3);
    
    float spotWidth = 50;
    float spotHeight = 100;
    
    float startX2 = evZoneRect2.x + 10; float startY2 = evZoneRect2.y + 10; 
    
    ChargingSpot s1;
    s1.rect = {startX2, startY2, spotWidth, spotHeight};
    s1.occupied = false;
    s1.index = 4;
    s1.occupiedByCarIndex = -1;
    chargingSpots.push_back(s1);

    ChargingSpot s2;
    s2.rect = {startX2 + spotWidth + 10.0f, startY2, spotWidth, spotHeight};
    s2.occupied = false;
    s2.index = 5;
    s2.occupiedByCarIndex = -1;
    chargingSpots.push_back(s2);

    ChargingSpot s3;
    s3.rect = {startX2 + (spotWidth + 10.0f) * 2.0f, startY2, spotWidth, spotHeight};
    s3.occupied = false;
    s3.index = 6;
    s3.occupiedByCarIndex = -1;
    chargingSpots.push_back(s3);
    
    cars.push_back(createCar({50, 500}, 0.0f, 50.0f, LIME, ELECTRIC)); 
    cars.push_back(createCar({100, 500}, 0.0f, 25.0f, ORANGE, GASOLINE)); 
    cars.push_back(createCar({150, 500}, 0.0f, 95.0f, SKYBLUE, ELECTRIC)); 
    cars.push_back(createCar({200, 500}, 0.0f, 5.0f, RED, GASOLINE)); 
    cars[currentCarIndex].tint = YELLOW; 
}

bool Game::ShouldClose() {
    return WindowShouldClose();
}

void Game::Update() {
    // Mise à jour de la musique (obligatoire pour le streaming)
    UpdateMusicStream(backgroundMusic);
    
    // Relancer la musique si elle s'arrête (looping) et si le son est activé
    if (!IsMusicStreamPlaying(backgroundMusic) && soundOn) {
        PlayMusicStream(backgroundMusic);
    }

    if (currentScreen == GAMEPLAY && IsKeyPressed(KEY_TAB)) {
        cars[currentCarIndex].tint = cars[currentCarIndex].originalTint;
        currentCarIndex = (currentCarIndex + 1) % (int)cars.size();
        cars[currentCarIndex].tint = YELLOW; 
    }
    
    if (IsKeyPressed(KEY_R)) {
        RestartGame();
    }
    
    Vector2 mousePos = GetMousePosition(); 

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (currentScreen != MENU && CheckCollisionPointRec(mousePos, restartButton)) {
            RestartGame();
        }

        if (CheckCollisionPointRec(mousePos, volumeButton)) {
            soundOn = !soundOn;
            if (soundOn) {
                PlayMusicStream(backgroundMusic);
                SetMusicVolume(backgroundMusic, 1.0f);
            } else {
                SetMusicVolume(backgroundMusic, 0.0f);
            }
        }
        
        if (CheckCollisionPointRec(mousePos, backButton)) {
            bool spotReleased = false;
            for (auto& spot : chargingSpots) {
                if (spot.occupied && spot.occupiedByCarIndex == currentCarIndex) {
                    cars[currentCarIndex].tint = cars[currentCarIndex].originalTint;
                    spot.occupied = false;
                    spot.occupiedByCarIndex = -1; 
                    int zoneIndex = getZoneIndex(spot.index);
                    if (zoneIndex != -1) zones[zoneIndex].occupiedSlots--;
                    cars[currentCarIndex].isServicing = false; 
                    cars[currentCarIndex].parked = false;
                    currentScreen = GAMEPLAY; 
                    cars[currentCarIndex].tint = YELLOW;
                    spotReleased = true;
                    break;
                }
            }
            
            if (!spotReleased) {
                if (currentScreen == PAUSE || currentScreen == ENDING) {
                    currentScreen = GAMEPLAY;
                    cars[currentCarIndex].isServicing = false;
                    cars[currentCarIndex].parked = false;
                } else {
                    currentScreen = MENU; 
                    gameStarted = false;
                    for (auto& c : cars) c.tint = c.originalTint; 
                }
            }
        }
        
        if (currentScreen == GAMEPLAY && CheckCollisionPointRec(mousePos, pauseButton)) { currentScreen = PAUSE; } 
        else if (currentScreen == PAUSE && CheckCollisionPointRec(mousePos, pauseButton)) { currentScreen = GAMEPLAY; }
        
        if (currentScreen == MENU) {
            if (CheckCollisionPointRec(mousePos, playButton)) {
                currentScreen = GAMEPLAY; 
                gameStarted = true;
                cars[currentCarIndex].tint = YELLOW; 
            }
            if (CheckCollisionPointRec(mousePos, exitButton)) CloseWindow(); 
        }
    }
    
    if (currentScreen == GAMEPLAY || currentScreen == ENDING) { 
        Car& playerCar = cars[currentCarIndex];
        float moveSpeed = 2.0f; 
        float rotationSpeed = 3.5f;

        bool movingUp = IsKeyDown(KEY_UP) || (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, upButton));
        bool movingDown = IsKeyDown(KEY_DOWN) || (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, downButton));
        bool movingLeft = (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, leftButton_Move));
        bool movingRight = (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, rightButton_Move));
        bool turningLeft = IsKeyDown(KEY_LEFT) || (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, turnLeftButton));
        bool turningRight = IsKeyDown(KEY_RIGHT) || (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, turnRightButton));
        
        bool isMoving = movingUp || movingDown || movingLeft || movingRight || turningLeft || turningRight;
        Rectangle playerCarRec = {playerCar.position.x, playerCar.position.y, CAR_SIZE, CAR_SIZE};
        
        if (currentScreen == ENDING && isMoving) {
            playerCar.isServicing = false; playerCar.parked = false; currentScreen = GAMEPLAY; 
        }
        
        for (auto& car : cars) {
            if (car.type == ELECTRIC && car.isServicing) { 
                if (car.energyLevel < 100.0f) car.energyLevel += CHARGE_RATE;
                else car.energyLevel = 100.0f;
            }
        }
        
        if (!playerCar.parked) { 
            if (movingUp) playerCar.position.y -= moveSpeed;
            if (movingDown) playerCar.position.y += moveSpeed;
            if (movingLeft) playerCar.position.x -= moveSpeed;
            if (movingRight) playerCar.position.x += moveSpeed;
            if (turningLeft) playerCar.rotation -= rotationSpeed;
            if (turningRight) playerCar.rotation += rotationSpeed;

            if (!isMoving) {
                for (auto& spot : chargingSpots) {
                    if (CheckCollisionRecs(playerCarRec, spot.rect) && !spot.occupied) {
                        int zoneIndex = getZoneIndex(spot.index);
                        bool isEVSpot = (zoneIndex == EV_ZONE_INDEX_1 || zoneIndex == EV_ZONE_INDEX_2);
                        if (isEVSpot && playerCar.type == ELECTRIC) {
                            playerCar.parked = true;
                            playerCar.isServicing = true; 
                            spot.occupied = true;
                            spot.occupiedByCarIndex = currentCarIndex; 
                            zones[zoneIndex].occupiedSlots++;
                            currentScreen = GAMEPLAY;
                            break; 
                        } 
                    }
                }
            }
        } 
        
        if (playerCar.parked && isMoving) {
             if (playerCar.isServicing && playerCar.type == ELECTRIC) playerCar.isServicing = false;
             playerCar.parked = false;
             playerCar.tint = playerCar.originalTint; 
             for (auto& spot : chargingSpots) {
                 if (spot.occupiedByCarIndex == currentCarIndex) { 
                     spot.occupied = false;
                     spot.occupiedByCarIndex = -1;
                     int zoneIndex = getZoneIndex(spot.index);
                     if (zoneIndex != -1) zones[zoneIndex].occupiedSlots--;
                     break;
                 }
             }
             playerCar.tint = YELLOW;
        }
        
        if (playerCar.type == ELECTRIC && playerCar.energyLevel >= 100.0f && playerCar.isServicing) {
            if (!isMoving) {
                currentScreen = ENDING;
                playerCar.parked = true;
            }
        }
        
        if (playerCar.position.x < 0) playerCar.position.x = 0;
        if (playerCar.position.x > screenWidth - CAR_SIZE) playerCar.position.x = screenWidth - CAR_SIZE;
        if (playerCar.position.y < 0) playerCar.position.y = 0;
        if (playerCar.position.y > screenHeight - CAR_SIZE) playerCar.position.y = screenHeight - CAR_SIZE;
    }
}

void Game::Draw() {
    Rectangle parkingAreaRec = {0, 0, (float)screenWidth, (float)screenHeight};

    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    if (currentScreen == MENU) {
        DrawTexturePro(loadedTextures["menu_bg"], {0,0,(float)loadedTextures["menu_bg"].width,(float)loadedTextures["menu_bg"].height},
            {0,0,(float)screenWidth,(float)screenHeight}, {0,0}, 0, WHITE);
            
        DrawRectangleRec(playButton, BROWN);
        DrawText("PLAY", playButton.x + 40, playButton.y + 10, 20, BLACK);
        
        DrawRectangleRec(exitButton, GRAY);
        DrawText("EXIT", exitButton.x + 40, exitButton.y + 10, 20, BLACK);
        
    } else if (currentScreen == GAMEPLAY || currentScreen == PAUSE || currentScreen == ENDING) {
        
        DrawTexturePro(loadedTextures["parking"], {0,0,(float)loadedTextures["parking"].width,(float)loadedTextures["parking"].height},
                        parkingAreaRec, {0,0}, 0, WHITE);
        
        char infoText[100];
        const int ICON_BAR_HEIGHT = 60; 
        const int TILE_WIDTH = 110; 
        const int TILE_Y = 5; 
        
        DrawRectangle(0, 0, screenWidth, ICON_BAR_HEIGHT + 10, Fade(BLACK, 0.4f)); 
        
        const int START_X_LEFT = 200; 
        const auto& zone1 = zones[PARKING_ZONE_INDEX]; 
        char costUnitText1[50]; sprintf(costUnitText1, "%.2f dh/h", zone1.costPerHour);
        // INFO PLACES LIBRES SUPPRIMÉE ICI
        DrawInfoTile(START_X_LEFT, TILE_Y, TILE_WIDTH, 50, BLUE, zone1.label, "", costUnitText1);
        
        int currentTileX_Right = screenWidth - TILE_WIDTH - 30; 
        const auto& zone2 = zones[EV_ZONE_INDEX_1]; 
        char costUnitText2[50]; sprintf(costUnitText2, "%.2f dh/h", zone2.costPerHour);
        // INFO PLACES LIBRES SUPPRIMÉE ICI
        DrawInfoTile(currentTileX_Right, TILE_Y, TILE_WIDTH, 50, RED, zone2.label, "", costUnitText2);
        
        const auto& zoneEV = zones[EV_ZONE_INDEX_2];
        int textX = (int)zoneEV.rect.x + 5; 
        int textY = (int)zoneEV.rect.y - 65; 
        
        // INFO PLACES LIBRES SUPPRIMÉE ICI (Seulement Nom et Prix)
        sprintf(infoText, "%s\nCout: %.2f dh/h", 
            zoneEV.label, zoneEV.costPerHour);
        
        DrawRectangle((int)zoneEV.rect.x, (int)zoneEV.rect.y - 75, 220, 65, Fade(GREEN, 0.6f)); 
        DrawTextLeftAligned(infoText, textX, textY, 15, WHITE);
        
        for (const auto& spot : chargingSpots) {
            Color spotColor = LIME; 
            if (spot.index >= 4) { 
                 DrawRectangleLinesEx(spot.rect, 2, spot.occupied ? RED : spotColor);
                 DrawText(TextFormat("Spot %d", spot.index), (int)spot.rect.x + 5, (int)spot.rect.y + 5, 15, BLACK);
            }
            if (spot.occupiedByCarIndex != -1 && cars[spot.occupiedByCarIndex].isServicing) {
                const Car& carCharging = cars[spot.occupiedByCarIndex];
                Rectangle chargeBar = {spot.rect.x, spot.rect.y + spot.rect.height - 15, spot.rect.width, 10};
                DrawRectangleRec(chargeBar, DARKGRAY);
                float currentWidth = chargeBar.width * (carCharging.energyLevel / 100.0f);
                DrawRectangle((int)chargeBar.x, (int)chargeBar.y, (int)currentWidth, (int)chargeBar.height, GREEN);
                DrawText(TextFormat("%.0f%%", carCharging.energyLevel), 
                            (int)chargeBar.x + (int)chargeBar.width / 2 - 20, (int)chargeBar.y - 15, 15, GREEN);
            }
        }
        
        if (gameStarted) {
            for (size_t i = 0; i < cars.size(); ++i) {
                const auto& c = cars[i];
                DrawTexturePro(c.texture, 
                    {0,0,(float)c.texture.width,(float)c.texture.height}, 
                    {c.position.x + CAR_SIZE/2, c.position.y + CAR_SIZE/2, CAR_SIZE, CAR_SIZE}, 
                    {CAR_SIZE/2, CAR_SIZE/2}, c.rotation, c.tint);
                        
                DrawText(TextFormat("Car %d (%s)", (int)i, (c.type == ELECTRIC ? "EV" : "ICE")), 
                         (int)c.position.x - 5, (int)c.position.y - 10, 10, BLACK);

                if (c.type == ELECTRIC) {
                    float barWidth = 30.0f;
                    float barHeight = 5.0f;
                    Rectangle barBg = {c.position.x + (CAR_SIZE/2 - barWidth/2), c.position.y - 20, barWidth, barHeight};
                    float fillWidth = barWidth * (c.energyLevel / 100.0f);
                    
                    Color barColor = LIME; 
                    if (c.energyLevel < 50.0f) barColor = ORANGE;
                    if (c.energyLevel < 20.0f) barColor = RED;

                    DrawRectangleRec(barBg, DARKGRAY);
                    DrawRectangle((int)barBg.x, (int)barBg.y, (int)fillWidth, (int)barHeight, barColor);
                }
            }
        }

        if (currentScreen != PAUSE && gameStarted) {
            // Contrôles réduits
            DrawRectangleRec(turnLeftButton, Fade(PURPLE, 0.8f)); DrawText("<-", turnLeftButton.x + 12, turnLeftButton.y + 10, 15, BLACK);
            DrawRectangleRec(turnRightButton, Fade(PURPLE, 0.8f)); DrawText("->", turnRightButton.x + 12, turnRightButton.y + 10, 15, BLACK);
            
            DrawRectangleRec(upButton, Fade(GREEN, 0.8f)); DrawText("^", upButton.x + 12, upButton.y + 8, 18, BLACK);
            DrawRectangleRec(downButton, Fade(RED, 0.8f)); DrawText("v", downButton.x + 12, downButton.y + 8, 18, BLACK);
            DrawRectangleRec(leftButton_Move, Fade(BLUE, 0.8f)); DrawText("<<", leftButton_Move.x + 8, leftButton_Move.y + 8, 15, BLACK);
            DrawRectangleRec(rightButton_Move, Fade(BLUE, 0.8f)); DrawText(">>", rightButton_Move.x + 8, rightButton_Move.y + 8, 15, BLACK);
            
            DrawText(TextFormat("Controlle: Car %d. Utilisez TAB pour changer.", currentCarIndex), 
                        (int)tabControlRec.x, (int)tabControlRec.y, 15, WHITE);
        }
        
        if (gameStarted && cars[currentCarIndex].energyLevel < 20.0f) {
            const char* label = (cars[currentCarIndex].type == ELECTRIC) ? "Batterie" : "Carburant";
            DrawText(TextFormat("%s: %.0f%%", label, cars[currentCarIndex].energyLevel), screenWidth - 150, screenHeight - 40, 15, RED);
        }
        
        if (currentScreen == PAUSE) {
            DrawTexturePro(loadedTextures["pause_bg"], {0,0,(float)loadedTextures["pause_bg"].width,(float)loadedTextures["pause_bg"].height},
                {screenWidth/2.0f-100, screenHeight/2.0f-100, 100, 100}, {0,0},0,WHITE);
            DrawText("PAUSE", screenWidth/2-50, screenHeight/2-130, 30, BLACK);
        }
        if (currentScreen == ENDING) {
            DrawTexturePro(loadedTextures["pause_bg"], {0,0,(float)loadedTextures["pause_bg"].width,(float)loadedTextures["pause_bg"].height},
                {screenWidth/2.0f-100, screenHeight/2.0f-100, 100, 100}, {0,0},0,WHITE);
            DrawText(TextFormat("VEHICULE %d CHARGÉ À 100%%!", currentCarIndex), screenWidth/2-190, screenHeight/2-130, 20, GREEN);
            DrawText("Appuyez sur BACK pour quitter le spot et continuer.", screenWidth/2-180, screenHeight/2+80, 20, BLACK);
        }
    }

    // --- ZONE GLOBALE DE DESSIN (Toujours visible sauf MENU) ---
    
    // Boutons globaux (Volume, Back, Pause)
  Rectangle src, dst;
src = {0,0,(float)loadedTextures["speaker_on"].width,(float)loadedTextures["speaker_on"].height};
dst = {volumeButton.x, volumeButton.y, volumeButton.width, volumeButton.height};
DrawTexturePro(soundOn ? loadedTextures["speaker_on"] : loadedTextures["speaker_off"], src, dst, {0,0},0,WHITE);

    src = {0,0,(float)loadedTextures["back"].width,(float)loadedTextures["back"].height};
    dst = {backButton.x, backButton.y, backButton.width, backButton.height};
    DrawTexturePro(loadedTextures["back"], src, dst, {0,0},0,WHITE);

    if (currentScreen != MENU) {
        src = {0,0,(float)loadedTextures["pause"].width,(float)loadedTextures["pause"].height};
        dst = {pauseButton.x, pauseButton.y, pauseButton.width, pauseButton.height};
        DrawTexturePro(loadedTextures["pause"], src, dst, {0,0},0,WHITE);
    }

    // DESSIN DU BOUTON REJOUER (Haut gauche, sous speaker)
    if (currentScreen != MENU) {
        if (loadedTextures.count("restart") && loadedTextures["restart"].id > 0) {
            Rectangle srcRes = {0,0,(float)loadedTextures["restart"].width,(float)loadedTextures["restart"].height};
            Rectangle dstRes = {restartButton.x, restartButton.y, restartButton.width, restartButton.height};
            DrawTexturePro(loadedTextures["restart"], srcRes, dstRes, {0,0}, 0, WHITE);
        } else {
            DrawRectangleRec(restartButton, ORANGE);
            DrawText("REPLAY", restartButton.x + 20, restartButton.y + 8, 12, BLACK);
        }
    }

    EndDrawing();
}