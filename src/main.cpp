#include <iostream>
#include "Body.h"
#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include "QuadTree.h"
#include <thread>

int main() {
    // create a SFML window with a resolution of 1920x1080
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "N-Body Simulation", sf::State::Windowed);
        
    // initialize the simulation instance
    Simulation simulation;

     // set a fixed delta time for the simulation update
    const float fixedDeltaTime = 1.0f / 675.0f;
    // counter for num of updates
    int numTimeSteps = 0;

    // load font for displaying statistics on screen
    sf::Font font;
    if (!font.openFromFile("C:/Users/matth/Desktop/N-Body_Sim/times.ttf")) {
        std::cerr << "Failed to load font!\n";
        return -1;
    }

    // initialize text object to display simulation statistics on the screen
    sf::Text statsText(font);
    statsText.setFont(font);
    statsText.setCharacterSize(24);
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition({10.0f, 10.0f});

    // start the main simulation loop
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>() || 
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) { 
                window.close();
            }
        }

        // stop the simulation after 800 time steps
        if (numTimeSteps > 800) { window.close(); }

        // clear off screen and set background to black
        window.clear(sf::Color::Black);
        
        // build and propagate QuadTree, Calculate Forces, Update bodies velocity and position
        simulation.update(fixedDeltaTime);

        // draw bodies at their new position
        simulation.render(window);

        // display stats for current time step
        statsText.setString(
            "Bodies: " + std::to_string(simulation.getStats(numTimeSteps)[3]) + "\n" +
            "Theta: " + std::to_string(simulation.getStats(numTimeSteps)[4]) + "\n" +
            "Capacity: " + std::to_string(simulation.getStats(numTimeSteps)[5]) + "\n\n" +
            "Num Time Steps: " + std::to_string(numTimeSteps) + "\n" +
            "Tree Build: " + std::to_string(simulation.getStats(numTimeSteps)[0]) + " ms\n" +
            "Force Calc: " + std::to_string(simulation.getStats(numTimeSteps)[1]) + " ms\n" +
            "Update Bodies: " + std::to_string(simulation.getStats(numTimeSteps)[2]) + " ms"
        );
        
        // draw stats text
        window.draw(statsText);

        // display current rendering
        window.display();

        numTimeSteps++;
    }
    
    simulation.printFinalStats();
}
