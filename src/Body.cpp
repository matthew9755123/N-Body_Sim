#include "Body.h"

Body::Body(float x, float y, float mass, float radius, sf::Color color)
    : position(x, y), velocity (0, 0), acceleration(0, 0), mass(mass), color(color), radius(radius){
        // shape.setRadius(radius);
        // shape.setFillColor(color);
        // shape.setPosition(position);
        // shape.setOrigin({shape.getRadius(), shape.getRadius()});
    }

    void Body::update(float deltaTime) {
        velocity += acceleration * deltaTime;
        position += velocity * deltaTime;
        //shape.setPosition(position);
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

    float Body::getRadius() const {
       // return shape.getRadius();
    }

    sf::Vector2f Body::getVelocity() const {
        return velocity;
    }
    
    void Body::draw(sf::RenderWindow& window) const {
        //window.draw(shape);
    }

    const sf::CircleShape& Body::getShape() const {
        //return shape;
    }
    
    sf::Color Body::getColor() {
        return color;
    }
