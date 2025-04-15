#ifndef SIMULATION_H
#define SIMULATION_H

#include <SFML/Graphics.hpp>
#include "Body.h"

/* 
Represents a physical body in the simulation. Stores position, 
velocity, mass, and methods for updating motion.
*/
class Simulation {
    private:
        std::vector<Body> bodies;

    public:
        Simulation();

        void update(float deltaTime);
        void render(sf::RenderWindow& window);
        void mergeOverlappingBodies();
};

#endif