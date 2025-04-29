#ifndef BODY_H
#define BODY_H

#include <SFML/Graphics.hpp>
#include "QuadTree.h"


/**
 * @class Body
 * @brief A class representing a physical body in 2d space with mass, position, velocity, and acceleration.
 * 
 * @details The Body class encapsulates properties and behaviors of a single body. It provides methods to 
 *          update its state over time, apply external forces, set and retrieve velocity, and render itself using SFML
 */
class Body {
    public:  
        /**
         * @brief Constructs a Body with given position, mass, radius, and color.
         *
         * @param x The initial x-coordinate of the body.
         * @param y The initial y-coordinate of the body.
         * @param mass The mass of the body.
         * @param radius The visual radius of the body for rendering.
         * @param color The color used to represent the body.
         */
        Body(float x = 0, float y = 0, float mass = 0.0f, float radius = 5.0f, sf::Color color = sf::Color::White); // constructor initializes the body's properties
        
        /**
         * @brief Updates the body's position and velocity over a time step.
         *
         * @details Integrates acceleration into velocity, and velocity into position, using the given time step.
         *          Resets acceleration to zero after updating.
         *
         * @param deltaTime The time step over which to update the body's state.
         */
        void update(float deltaTime);

        /**
         * @brief Applies a force to the body.
         *
         * @details Adds the given force (divided by mass) to the body's current acceleration.
         *
         * @param force The force vector to apply.
         */
        void applyForce(const sf::Vector2f& force);

         /**
         * @brief Sets the velocity of the body.
         *
         * @details Directly assigns a new velocity vector to the body, overriding the current velocity.
         *
         * @param newVelocity The new velocity to set.
         */
        void setVelocity(const sf::Vector2f& newVelocity);

         /**
         * @brief Retrieves the current position of the body.
         * 
         * @return The 2D position vector of the body.
         */
        sf::Vector2f getPosition() const;
        
        /**
         * @brief Retrieves the mass of the body.
         * 
         * @return The mass of the body as a float.
         */
        float getMass() const;

        /**
         * @brief Retrieves the radius of the body.
         * 
         * @return The radius of the body used for rendering.
         */
        float getRadius() const;

        /**
         * @brief Retrieves the current velocity of the body.
         * 
         * @return The 2D velocity vector of the body.
         */
        sf::Vector2f getVelocity() const;

        /**
         * @brief Retrieves the color of the body.
         * 
         * @return The color of the body used for rendering.
         */
        sf::Color getColor();
        
        /**
         * @brief Draws the body onto the given window.
         *
         * @details This function is intended to render the body visually using SFML.
         *
         * @param window The SFML window where the body will be drawn.
         */
        void draw(sf::RenderWindow& window) const; // draws the body using SFML

        // Body properties
        sf::Vector2f position; // (x, y) coordinates in 2D space
        sf::Vector2f velocity; // (vx, vy) speed in pixels per second
        sf::Vector2f acceleration; // (ax, ay) acceleration in pixel per second
        float mass; // mass of the body
        sf::Color color; // color used to render body
        float radius; // radius for rendering the body
};

#endif