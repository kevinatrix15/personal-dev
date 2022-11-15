#pragma once

#include "Point.h"
#include "Map.h"

#include <limits>
#include <iostream>
#include <queue>

constexpr double UNSET_VAL = std::numeric_limits<double>::max();
constexpr size_t _UNSET_IDX = std::numeric_limits<size_t>::max();
const Point UNSET_PT(_UNSET_IDX, _UNSET_IDX);

// stores {fCost, Point}
using CostPoint = std::pair<double, Point>;

struct Node
{
    public:
    // NOTE: we initialize the parent position to itself
    Node() : pos(UNSET_PT), parentPos(UNSET_PT), gCost(UNSET_VAL), hCost(UNSET_VAL), fCost(UNSET_VAL)
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
        hCost = pos.distance(goal);
        fCost = gCost + hCost;
    }

    Point pos;
    Point parentPos;

    // gCost, hCost, fCost
    double gCost;
    double hCost;
    double fCost;

    private:
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
        // choosing not to throw an exception to allow program to continue with
        // new user-provided inputs
        if (!isValidStartGoal(start, goal)) {
            return std::vector<Point>();
        }

        if (start == goal) {
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
        auto comp = [&](const CostPoint& c1, const CostPoint& c2) {
                return c1.first < c2.first;
        };
        std::priority_queue<CostPoint, std::vector<CostPoint>, decltype(comp)> unexploredNodes(comp);

        const std::pair<size_t, size_t> mapShape = m_cSpace.shape();
        DataMap<Node> nodeMap(mapShape);

        // Map indicating tracking which nodes have been explored in the search
        DataMap<bool> exploredNodes(mapShape, false);

        // - put starting node on the open list (with f = 0)
        // NOTE: startNode's fCost = 0.0
        Node startNode(start, start);
        nodeMap.at(start) = startNode;

        CostPoint startCP{startNode.fCost, start};
        unexploredNodes.emplace(startCP);

        while (!unexploredNodes.empty()) {
            CostPoint q = unexploredNodes.top();
            const double qFCost = q.first;
            const Point qPos = q.second;
            unexploredNodes.pop();
            exploredNodes.at(qPos) = true;
            // m_cSpace.getAccessibleNbrs(q.second);
            const std::vector<Point> nbrPts = m_cSpace.getAccessibleNbrs(qPos);
            for (const auto& nbrPt : nbrPts) {
                if (nbrPt == goal) {
                  std::cout << "Goal found!!!" << std::endl;
                  Node nbr(nbrPt, qPos);
                  nodeMap.at(nbr.pos) = nbr;
                  return generatePath(nodeMap, goal);
                }
                // If we haven't explored this point yet
                if (!exploredNodes.at(nbrPt)) {
                    Node nbr(nbrPt, qPos);
                    const double parentGCost = nodeMap.at(qPos).gCost;
                    nbr.updateCosts(goal, parentGCost);

                    // if not on the open list, add to open list, and set current cell as the parent
                    // nbr.parent = q.second
                    //         OR
                    // if on the open list, check if has a lower f
                    if (nodeMap.at(nbr.pos).fCost == UNSET_VAL ||
                        nodeMap.at(nbr.pos).fCost > nbr.fCost) {
                            unexploredNodes.emplace(std::make_pair(nbr.fCost, nbr.pos));
                            nodeMap.at(nbr.pos) = nbr;
                    }
                }
            }

        }
        std::cout << "Goal not found... :(" << std::endl;
        return std::vector<Point>();
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

    // put starting cell in unexplored (f = 0)

    // while (!unexplored.empty()) {
        // CostPoint q = unexplored.top();
        // unexplored.pop();
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
                // unexplored.emplace(nbr.f, nbr.pos);

                // update the nodeMap
                // nodeMap(nbr.pos) = nbr;
            // }
        // }

    // }

    private:
    ConfigurationSpace m_cSpace;

    bool isValidStartGoal(const Point& start, const Point& goal) const
    {
        if (!m_cSpace.contains(start)) {
            std::cout << "Start point " << start << " is not in the grid" << std::endl;
            return false;
        }

        if (!m_cSpace.contains(goal)) {
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
      return true;
    }

    static std::vector<Point> generatePath(const DataMap<Node>& nodeMap, const Point& goal)
    {
      std::vector<Point> path;
      path.emplace_back(goal);
      Point next = nodeMap.at(goal).parentPos;
      do {
        path.emplace_back(next);
        next = nodeMap.at(next).parentPos;
      } while (next != nodeMap.at(next).parentPos);
      std::reverse(path.begin(), path.end());
      return path;
    }
};