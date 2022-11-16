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
    Node() : pos(UNSET_PT), parentPos(UNSET_PT), gCost(UNSET_VAL), fCost(UNSET_VAL)
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
        // TODO: make configurable function to determine which heuristic to use
        const double hCost = pos.distance(goal);
        gCost = parentGCost + 1.0;
        fCost = gCost + hCost;
    }

    Point pos;
    Point parentPos;

    // gCost, fCost
    double gCost;
    double fCost;

    private:
};

class AStar
{
    public:
    // TODO: provide a heuristic function to enable experimenting with different
    // approaches.
    explicit AStar(const ConfigurationSpace& cSpace) : m_cSpace(cSpace)
    {
        // do nothing
    }

    /**
     * @brief Perform path-finding using the A* algorithm. The below implementation follows
     * the descriptions provided at the following links:
     *   https://en.wikipedia.org/wiki/A*_search_algorithm
     *   https://www.simplilearn.com/tutorials/artificial-intelligence-tutorial/a-star-algorithm#algorithm
     *   https://www.geeksforgeeks.org/a-search-algorithm/
     * 
     * @param start The start location
     * @param goal The goal location
     * @return std::vector<Point> The cell locations making up the path, ordered from start to goal, if found.
     * Otherwise, an empty vector is returned.
     */
    std::vector<Point> searchPath(const Point& start, const Point& goal) const
    {
        // Check for blocked / unreachable start and goal positions, or start is at the goal.
        // Chose not to throw an exception to allow program to continue with new user-provided inputs
        if (!isValidStartGoal(start, goal)) {
            return std::vector<Point>();
        }

        // Use a priority queue as a min-heap with smallest f-cost at the top for
        // storing the unexplored (open) nodes
        auto comp = [&](const CostPoint& c1, const CostPoint& c2) {
                return c1.first > c2.first;
        };
        std::priority_queue<CostPoint, std::vector<CostPoint>, decltype(comp)> unexploredNodes(comp);

        // Container storing the current state of each node in the map 
        const std::pair<size_t, size_t> mapShape = m_cSpace.shape();
        DataMap<Node> nodeMap(mapShape);

        // Map tracking which nodes have been explored in the search (closed)
        // TODO: consider replacing this with unordered_set
        DataMap<bool> exploredNodes(mapShape, false);

        // Put starting node on the open list (with fCost = 0 and gCost = 0)
        Node startNode(start, start);
        startNode.fCost = 0.0;
        startNode.gCost = 0.0;
        nodeMap.at(start) = startNode;

        CostPoint startCP{startNode.fCost, start};
        unexploredNodes.emplace(startCP);

        while (!unexploredNodes.empty()) {
            // Next search node 'q' is the node with lowest fCost from the heap
            CostPoint q = unexploredNodes.top();
            const Point qPos = q.second;
            // Remove q from the top of the heap and add it to the explored nodes
            unexploredNodes.pop();
            exploredNodes.at(qPos) = true;

            // Get all of the current node's accessible neighbors.
            // There are 8 max possible neighbors, but may be less if near
            // the border or within an obstacle
            const std::vector<Point> nbrPts = m_cSpace.getAccessibleNbrs(qPos);
            for (const auto& nbrPt : nbrPts) {
                // Check if this neighbor is the goal, updates its state, and
                // return a path
                if (nbrPt == goal) {
                  std::cout << "Goal found!!!" << std::endl;
                  Node nbr(nbrPt, qPos);
                  nodeMap.at(nbr.pos) = nbr;
                  return generatePath(nodeMap, goal);
                }

                // Explore this neighbor if we haven't already
                if (!exploredNodes.at(nbrPt)) {
                    Node nbr(nbrPt, qPos);
                    const double parentGCost = nodeMap.at(qPos).gCost;
                    nbr.updateCosts(goal, parentGCost);

                    // Add this neighbor to the unexplored nodes
                    // if not on the open list, add to open list, and set current cell as the parent
                    //         OR
                    // if on the open list, check if has a smaller f
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

    private:
    ConfigurationSpace m_cSpace;

    /**
     * @brief Check the start and goal positions to ensure they are valid. Incalid
     * cases include:
     *  - Start is outside of the domain
     *  - Goal is outside of the domain
     *  - Start is not accessible
     *  - Goal is not accessible
     *  - Start is already at the goal
     * 
     * @param start The start position
     * @param goal The goal position
     * @return false if one of the above conditions, else true
     */
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

        if (start == goal) {
            std::cout << "Start position " << start << " is already at goal" << std::endl;
            return false;
        }
      return true;
    }

    /**
     * @brief Generate the path followed from start to goal
     * 
     * @param nodeMap The node map containing node data at each grid point
     * @param goal The goal location
     * @return std::vector<Point> All points followed from start to goal
     */
    static std::vector<Point> generatePath(const DataMap<Node>& nodeMap, const Point& goal)
    {
      std::vector<Point> path;

      // Generate the path, working backwards from the goal
      // node, and terminating when we reach the start location
      path.emplace_back(goal);
      Point next = nodeMap.at(goal).parentPos;
      do {
        path.emplace_back(next);
        next = nodeMap.at(next).parentPos;
      } while (next != nodeMap.at(next).parentPos);

      // Reverse the order to go from start to goal
      std::reverse(path.begin(), path.end());
      return path;
    }
};