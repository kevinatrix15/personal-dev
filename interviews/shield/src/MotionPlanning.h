#pragma once

#include "Point.h"
#include "Map.h"

#include <iostream>

struct Node
{
    public:
    Node() {

    }

    // gCost, hCost, fCost
    size_t gCost;
    size_t hCost;
    size_t fCost;

    Point pos;
    Node* parent;
};

class AStar
{
    public:
    explicit AStar(const ConfigurationSpace* cSpace) : m_cSpace(cSpace)
    {
        // do nothing
    }

    std::vector<Point> plan(const Point& start, const Point& goal)
    {
        // choosing not to throw an exception to allow program to continue with
        // new inputs
        if (!m_cSpace.isValid(start)) {
            std::cout << "Start point " << start << " is not valid" << std::endl;
            return std::vector<Point>();
        }
        if (!m_cSpace.isValid(goal)) {
            std::cout << "Goal point " << start << " is not valid" << std::endl;
            return std::vector<Point>();
        }

        // NOTE / WARNING: watch out for euclidean distance on ints with flooring of diagonal distance calculation

        // examples
        // https://codereview.stackexchange.com/questions/97834/a-algorithm-in-c
        // https://dev.to/jansonsa/a-star-a-path-finding-c-4a4h

        // https://www.simplilearn.com/tutorials/artificial-intelligence-tutorial/a-star-algorithm#algorithm
        // algorithm:
        // - initialize the open and closed list
        // - put starting node on the open list (with f = 0)
        // - while !openList.empty():
        // 1. find the node with least f on the open list and name it 'q'
        // 2. remove q from the open list
        // 3. produce q's 8 descendents and set q as their parent
        // 4. for each descendent:
        //      - if successor is the goal, stop
        //      - else calculate g and h for successor:
        //      successor.g = q.g + q.distance(successor)
        //      successor.h = successor.distance(goal) // assuming euclidean for now
        //      successor.f = successor.g + successor.h
        //      - skip this successor if a node in the open list with same location has a lower f
        //      - skip this successor if a node in the closed list with same location has a lower f
        //      - Otherwise, add successor to the open list
        // - push q into the closed list and end the while loop

    }

    private:
    ConfigurationSpace m_cSpace;
};