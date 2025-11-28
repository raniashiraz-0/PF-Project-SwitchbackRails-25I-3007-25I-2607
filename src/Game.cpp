#include "Game.h"

Game::Game() : window(sf::VideoMode(800, 600), "SFML Game") {}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update() {
    // Game logic goes here (nothing for now)
}

void Game::render() {
    window.clear(sf::Color::Black);
    // Draw game objects here
    window.display();
}

