#include "QuadTree.h"
#include <iostream>

//Init static member
int QuadTreeNode::totalNodeCount = 1;

//Region Constructor
QuadTreeNode::Region::Region(sf::Vector2f centerPos, float regionWidth, int regionID)
    : center(centerPos), width(regionWidth), regionID(regionID) {}

//Check is a point is contained within this region
bool QuadTreeNode::Region::contains(float x, float y) const {
    float halfWidth = width / 2.0f;
    return (x >= center.x - halfWidth &&
            x < center.x + halfWidth &&
            y >= center.y - halfWidth &&
            y < center.y + halfWidth);
}

//Get quadrant regions
QuadTreeNode::Region QuadTreeNode::Region::getNorthWest() const {
    float quarterWidth = width / 4.0f;
    return Region(
        sf::Vector2f(center.x - quarterWidth, center.y - quarterWidth),
        width / 2.0f, 1
    );
}

QuadTreeNode::Region QuadTreeNode::Region::getNorthEast() const {
    float quarterWidth = width / 4.0f;
    return Region(
        sf::Vector2f(center.x + quarterWidth, center.y - quarterWidth), 
        width / 2.0f, 2 
    );
}

QuadTreeNode::Region QuadTreeNode::Region::getSouthWest() const {
    float quarterWidth = width / 4.0f;
    return Region(
        sf::Vector2f(center.x - quarterWidth, center.y + quarterWidth), 
        width / 2.0f, 3
    );
}

QuadTreeNode::Region QuadTreeNode::Region::getSouthEast() const {
    float quarterWidth = width / 4.0f;
    return Region(
        sf::Vector2f(center.x + quarterWidth, center.y + quarterWidth), 
        width / 2.0f, 4
    );
}

//QuadTreeNode Constructor
QuadTreeNode::QuadTreeNode(Region nodeRegion, int capacity) 
    : region(nodeRegion), maxCapacity(capacity) {
        totalNodeCount++;
}

//Subdivide
void QuadTreeNode::subdivide() {
    northWest = std::make_unique<QuadTreeNode>(region.getNorthWest(), maxCapacity);
    northEast = std::make_unique<QuadTreeNode>(region.getNorthEast(), maxCapacity);
    southWest = std::make_unique<QuadTreeNode>(region.getSouthWest(), maxCapacity);
    southEast = std::make_unique<QuadTreeNode>(region.getSouthEast(), maxCapacity);

    isLeaf = false;
    totalNodeCount += 4;

    std::vector<Body> existingBodies = std::move(bodies);
    bodies.clear();

    for (const auto& body : existingBodies) {
        insert(body);
    }
}

//Insert a body into the tree
void QuadTreeNode::insert(const Body& body) {
    if (!region.contains(body.getPosition().x, body.getPosition().y)) {
        return;
    }

    if (isLeaf && bodies.size() < maxCapacity) {
        bodies.push_back(body);
        propagate();
        return;
    }

    if (isLeaf) {
        subdivide();
    }

    northWest->insert(body);
    northEast->insert(body);
    southWest->insert(body);
    southEast->insert(body);
}

void QuadTreeNode::calculateForces(Body& targetBody, float theta) {
    if (region.mass == 0) return;

    sf::Vector2f bodyPos = targetBody.getPosition();
    sf::Vector2f regionCOM = region.centerOfMass;
    
    float distance = std::sqrt(((regionCOM.x - bodyPos.x)*(regionCOM.x - bodyPos.x)) +
                            ((regionCOM.y - bodyPos.y)*(regionCOM.y - bodyPos.y)));

    float s = region.width;
    
    if (isLeaf || (s / distance) < theta) {
        sf::Vector2f force = Force::calcGravityForce(
            targetBody.getMass(), 
            targetBody.getPosition(),
            region.mass,
            region.centerOfMass
        );
        
        targetBody.applyForce(force);
        return;
    }

    if (northWest) northWest->calculateForces(targetBody, theta);
    if (northEast) northEast->calculateForces(targetBody, theta);
    if (southWest) southWest->calculateForces(targetBody, theta);
    if (southEast) southEast->calculateForces(targetBody, theta);
}

void QuadTreeNode::propagate(){
    region.mass = 0.0f;
    region.centerOfMass = sf::Vector2f(0.0f, 0.0f);

    if (isLeaf && !bodies.empty()) {
        for (const auto& body : bodies) {
            region.mass += body.getMass();
            region.centerOfMass += body.getPosition() * body.getMass();
        }

        if (region.mass > 0) {
            region.centerOfMass /= region.mass;
        }
        return;
    }

    if (!isLeaf) {
        float totalMass = 0.0f;
        sf::Vector2f massWeightedPosition(0.0f, 0.0f);

        std::vector<std::unique_ptr<QuadTreeNode>*> children = {
            &northWest, &northEast, &southWest, &southEast
        };

        for (auto* child : children) {
            if(*child) {
                (*child)->propagate();

                totalMass += (*child)->region.mass;
                massWeightedPosition += (*child)->region.centerOfMass * (*child)->region.mass;
            }
        }

        region.mass = totalMass;
        if (totalMass > 0) {
            region.centerOfMass = massWeightedPosition / totalMass;
        }
    }
}

int QuadTreeNode::getTotalNodeCount() {
    return totalNodeCount;
}

void QuadTreeNode::printStats() {
    if (region.regionID == 1) {
        std::cout << "Mass of Quadrant 1: " << region.mass << std::endl;
    }

    if (northWest) northWest->printStats();
    if (northEast) northEast->printStats();
    if (southWest) southWest->printStats();
    if (southEast) southEast->printStats();
}


void QuadTreeNode::drawRegionLines(sf::RenderWindow& window) {
    sf::RectangleShape boundary;
    boundary.setPosition(
        sf::Vector2f(
            region.center.x - region.width / 2.0f, 
            region.center.y - region.width / 2.0f
        )
    );

    boundary.setSize(sf::Vector2f(region.width, region.width));
    boundary.setFillColor(sf::Color::Transparent);


    boundary.setOutlineColor(sf::Color(0, 255, 0, 60));
    boundary.setOutlineThickness(0.5f);
    
    window.draw(boundary);

    if (!isLeaf) {
        northWest->drawRegionLines(window);
        northEast->drawRegionLines(window);
        southWest->drawRegionLines(window);
        southEast->drawRegionLines(window);
    }
}