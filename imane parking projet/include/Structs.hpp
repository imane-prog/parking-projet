// include/Structs.h
#ifndef STRUCTS_H
#define STRUCTS_H

#include <raylib.h>
#include <map>
#include <string>

// --- NOUVELLE ÉNUMÉRATION POUR LES TYPES DE CARBURANT ---
enum FuelType { ELECTRIC = 0, GASOLINE };

// Énumération pour les états de jeu
enum GameScreen { MENU = 0, GAMEPLAY, PAUSE, ENDING };

// Opérateur de comparaison pour la structure Color (Nécessaire pour l'opérateur ==)
bool operator==(const Color& a, const Color& b); 

// Structure pour l'emplacement de service (recharge ou plein)
struct ChargingSpot {
    Rectangle rect;
    bool occupied; 
    int index; 
    int occupiedByCarIndex; 
};

// Structure de la voiture
struct Car {
    Texture2D texture;
    Vector2 position;
    float speed;
    float rotation; 
    bool parked;
    
    float energyLevel; 
    FuelType type; 
    
    bool isServicing; 
    
    Color tint; 
    Color originalTint; 
};

// Structure de zone de parking existante
struct ParkingZone {
    Rectangle rect; 
    int totalSlots; 
    int occupiedSlots; 
    float costPerHour; 
    Color color; 
    const char* label; 
};

// --- DÉCLARATION DES CONSTANTES GLOBALES (ou ConfigManager) ---
const int screenWidth = 800;
const int screenHeight = 600;
const float CAR_SIZE = 40.0f;
const float CHARGE_RATE = 0.5f; 

// Index des zones
const int PARKING_ZONE_INDEX = 0;
const int EV_ZONE_INDEX_1 = 1; 
const int EV_ZONE_INDEX_2 = 2; 

#endif // STRUCTS_H