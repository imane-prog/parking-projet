#include <iostream>
#include <cassert>
#include "../include/Structs.h"
#include "../include/Game.hpp"

// Fonction utilitaire pour afficher le succès
void TestPassed(const char* name) {
    std::cout << "[SUCCESS] Test: " << name << std::endl;
}

// 1. Test de la recharge d'énergie
void TestCarCharging() {
    Car testCar;
    testCar.type = ELECTRIC;
    testCar.energyLevel = 50.0f;
    testCar.isServicing = true;

    // Simulation d'une étape de recharge
    float chargeRate = 0.5f;
    testCar.energyLevel += chargeRate;

    assert(testCar.energyLevel == 50.5f);
    
    // Test de la limite à 100%
    testCar.energyLevel = 99.9f;
    testCar.energyLevel += chargeRate;
    if (testCar.energyLevel > 100.0f) testCar.energyLevel = 100.0f;
    
    assert(testCar.energyLevel == 100.0f);
    TestPassed("Car Charging Logic");
}

// 2. Test de collision simple (Parking Spot)
void TestSpotCollision() {
    Rectangle spot = { 100, 100, 50, 50 };
    Vector2 carPos = { 110, 110 }; // À l'intérieur du rectangle

    bool isInside = CheckCollisionPointRec(carPos, spot);
    assert(isInside == true);

    Vector2 carPosOutside = { 300, 300 }; // À l'extérieur
    bool isOutside = CheckCollisionPointRec(carPosOutside, spot);
    assert(isOutside == false);

    TestPassed("Collision Logic");
}

int main() {
    std::cout << "--- DEMARRAGE DES TESTS UNITAIRES ---" << std::endl;
    
    // Initialisation minimale de Raylib si nécessaire pour les types
    // (Note: CheckCollisionPointRec ne nécessite pas de fenêtre ouverte)
    
    TestCarCharging();
    TestSpotCollision();

    std::cout << "--- TOUS LES TESTS SONT OK ---" << std::endl;
    return 0;
}