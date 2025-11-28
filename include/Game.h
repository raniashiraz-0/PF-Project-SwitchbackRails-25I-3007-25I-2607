#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

class Game {
public:
    Game();             // Constructor
    void run();         // Start the game loop
private:
    sf::RenderWindow window; // SFML window
    void processEvents();    // Handle input
    void update();           // Update game logic
    void render();           // Draw everything
};

#endif

