#include <iostream>
#include "Body.h"
#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include "QuadTree.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "N-Body Simulation", sf::State::Fullscreen);

    Simulation simulation;
    const float fixedDeltaTime = 1.0f / 120.0f;
    size_t i = 0;

    while (window.isOpen()) {
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) { 
                    window.close();
                }
            }

            for (i; i <= 500; ++i){
                window.clear(sf::Color::Black);
                simulation.update(fixedDeltaTime);
                simulation.render(window);
                window.display();
            }
        }
        simulation.printStats();
}
