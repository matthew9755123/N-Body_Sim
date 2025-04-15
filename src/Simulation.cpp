    #include "Simulation.h"
    #include "Force.h"
    #include "Body.h"
    #include <random>
    #include <algorithm>
    #include <iostream>
    #include "QuadTree.h"
    #include <omp.h>

    QuadTreeNode::Region squilly({960, 540}, 1920, 1);
    QuadTreeNode testTree(squilly, 1); 
    
    //RATIO 0.00006
    Simulation::Simulation() {
        std::random_device rd;
        std::mt19937 gen(rd());
    
        Body bigMass1(960, 540, 10000000.0f, 5.0f, sf::Color(186, 62, 0)); 
        bodies.push_back(bigMass1);

        int numBodies = 4000;
        std::uniform_real_distribution<> angleDist(0, 2 * M_PI);
        std::exponential_distribution<> distDist(0.01);
        std::uniform_real_distribution<> xDist(0, 1920);
        std::uniform_real_distribution<> yDist(0, 1090);
        
        for (int i = 0; i < numBodies; ++i) {
            float angle = angleDist(gen);

            float distance = distDist(gen) + 15;
            float x = bigMass1.getPosition().x + std::cos(angle) * distance;
            float y = bigMass1.getPosition().y + std::sin(angle) * distance;
    
            float velMag = std::sqrt(1.0f*bigMass1.getMass() / distance);
            sf::Vector2f vel = {-std::sin(angle) * velMag, std::cos(angle) * velMag};
            
            Body bod(x, y, 500.0f, 1.0f, sf::Color::White);
            bod.setVelocity(vel);
            bodies.push_back(bod);
        }
    }
    
    void Simulation::update(float deltaTime) {
        auto start = std::chrono::high_resolution_clock::now();

        testTree = QuadTreeNode(squilly, 1);

        for (auto& body : bodies) {
            testTree.insert(body);
        }
    
        testTree.propagate();

        float theta = 0.5;
        for (auto& body : bodies) {
            testTree.calculateForces(body, theta);
        }
    
        for (auto& body : bodies) {
            body.update(deltaTime);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Update time: " << elapsed.count() * 1000.0 << " ms" << std::endl;
;
    }

    void Simulation::render(sf::RenderWindow& window) {
        for (auto& body : bodies) {
            body.draw(window);
        }
        //testTree.draw(window);
    }