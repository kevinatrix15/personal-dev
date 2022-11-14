#pragma once

#include "Point.h"
#include "Map.h"

#include <iostream>
#include <queue>

using WeightPair = std::Pair<double, Point>;

struct Node
{
    public:
    Node(const Point& p, const Point& goal, Node* parent) :
        position(p), parent(parent)
    {
        // assign the costs
        gCost = parent ? parent->gCost + 1.0 : 0.0;
        hCost = position.distance(goal);
        // TODO: does this need to be set to 0.0 for root node?
        fCost = gCost + hCost;
    }

    Point position;
    Node* parent;

    // gCost, hCost, fCost
    double gCost;
    double hCost;
    double fCost;

    private:
};

class AStar
{
    public:
    explicit AStar(const ConfigurationSpace* cSpace) : m_cSpace(cSpace)
    {
        // do nothing
    }

    std::vector<Point> searchPath(const Point& start, const Point& goal) const
    {
        // helper lambdas
        auto isGoal{ [&](const Point& p) { return goal == p; } };

        auto calcHCost{ [&](const WeightPair& p) { return p.second.distance(goal); } };

        // choosing not to throw an exception to allow program to continue with
        // new user-provided inputs
        if (!isValidStartGoal(start, goal)) {
            return std::vector<Point>();
        }

        if (isGoal(start)) {
            std::cout << "Start position " << start << " is already at goal" << std::endl;
        }

        // NOTE / WARNING: watch out for euclidean distance on ints with flooring of diagonal distance calculation

        // examples
        // https://codereview.stackexchange.com/questions/97834/a-algorithm-in-c
        // https://dev.to/jansonsa/a-star-a-path-finding-c-4a4h

        // https://www.geeksforgeeks.org/a-search-algorithm/
        // https://www.simplilearn.com/tutorials/artificial-intelligence-tutorial/a-star-algorithm#algorithm
        // algorithm:
        // - initialize the open and closed list
        // open- priority_queue<f, Point>, closed- bool hash table

        // use a priority queue as a min-heap with smallest f-weight at the top
        // open cells are the un-visited cells
        std::priority_queue<Node, std::vector<Node>,
            [](const Node& n1, const Node& n2) {return n1.fCost < n2.fCost}> unexploredNodes;
        // std::priority_queue<WeightPair, std::vector<WeightPair>, std::greater<WeightPair>> openCells;

        // TODO: create method to get cell's neighbors
        // TODO: create vector of Nodes
        std::vector<Node> mapData()
        // check if valid
            // check if destination

        // TODO: create a convenient way to access shape and do 2D->1D indexing
        std::vector<bool> exploredNodes(m_cSpace.size(), false);

        // - put starting node on the open list (with f = 0)
        Node startNode(start, goal, nullptr);
        unexploredNodes.emplace(startNode);
        // - while !openList.empty():
        while (!unexploredNodes.empty()) {
            // 1. find the node with smallest f on the open list and name it 'q'
            Node q = unexploredNodes.top(); 

            // 2. remove q from the open list and push into the closed
            unexploredNodes.pop();

            // 3. produce q's 8 descendents and set q as their parent
            // TODO: decide where to implement the getNeighbors()
            // TODO: as part of this, set the parent to q???
            // TODO: could this take into account accessible cells?
            std::vector<Point> childrenPts = m_cSpace.getAccessibleNbrs(q.position);

            // 4. for each descendent:
            for (const Point& pt : childrenPts) {
                //      - if successor is the goal, stop
                if (pt == goal) {
                    // update path
                    // return path
                }

                // create new node, calculating costs upon construction
                Node child(pt, goal, q);

                // TODO: below checks before adding to unexplored
                //      - skip this successor if a node in the open list with same location has a lower f
                //      - skip this successor if a node in the closed list with same location has a lower f
                //      - Otherwise, add successor to the open list
                unexploredNodes.emplace(child);
            }
            // - push q into the closed list and end the while loop
            exploredNodes(q.position.x(), q.position.y()) = true;
        }

    }

    private:
    ConfigurationSpace m_cSpace;

    bool isValidStartGoal(const Point& start, const Point& goal)
    {
        if (!m_cSpace.isInGrid(start)) {
            std::cout << "Start point " << start << " is not in the grid" << std::endl;
            return false;
        }

        if (!m_cSpace.isInGrid(goal)) {
            std::cout << "Goal point " << start << " is not in the grid" << std::endl;
            return false;
        }

        if (!m_cSpace.isAccessible(start)) {
            std::cout << "Start point " << start << " is not accessible" << std::endl;
            return false;
        }

        if (!m_cSpace.isAccessible(goal)) {
            std::cout << "Goal point " << goal << " is not accessible" << std::endl;
            return false;
        }

    }
};