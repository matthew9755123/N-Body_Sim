#include "Body.h"

Body::Body(float x, float y, float mass, float radius, sf::Color color)
    : position(x, y), velocity (0, 0), acceleration(0, 0), mass(mass), color(color), radius(radius){
}

void Body::update(float deltaTime) {
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    acceleration = sf::Vector2f(0, 0);
}

void Body::applyForce(const sf::Vector2f& force) {
    acceleration += force/mass;
}

void Body::setVelocity(const sf::Vector2f& newVelocity) {
    velocity = newVelocity;
}

sf::Vector2f Body::getPosition() const{
    return position;
}

float Body::getMass() const{
    return mass;
}

sf::Vector2f Body::getVelocity() const {
    return velocity;
}

void Body::draw(sf::RenderWindow& window) const {
    //window.draw(shape);
}

sf::Color Body::getColor() {
    return color;
}
