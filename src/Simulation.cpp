    #include "Simulation.h"
    #include "Force.h"
    #include "Body.h"
    #include <random>
    #include <algorithm>
    #include <iostream>
    #include "QuadTree.h"
    #include <thread>

    int capacity = 50;
    QuadTreeNode::Region squilly({960, 540}, 1920, capacity);
    QuadTreeNode testTree(squilly, capacity); 
    
    //RATIO 0.00006
    Simulation::Simulation() {
        bodies.reserve(20000);

        std::random_device rd;
        std::mt19937 gen(rd());

        Body bigMass1(960, 540, 80000000.0f, 5.0f, sf::Color::White);
        bigMass1.setVelocity({12.0f, 0.0f});
        bodies.push_back(bigMass1);

        int numBodies = 20000;
        std::uniform_real_distribution<> angleDist(0, 2 * M_PI);
        std::exponential_distribution<> exDist(0.004);
        std::uniform_real_distribution<> xDist(0, 1920);
        std::uniform_real_distribution<> yDist(0, 1080);
        std::uniform_real_distribution<> circleSize(0, 300);

        for (int i = 0; i < numBodies; ++i) {
            float angle = angleDist(gen);

            float distance = circleSize(gen) + 12;
            float x = bigMass1.getPosition().x + std::cos(angle) * distance;
            float y = bigMass1.getPosition().y + std::sin(angle) * distance;
    
            float velMag = std::sqrt(bigMass1.getMass() / distance * 1.15);
            sf::Vector2f vel = {-std::sin(angle) * velMag, std::cos(angle) * velMag};
            vel += bigMass1.getVelocity();

            Body bod(x, y, 2500.0f, 0.75f, sf::Color::White);
            bod.setVelocity(vel);
            bodies.push_back(bod);
        }
    }
    
    void Simulation::update(float deltaTime) {
        t.start();

        testTree = QuadTreeNode(squilly, capacity);
        for (auto& body : bodies) {
            testTree.insert(body);
        }
        testTree.propagate();

        t.stop();
        treeBuildTimes.push_back(t.elapsed());

        float theta = 0.8;
        t.start();

        std::vector<std::thread> threads;
        int numBlocks = std::thread::hardware_concurrency(); // allows for use of hyper threading if available.
        int bodiessPerThread = bodies.size() / numBlocks;

        for (int i=0; i != numBlocks; ++i) {
            threads.emplace_back([&, i]() { // lambda function so we can construct thread inside the vector
                int start = i * bodiessPerThread;
                int end = (i == numBlocks - 1) ? bodies.size() : start + bodiessPerThread; // helps to include leftovers in uneven division
                for (int j = start; j < end; ++j) {
                    testTree.calculateForces(bodies[j], theta);
                }
            });
        }
        
        for (auto& thread : threads) { 
            thread.join();
        }
        threads.clear();

        t.stop();
        calculateForcesTimes.push_back(t.elapsed());

        t.start();
        
        for (auto& body : bodies) {
            body.update(deltaTime);
        }

        t.stop();
        updateBodiesTimes.push_back(t.elapsed());
    }

    void Simulation::render(sf::RenderWindow& window) {
        sf::VertexArray points (sf::PrimitiveType::Points, bodies.size());

        for (size_t i = 0; i < bodies.size(); ++i){
            points[i].position = bodies[i].getPosition();
            points[i].color = bodies[i].getColor();
        }

        window.draw(points);

        sf::CircleShape centerCircle;
        centerCircle.setRadius(6); // visual size in pixels
        centerCircle.setFillColor(sf::Color::White);
        centerCircle.setOrigin({6.0f,6.0f}); // center the circle on position
        centerCircle.setPosition(bodies[0].getPosition()); // assuming it's the first body
        window.draw(centerCircle);


        // for (auto& body : bodies) {
        //     window.draw(body.getShape());
        // }
        //testTree.drawRegionLines(window);
    }

    void Simulation::printStats() {
        std::cout << "Average Tree Build Time: " << std::accumulate(treeBuildTimes.begin(), treeBuildTimes.end(), 0.0)/treeBuildTimes.size() << "ms" << std::endl;
        std::cout << "Average Force Calc Time: " << std::accumulate(calculateForcesTimes.begin(), calculateForcesTimes.end(), 0.0)/calculateForcesTimes.size() << "ms" << std::endl;
        std::cout << "Average Update Bodies Time: " << std::accumulate(updateBodiesTimes.begin(), updateBodiesTimes.end(), 0.0)/updateBodiesTimes.size() << "ms" << std::endl;
    }