#ifndef QUADTREE_H
#define QUADTREE_H
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Body.h"
#include <cmath>
#include "Force.h"

class Body;

class QuadTreeNode{
public: 
    //Represents the region of space this node covers
    class Region {
        public:
            sf::Vector2f center;
            float width;
            float mass = 0.0f;
            sf::Vector2f centerOfMass;
            int regionID;

            Region(sf::Vector2f centerPos, float regionWidth, int regionID);

            bool contains(float x, float y) const;

            Region getNorthWest() const;
            Region getNorthEast() const;
            Region getSouthWest() const;
            Region getSouthEast() const;
    };

    //Node properties
    Region region;
    std::vector<Body> bodies;
    int maxCapacity;
    bool isLeaf = true;

    //Child Nodes
    std::unique_ptr<QuadTreeNode> northWest;
    std::unique_ptr<QuadTreeNode> northEast;
    std::unique_ptr<QuadTreeNode> southWest;
    std::unique_ptr<QuadTreeNode> southEast;

    //Static counter for total nodes
    static int totalNodeCount;

    //Subdivide the current node into four child nodes
    void subdivide();

    //Recalculate the center of mass
    void updateCenterOfMass();

    public:
        //Constructor
        QuadTreeNode(Region nodeRegion, int capacity);

        //Insert a body into the tree
        void insert(const Body& body);

        //Calculate and apply gravitational forces
        void calculateForces(Body& targetBody, float theta = 0.5f);

        //Propagate the COM(masses and positions) through the nodes
        void propagate();

        //Static method to get total node count(including empty nodes)
        static int getTotalNodeCount();

        void printStats();

        void draw(sf::RenderWindow& window);
};

#endif