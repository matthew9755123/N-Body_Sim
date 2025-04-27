#ifndef SIMULATION_H
#define SIMULATION_H

#include <SFML/Graphics.hpp>
#include "Body.h"
#include "timer.hpp"
#include "BS_thread_pool.hpp"
#include "QuadTree.h"


/**
 * @class Simulation
 * @brief Represents the main simulation that calculates forces, updates bodies, 
 *        and handles rendering using a quadtree
 */
class Simulation {
    private:
        std::vector<Body> bodies; // list of bodies in the simulation
        std::vector<float> treeBuildTimes; // stores times for quadtree construction
        std::vector<float> calculateForcesTimes; // stores times for force calculation
        std::vector<float> updateBodiesTimes; // stores times for updating bodies
        std::vector<float> stats; // stores general statisics

        timer t; // timer used to measure elapsed times during simulation updates

        BS::thread_pool<> pool; // thread pool used for parallelizing force calculation

        int n = 22000; 
        float theta = 0.8;

    public:
        /**
         * @brief Default constructor for the simulation. Initializes bodies and their positions
         */
        Simulation();

        /**
         * @brief Updates the simulation by calculating forces and updating the body positions
         * 
         * @param deltaTime time frame
         */
        void update(float deltaTime);

        /**
         * @brief Renders the simulation by drawing bodies and additional visuals on the screen
         * 
         * @param window The SFML window where the simulation is rendered
         */
        void render(sf::RenderWindow& window);

         /**
         * @brief Retrieves specific simulation statistics
         * 
         * @param i The index of the stats to retrieve
         * @return A vector containing the stats
         */
        std::vector<float> getStats(int i);

        /**
         * @brief Prints the final statistics of the simulation.
         */
        void printFinalStats();
};

#endif