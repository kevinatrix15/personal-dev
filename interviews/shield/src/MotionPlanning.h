#pragma once

#include "Point.h"
#include "Map.h"

#include <limits>
#include <iostream>
#include <queue>

const double UNSET_VAL = std::numeric_limits<double>::max();
const size_t UNSET_IDX = std::numeric_limits<size_t>::max();

// stores {fCost, Point}
using CostPoint = std::Pair<double, Point>;

struct Node
{
    public:
    // NOTE: we initialize the parent position to itself
    Node() : pos(UNSET_IDX, UNSET_IDX), parentPos(UNSET_IDX, UNSET_IDX), gCost(UNSET_VAL), hCost(UNSET_VAL), fCost(UNSET_VAL)
    {
        // do nothing
    }

    Node(const Point& p, const Point& parentPos) :
        pos(p), parentPos(parentPos)
    {
        // do nothing
    }

    void updateCosts(const Point& goal, const double parentGCost)
    {
        gCost = parentGCost + 1.0;
        hCost = pos.distance(goal)
        fCost = gCost + hCost
    }

    Point pos;
    Point parentPos;

    // gCost, hCost, fCost
    double gCost;
    double hCost;
    double fCost;

    private:
};

template<typename T>
class DataMap : public GridIndexer
{
    public:
    DataMap(const std::pair<size_t, size_t>& shape) : GridIndexer(shape), m_data(size())
    {
        // do nothing
    }

    DataMap(const std::pair<size_t, size_t>& shape, const T& initVal) :
        GridIndexer(shape), m_data(size(), initVal)
    {
        // do nothing
    }

    T operator()(const size_t xIdx, const size_t yIdx) const
    {
        return m_data[GridIndexer::(xIdx, yIdx)];
    }

    T fromPoint(const Point& p) const
    {
        return (p.x(), p.y());
    }

    T& operator()(const size_t xIdx, const size_t yIdx)
    {
        return m_data[GridIndexer::(xIdx, yIdx)];
    }

    T& fromPoint(const Point& p)
    {
        return (p.x(), p.y());
    }

    private:
    std::vector<T> m_data;
};

class AStar
{
    public:
    explicit AStar(const ConfigurationSpace& cSpace) : m_cSpace(cSpace)
    {
        // do nothing
    }

    std::vector<Point> searchPath(const Point& start, const Point& goal) const
    {
        // helper lambdas
        auto isGoal{ [&](const Point& p) { return goal == p; } };

        auto calcHCost{ [&](const CostPoint& p) { return p.second.distance(goal); } };

        // choosing not to throw an exception to allow program to continue with
        // new user-provided inputs
        if (!isValidStartGoal(start, goal)) {
            return std::vector<Point>();
        }

        if (isGoal(start)) {
            std::cout << "Start position " << start << " is already at goal" << std::endl;
            return std::vector<Point>();
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
        std::priority_queue<CostPoint, std::vector<CostPoint>,
            [](const CostPoint& c1, const CostPoint& c2) {return c1.first < c2.first}> unexploredCostPoints;

        DataMap nodeMap<Node>(m_cSpace.shape());
        Point pCurr;
        for (size_t yIdx = 0; yIdx < nodeMap.numY(); ++yIdx) {
            for (size_t xIdx = 0; xIdx < nodeMap.numX(); ++xIdx) {
                pCurr = Point(xIdx, yIdx);
                // NOTE: we initialize each node with its parent position set to itself
                nodeMap(xIdx, yIdx) = Node(pCurr, pCurr);
            }
        }

        // Map indicating tracking which nodes have been explored in the search
        DataMap exploredNodes<bool>(m_cSpace.size(), false);

        // - put starting node on the open list (with f = 0)
        // NOTE: startNode's fCost = 0.0
        Node startNode = nodeMap.fromPoint(start);
        CostPoint startCP{startNode.fCost, start};
        unexploredNodes.emplace(startCP);

        while (!unexploredNodes.empty()) {
            CostPoint q = unexploredNodes.top();
            const qFCost = q.first;
            const qPos = q.second;
            unexploredNodes.pop();
            exploredNodes(qPos) = true;
            // m_cSpace.getAccessibleNbrs(q.second);
            const std::vector<Point> nbrPts = m_cSpace.getAccessibleNbrs(qPos);
            for (const auto& nbrPt : nbrPts) {
                if (nbrPt == goal) {
                    // create path
                    // return
                }
                // If we haven't explored this point yet
                if (!exploredNodes(nbrPt)) {
                    Node nbr(nbrPt, qPos);
                    const double parentGCost = nodeMap(qPos).gCost;
                    nbr.updateCosts(goal, parentGCost);

                    // if not on the open list, add to open list, and set current cell as the parent
                    // nbr.parent = q.second
                    //         OR
                    // if on the open list, check if has a lower f
                    if (nodeMap(nbr.pos).fCost == UNSET_VAL ||
                        nodeMap(nbr.pos).fCost > nbr.fCost) {
                            unexploredNodes.emplace({nbr.fCost, nbr.pos});
                            nodeMap(nbr.pos) = nbr;
                    }
                }
            }

        }
    }
    // Data structure considerations:
    // Closed list
    // operations:
    // - check if a node with same location (index) has a smaller f
    //      * implies storing node data, including at least position and f
    //
    // Open list
    // operations:
    // - readily get the node with smallest 'f'
    // - add children (emplace())
    // - remove top candidate (pop())
    // - check for other nodes with same position, compare whether have smaller 'f'

    // operations:
    // initialize all nodes in nodeMap with invalid (UNSET) values

    // set nodeMap(start):
    //  f = 0
    //  g = 0
    //  h = 0
    //  parent = start (x(), y())

    // put starting cell in unexploredNodes (f = 0)

    // while (!unexploredNodes.empty()) {
        // CostPoint q = unexploredNodes.top();
        // unexploredNodes.pop();
        // exploredNodes(q.second) = true;
        // m_cSpace.getAccessibleNbrs(q.second);
        // if (isDestination) {
            // create path
        // }
        // if (!exploredNodes(q.second)) {
            // nbr.g = q.g + 1.0
            // nbr.h = nbr.pos.distance(goal)
            // nbr.f = nbr.g + nbr.h

            // if not on the open list, add to open list, and set current cell as the parent
            // nbr.parent = q.second
            //         OR
            // if on the open list, check if has a lower f
            // if (nodeMap(nbr.pos) == UNSET || nodeMap(nbr.pos) > nbr.f) {
                // unexploredNodes.emplace(nbr.f, nbr.pos);

                // update the nodeMap
                // nodeMap(nbr.pos) = nbr;
            // }
        // }

    // }

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