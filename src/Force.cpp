#include "Force.h"
#include <cmath>

sf::Vector2f Force::calcGravityForce(float mass1, sf::Vector2f pos1, float mass2, sf::Vector2f pos2) {
    sf::Vector2f distanceVec = pos2 - pos1;
    
    float distanceSquared = distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y;
    float softCoreDistance = std::sqrt(0.1 * 0.1 + distanceSquared);
    float forceMagnitude = (G * mass1 * mass2) / (softCoreDistance * softCoreDistance);    
    float distance = std::sqrt(distanceSquared);
    
    sf::Vector2f forceDirection = (distance > 0.0f) ? (distanceVec / distance) : sf::Vector2f(0.0f, 0.0f);

    if ((mass1 > 21000.0f || mass2 > 21000.0f) && distance <= 10.0f) {
         return sf::Vector2f({0.0, 0.0});
    } else if (distance <= 1.1f) {
        return sf::Vector2f({0.0, 0.0});
    }
    
    return forceDirection * forceMagnitude;
}