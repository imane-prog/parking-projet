#include "../include/Game.hpp"

int main() {
    Game game;

    game.Init();

    // Boucle de jeu principale
    while (!game.ShouldClose()) {
        game.Update();
        game.Draw();
    }

    // Le destructeur de Game (~Game) s'occupera de fermer Raylib
    return 0;
}