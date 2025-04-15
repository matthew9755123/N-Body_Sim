#ifndef SIMULATION_H
#define SIMULATION_H

#include <SFML/Graphics.hpp>
#include "Body.h"
#include "timer.hpp"

/* 
Represents a physical body in the simulation. Stores position, 
velocity, mass, and methods for updating motion.
*/
class Simulation {
    private:
        std::vector<Body> bodies;
        std::vector<float> treeBuildTimes;
        std::vector<float> calculateForcesTimes;
        std::vector<float> updateBodiesTimes;

        timer t;

    public:
        Simulation();

        void update(float deltaTime);
        void render(sf::RenderWindow& window);
        void printStats();
};

#endif