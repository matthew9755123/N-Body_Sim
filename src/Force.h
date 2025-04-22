#ifndef FORCE_H
#define FORCE_H

#include <SFML/Graphics.hpp>
#include "Body.h"

class Body; // forward declaration to inform compiler Body exists

/**
 * @class Force
 * @brief A class that contains a static function to calcualte physical forces between bodies.
 * 
 * @details The 'Force' class provides static methods to calcualte different types of forces between two bodies (or region COM) 
 */
class Force {
    private: 
        static constexpr float G = 1.0f;

    public:
        /**
         * @brief Static function that calculates the gravitational force between two bodies.
         *
         * @details This function uses the masses and positions of two bodies (or region COM) to compute the gravitational
         *          forces between them
         *
         * @param mass1 The mass of the first body or region COM.
         * @param pos1 The position of the first body or region COM.
         * @param mass2 The mass of the second body or region COM.
         * @param pos2 The position of the second body or region COM.
         * 
         * @return The gravitational force vector acting on the first body, representing both magnitude and direction. 
        *          The result is a sf::Vector2f with components proportional to the force experienced by the first body 
        *          due to the second body.
        */
        static sf::Vector2f calcGravityForce(float mass1, sf::Vector2f pos1, float mass2, sf::Vector2f pos2);       
};

#endif