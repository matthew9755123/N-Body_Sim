#ifndef BODY_H
#define BODY_H

#include <SFML/Graphics.hpp>
#include "QuadTree.h"

/* 
Represents a physical body in the simulation. Stores position, 
velocity, mass, and methods for updating motion.
*/
class Body {
    public:  
        /**
         * @brief Constructs a Body object with given properties.
         * 
         * @param x Initial x-coordinate position.
         * @param y Initial y-coordinate position.
         * @param mass Mass of the body (affects force-based acceleration).
         * @param radius Radius of the body's shape (for visualization).
         * @param color Color of the body when rendered.
         */
        Body(float x = 0, float y = 0, float mass = 0.0f, float radius = 5.0f, sf::Color color = sf::Color::White); // constructor initializes the body's properties
        
        /**
         * @brief Updates the body's velocity and position based on acceleration.
         * 
         * @param deltaTime Time step for the simulation update (in seconds).
         */
        void update(float deltaTime);

        /**
         * @brief Applies a force to the body, modifying its acceleration.
         * 
         * @param force Force vector (F = ma) applied to the body.
         */
        void applyForce(const sf::Vector2f& force);

        /**
         * @brief Sets the body's velocity directly.
         * 
         * @param newVelocity The new velocity vector.
         */
        void setVelocity(const sf::Vector2f& newVelocity);

        /**
         * @brief Returns the position of the body
         * @return The body's position
         */
        sf::Vector2f getPosition() const;

        float getMass() const;

        float getRadius() const;

        sf::Vector2f getVelocity() const;
        
         /**
         * @brief Draws the body onto the given SFML render window.
         * 
         * @param window SFML RenderWindow where the body is drawn.
         */
        void draw(sf::RenderWindow& window) const; // draws the body using SFML


        sf::CircleShape shape; // shape to represent the body
        sf::Vector2f position; // (x, y) coordinates in 2D space
        sf::Vector2f velocity; // (vx, vy) speed in pixels per second
        sf::Vector2f acceleration; // (ax, ay) acceleration in pixel per second
        float mass; // mass of the body
};

#endif