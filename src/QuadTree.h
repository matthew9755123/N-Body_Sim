#ifndef QUADTREE_H
#define QUADTREE_H
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Body.h"
#include <cmath>
#include "Force.h"

class Body;

/**
 * @class QuadTreeNode
 * @brief Represents a node in the quadtree
 * 
 * @details Each node in the quadtree divides space into four quadrants and holds bodies within its region. The node 
 *          stores information about its center of mass, mass, and child nodes, and can recursively subdivide to 
 *          accommodate more bodies.
 */
class QuadTreeNode{
public:
    /**
     * @class Region
     * @brief Represents the rectangular region covered by the quad-tree node.
     *
     * @details The region is defined by a center point, width, and contains information about the mass and center of mass.
     */
    class Region {
        public:
            sf::Vector2f center; // center point of the region
            float width; // width (and height) of the region
            float mass = 0.0f; // total mass within the region
            sf::Vector2f centerOfMass; // position of region COM
            int regionID; // id for the region

            /**
             * @brief Constructor for the Region class
             * @param centerPos The center position of the region
             * @param regionWidth The width of the region
             * @param regionID The ID for the region
             */
            Region(sf::Vector2f centerPos, float regionWidth, int regionID);

            /**
             * @brief Checks if a point (x, y) lies within this region
             * @param x The x-coordinate of the point
             * @param y The y-coordinate of the point
             * @return true if the point is inside the region, otherwise false
             */
            bool contains(float x, float y) const;

            /**
             * Divides the region into four quadrants and returns the the respective quadrant
             */
            Region getNorthWest() const;
            Region getNorthEast() const;
            Region getSouthWest() const;
            Region getSouthEast() const;
    };

    //Node properties
    Region region; // region covered by this node
    std::vector<Body> bodies; // bodies contained wihtin this node's region
    int maxCapacity; // max num of bodies a leaf node will hold before subdividing
    bool isLeaf = true; // indicates is this is a leaf node

    //Pointers to child nodes
    std::unique_ptr<QuadTreeNode> northWest;
    std::unique_ptr<QuadTreeNode> northEast;
    std::unique_ptr<QuadTreeNode> southWest;
    std::unique_ptr<QuadTreeNode> southEast;

    /**
     * @brief Subdivides the current node into four child nodes
     * @details This method divides the node into four smaller regions and redistributes any bodies contained within the node
     */
    void subdivide();

    /**
     * @brief Recalculates the center of mass for the node based on the bodies it contains
     * @details This method updates the mass and center of mass for the node and propagates this information up to its parent nodes
     */
    void updateCenterOfMass();

    public:
        /**
         * @brief Constructs a new QuadTreeNode
         * @param nodeRegion The region that the node covers
         * @param capacity The maximum capacity of bodies this node can hold before subdividing
         */
        QuadTreeNode(Region nodeRegion, int capacity);

        /**
         * @brief Inserts a body into the quadtree node
         * @param body The body to insert
         */
        void insert(const Body& body);

         /**
         * @brief Calculates and applies gravitational forces on a target body
         * @param targetBody The body to calculate forces on
         * @param theta The threshold for when to stop subdividing (default: 0.5)
         */
        void calculateForces(Body& targetBody, float theta = 0.5f);

        /**
         * @brief Propagates the center of mass and mass through the tree
         * @details This method updates the center of mass and mass for all nodes in the tree 
         */
        void propagate();

        /**
        * @brief Draws the boundary lines of the regions in the quadtree
        * @param window The window to render the lines on
        */
        void drawRegionLines(sf::RenderWindow& window);
};

#endif