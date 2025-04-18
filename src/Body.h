#ifndef BODY_H
#define BODY_H

#include <SFML/Graphics.hpp>
#include "QuadTree.h"

class Body {
    public:  
        Body(float x = 0, float y = 0, float mass = 0.0f, float radius = 5.0f, sf::Color color = sf::Color::White); // constructor initializes the body's properties
        
        void update(float deltaTime);

        void applyForce(const sf::Vector2f& force);

        void setVelocity(const sf::Vector2f& newVelocity);

        sf::Vector2f getPosition() const;

        float getMass() const;

        float getRadius() const;

        sf::Vector2f getVelocity() const;

        sf::Color getColor();
        
        void draw(sf::RenderWindow& window) const; // draws the body using SFML

        const sf::CircleShape& getShape() const;

        //sf::CircleShape shape; // shape to represent the body
        sf::Vector2f position; // (x, y) coordinates in 2D space
        sf::Vector2f velocity; // (vx, vy) speed in pixels per second
        sf::Vector2f acceleration; // (ax, ay) acceleration in pixel per second
        float mass; // mass of the body
        sf::Color color;
        float radius;
};

#endif