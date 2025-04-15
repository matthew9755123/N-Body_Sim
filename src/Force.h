#ifndef FORCE_H
#define FORCE_H

#include <SFML/Graphics.hpp>
#include "Body.h"

class Body;
class Force {
public:
    static sf::Vector2f calcGravForce(float node1Mass, sf::Vector2f node1Pos, float node2Mass, sf::Vector2f node2Pos);       
};

#endif