#include "Force.h"
#include <cmath>

const float G = 1.0f;

sf::Vector2f Force::calcGravForce(float node1Mass, sf::Vector2f node1Pos, float node2Mass, sf::Vector2f node2Pos) {
    sf::Vector2f distanceVec = node2Pos - node1Pos;
    
    float distanceSquared = distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y;
    
    const float epsilon = 0.0001f;

    float softCoreDistance = std::sqrt(epsilon * epsilon + distanceSquared);
    
    float forceMagnitude = (G * node1Mass * node2Mass) / (softCoreDistance * softCoreDistance);
    
    float distance = std::sqrt(distanceSquared);
    
    sf::Vector2f forceDirection = (distance > 0.0f) ? (distanceVec / distance) : sf::Vector2f(0.0f, 0.0f);

    if ((node1Mass > 1000.0f || node2Mass > 1000.0f) && distance <= 15.0f) {
         return sf::Vector2f({0.0, 0.0});
    }else if (distance <= 1.5f)
    {
        return sf::Vector2f({0.0, 0.0});
    }
    
    return forceDirection * forceMagnitude;
}