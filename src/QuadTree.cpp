#include "QuadTree.h"
#include <iostream>

QuadTreeNode::Region::Region(sf::Vector2f centerPos, float regionWidth, int regionID)
    : center(centerPos), width(regionWidth), regionID(regionID) {}

// Check is a point is contained within this region
bool QuadTreeNode::Region::contains(float x, float y) const {
    float halfWidth = width / 2.0f;
    return (x >= center.x - halfWidth &&
            x < center.x + halfWidth &&
            y >= center.y - halfWidth &&
            y < center.y + halfWidth);
}

// Get quadrant regions
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
}

void QuadTreeNode::subdivide() {
    // create four new child nodes by sibdividing the current region
    northWest = std::make_unique<QuadTreeNode>(region.getNorthWest(), maxCapacity);
    northEast = std::make_unique<QuadTreeNode>(region.getNorthEast(), maxCapacity);
    southWest = std::make_unique<QuadTreeNode>(region.getSouthWest(), maxCapacity);
    southEast = std::make_unique<QuadTreeNode>(region.getSouthEast(), maxCapacity);

    // mark current node as not a leaf
    isLeaf = false;

    // move all the existing bodies from current node to temp vector
    std::vector<Body> existingBodies = std::move(bodies);
    // remove bodies in we arent a leaf
    bodies.clear();

    // reinsert the bodies into the child nodes
    for (const auto& body : existingBodies) {
        insert(body);
    }
}

void QuadTreeNode::insert(const Body& body) {
    // if body's position is not within this node's region, skip
    if (!region.contains(body.getPosition().x, body.getPosition().y)) return;

    // if current node is lead and has more space
    if (isLeaf && bodies.size() < maxCapacity) { 
        bodies.push_back(body); // ass the body to the list of bodies within the region
        propagate(); // propagate this region's COM
        return;
    }

    // if current node is leaf but is at capacity
    if (isLeaf && bodies.size() >= maxCapacity) { 
        subdivide(); 
    }

    // recursively try to insert the body into current region's children
    northWest->insert(body);
    northEast->insert(body);
    southWest->insert(body);
    southEast->insert(body);
}

void QuadTreeNode::calculateForces(Body& targetBody, float theta) {
    // if region has no mass, no force calc needed
    if (region.mass == 0) return;

    // get the position of target body and COM of this region
    sf::Vector2f bodyPos = targetBody.getPosition();
    sf::Vector2f regionCOM = region.centerOfMass;
    
    // calculate the distance betweent the target body and the region's COM
    float distance = std::sqrt(((regionCOM.x - bodyPos.x)*(regionCOM.x - bodyPos.x)) +
                            ((regionCOM.y - bodyPos.y)*(regionCOM.y - bodyPos.y)));

    // get width of region
    float s = region.width;
    
    // if region is lead or is ratio is less than theta
    if (isLeaf || (s / distance) < theta) {
        // compute the grav force on the target body from COM 
        sf::Vector2f force = Force::calcGravityForce(
            targetBody.getMass(), 
            targetBody.getPosition(),
            region.mass,
            region.centerOfMass
        );
        
        // apply the calculated force
        targetBody.applyForce(force);
        return;
    }

    // if the region is not a leaf and the condition for approximation if not met,
    // recurse into the four child node to calc force from each child
    if (northWest) northWest->calculateForces(targetBody, theta);
    if (northEast) northEast->calculateForces(targetBody, theta);
    if (southWest) southWest->calculateForces(targetBody, theta);
    if (southEast) southEast->calculateForces(targetBody, theta);
}

void QuadTreeNode::propagate(){
    // init mass and com to zero for region
    region.mass = 0.0f;
    region.centerOfMass = sf::Vector2f(0.0f, 0.0f);

    // if this node is a leaf and contains bodies
    if (isLeaf && !bodies.empty()) {
        // loop through each body in this node's region and add pos and weight to region COM
        for (const auto& body : bodies) {
            region.mass += body.getMass();
            region.centerOfMass += body.getPosition() * body.getMass();
        }

        // if the region has mass, calc COM
        if (region.mass > 0) {
            region.centerOfMass /= region.mass;
        }
        return;
    }

    // if node is not leaf, calc mass and COM recursivly 
    if (!isLeaf) {
        float totalMass = 0.0f;
        sf::Vector2f massWeightedPosition(0.0f, 0.0f);

        std::vector<std::unique_ptr<QuadTreeNode>*> children = {
            &northWest, &northEast, &southWest, &southEast
        };

        // loop through each child node and propagate the COM calculation
        for (auto* child : children) {
            if(*child) {
                (*child)->propagate();

                totalMass += (*child)->region.mass;
                massWeightedPosition += (*child)->region.centerOfMass * (*child)->region.mass;
            }
        }

        // set region's total mass to total mass
        region.mass = totalMass;

        // if the mass > 0, calc COM
        if (totalMass > 0) {
            region.centerOfMass = massWeightedPosition / totalMass;
        }
    }
}

void QuadTreeNode::drawRegionLines(sf::RenderWindow& window) {
    // create a rectangle shape to represent the boundary of this region
    sf::RectangleShape boundary;

    // set pos of rectangle to be top-left corner of this region
    boundary.setPosition(
        sf::Vector2f(
            region.center.x - region.width / 2.0f, 
            region.center.y - region.width / 2.0f
        )
    );

    // set the size of the rectangle to represent the width of the region
    boundary.setSize(sf::Vector2f(region.width, region.width));

    // set the rectangle to be transparent
    boundary.setFillColor(sf::Color::Transparent);

    // set the rectangle's border color and tickness
    boundary.setOutlineColor(sf::Color(0, 255, 0, 60));
    boundary.setOutlineThickness(0.5f);
    
    // actually draw border
    window.draw(boundary);

    // recursivly call draw border for child is we aren't a leaf
    if (!isLeaf) {
        northWest->drawRegionLines(window);
        northEast->drawRegionLines(window);
        southWest->drawRegionLines(window);
        southEast->drawRegionLines(window);
    }
}