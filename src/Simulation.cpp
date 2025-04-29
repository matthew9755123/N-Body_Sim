#include "Simulation.h"
#include "Force.h"
#include "Body.h"
#include <random>
#include <algorithm>
#include <iostream>
#include <thread>
#include <future>
#include "BS_thread_pool.hpp"

int capacity = 60;
QuadTreeNode::Region squilly({960, 540}, 1920, capacity);
QuadTreeNode testTree(squilly, capacity);

Simulation::Simulation() {
    bodies.reserve(n);

    std::random_device rd;
    std::mt19937 gen(rd());

    Body bigMass1(960, 540, 80000000.0f, 5.0f, sf::Color::White);
    bodies.push_back(bigMass1);

    std::uniform_real_distribution<> angleDist(0, 2 * M_PI);
    std::exponential_distribution<> exDist(0.004);
    std::uniform_real_distribution<> xDist(0, 1920);
    std::uniform_real_distribution<> yDist(0, 1080);
    std::uniform_real_distribution<> circleSize(0, 250);

    for (int i = 0; i < n; ++i) {
        float angle = angleDist(gen);

        float distance = circleSize(gen) + 12;
        float x = bigMass1.getPosition().x + std::cos(angle) * distance;
        float y = bigMass1.getPosition().y + std::sin(angle) * distance;

        float velMag = std::sqrt(bigMass1.getMass() / distance * 1.15);
        sf::Vector2f vel = {-std::sin(angle) * velMag, std::cos(angle) * velMag};

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
    treeBuildTimes.push_back(t.elapsed()/1000);

    t.start();

    int numBlocks = std::thread::hardware_concurrency();
    int bodiesPerThread = bodies.size() / numBlocks;

    std::vector<std::future<void>> futures;
    for (int i = 0; i < numBlocks; ++i) {
        int start = i * bodiesPerThread;
        int end = (i == numBlocks - 1) ? bodies.size() : start + bodiesPerThread;

        futures.push_back(pool.submit_task([&, start, end]() {
            for (int j = start; j < end; ++j) {
                testTree.calculateForces(bodies[j], theta);
            }
        }));
    }

    for (auto& f : futures) {
        f.get();
    }

    t.stop();
    calculateForcesTimes.push_back(t.elapsed()/1000);

    t.start();
    
    for (auto& body : bodies) {
        body.update(deltaTime);
    }

    t.stop();
    updateBodiesTimes.push_back(t.elapsed()/1000);
}

void Simulation::render(sf::RenderWindow& window) {
    sf::VertexArray points (sf::PrimitiveType::Points, bodies.size());

    for (size_t i = 0; i < bodies.size(); ++i){
        points[i].position = bodies[i].getPosition();
        points[i].color = bodies[i].getColor();
    }

    window.draw(points);

    sf::CircleShape centerCircle;
    centerCircle.setRadius(6);
    centerCircle.setFillColor(sf::Color::White);
    centerCircle.setOrigin({6.0f,6.0f});
    centerCircle.setPosition(bodies[0].getPosition());
    window.draw(centerCircle);


    //testTree.drawRegionLines(window);
}

std::vector<float> Simulation::getStats(int i) {
    std::vector<float> currStats(6);

    if (i < treeBuildTimes.size() && i < calculateForcesTimes.size() && i < updateBodiesTimes.size()) {
        currStats[0] = treeBuildTimes[i];
        currStats[1] = calculateForcesTimes[i];
        currStats[2] = updateBodiesTimes[i];
        currStats[3] = n;
        currStats[4] = theta;
        currStats[5] = capacity;
    } else {
        currStats[0] = currStats[1] = currStats[2] = -1.0f;
    }

    return currStats;
}

void Simulation::printFinalStats() {
    std::cout << "Average Tree Build Time: " << std::accumulate(treeBuildTimes.begin(), treeBuildTimes.end(), 0.0)/treeBuildTimes.size() << "ms" << std::endl;
    std::cout << "Average Force Calc Time: " << std::accumulate(calculateForcesTimes.begin(), calculateForcesTimes.end(), 0.0)/calculateForcesTimes.size() << "ms" << std::endl;
    std::cout << "Average Update Bodies Time: " << std::accumulate(updateBodiesTimes.begin(), updateBodiesTimes.end(), 0.0)/updateBodiesTimes.size() << "ms" << std::endl;
}