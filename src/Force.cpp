#include "Force.h"
#include <cmath>

sf::Vector2f Force::calcGravityForce(float mass1, sf::Vector2f pos1, float mass2, sf::Vector2f pos2) {
    // compute the distance between both bodies
    sf::Vector2f distanceVec = pos2 - pos1;
    
    // compute the squared distance
    float distanceSquared = distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y;
    
    // help avoid large vectors
    float softCoreDistance = std::sqrt(0.1 * 0.1 + distanceSquared);

    // calculate magnitude
    float forceMagnitude = (G * mass1 * mass2) / (softCoreDistance * softCoreDistance); 
    
    // calcualte distance
    float distance = std::sqrt(distanceSquared);
    
    // calculate direction
    sf::Vector2f forceDirection = (distance > 0.0f) ? (distanceVec / distance) : sf::Vector2f(0.0f, 0.0f);

    // special case where if the masses are large and distance is very small, the force is zeroed
    if ((mass1 > 21000.0f || mass2 > 21000.0f) && distance <= 10.0f) {
         return sf::Vector2f({0.0, 0.0});
    } else if (distance <= 1.1f) {
        return sf::Vector2f({0.0, 0.0});
    }
    
    // return acceleration vector
    return forceDirection * forceMagnitude;
}