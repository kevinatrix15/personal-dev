Path Planning Algorithms to consider:
* [Dijkstra's Algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm)- 
  * Implemented with a min priority queue
  * May be further optimized with Fibonacci heap min-priority queue
  * Complexity: O((|V| + |E|)log|V|)
    * With Fibonacci, reduces to O(|E| + |V|log|V|)- asymptotically fastest known single source shortest-path algorithm for arbitrary directed graphs.

Other notes:

Common notation:
* Start denoted as 'S', goal as 'G'
* Working field is the 'workspace'
* C is the configuration space
* Set of configurations that avoids collision with obstacles is C_free
* Free space- Set of configurations that avoids collisions with obstacles
* Target space- subset of free space where we want the robot to move to
* Obstacle space- space the robot can not move. Not opposite of free space

[More on Task Space, Work Space, and Configuration Space:](https://www.mecharithm.com/task-space-and-workspace-for-robots/)
* Task space- space in which robot's task is naturally expressed.
  * Should only have to know about the task and not the robot.
  * Possible robot can't reach some configurations.
* Work space- specification of the reachable configurations of the end-effector.
  * Nothing to do with a particular task.
* Configuration Space
  * May be represented with explicit parameterization (minimum number of coordinates e.g., to represent a sphere), or
  * Implicit representation- expressing in Euclidean space

# References:
* [Wiki- Motion Planning](https://en.wikipedia.org/wiki/Motion_planning)
* 

# To look at next:
* [Forward kinematics](https://en.wikipedia.org/wiki/Forward_kinematics)- position of robot geometry
* [Collision detection](https://en.wikipedia.org/wiki/Collision_detection) 
* Watchdog timer- prevents infinite searching
* [VL](https://www.visualizationlibrary.org/docs/2.1/html/index.html)- look into for visualizing results
* [functional programming](https://learn.microsoft.com/en-us/archive/msdn-magazine/2012/august/c-functional-style-programming-in-c)

# Requirements:
Inputs:
* Map size (m, n)
* List of obstacles, each with a radius and center location
* Start position and target position
* Robot radius

## Actions:
* Create the map with obstacles
* Include start position and target
* Save map to a file
* Load map from file
* Find 'best' safe path 
* Visualize the path to the screen

# Approach 1:

## Components:

### Map Generation
Map:
- M, N (map size, pixels)
- vector<CircularObstacle>
- Path- owns and operates on it???
+ write()- writes the map to disk

Path
- Map - passed to ctor
- start (Point)
- goal (Point)
+ seek()- return sequence of Points 

CircularObstacle
- Center (Point)
- Radius (int)

Point
- (x, y)
+ distance(Point other)- euclidian distance calculation

MapViewer
- Map- constructed with
- Path- optimal path
+ viewConfigSpace()- displays radial distance to nearest obstacles

ObstacleDistance
+ ctor- perform BFS working outside in (or some other approach to assigning)
+ update(vector<Circle>& obstacles)- perform BFS, only appending points
to the queue if distance is lower than existing
- Consider having two distance detectors- edge and obstacle. Maybe give each a shared_ptr to
the Map's distance map.

Logger???


# Approach 2:
Idea: rather than dealing with distance checking and assignment, let's instead simply extend each obstacle by the robot's radius, and represent the robot as as point.

## Algorithm / steps:
* Create obstacle-free C-Space, given size of domain and robot radius
  * Use binary representation to mark points as out-of-bounds near domain edges with robot radius
  * Have different values to represent actual obstacles vs. padded out-of-bounds cells
* Add obstacles
  * Fill in obstacles with obstacle value
  * Add padding around obstacles with padding value
* Search for best safe path


# Visualization
* Simple- write data to stdout, then use gnuplot to plot: https://stackoverflow.com/questions/15844153/best-c-library-to-display-an-image-from-a-2d-array


# TODO:
1. Write map and path to file, and read back in
2. Clean up MotionPlanning A* implementation
4. Rename Point to Cell
6. Perform profiling to identify bottlenecks

5. Rearrange files to be more cohesive
3. Make slides
7. Unit test critical classes
8. Add function & class documentation / README
9. Construct AStar class with custom heuristic function, and run a few comparison cases
   1.  Manhattan distance, diagonal distance, euclidean distance, h = 0 (Djikstra)?


## Presentation Brainstorming:

### Possible topics to cover:
* Problem description:
  * Static configuration space
  * Dimensionaltiy of the c-space
    * 2D robot
* Collision avoidance approach:
  * Rather than determining the local distance to the nearest obstacle for each cell, chose to simply add padding to all obstacles based on the robot radius and consider distance in the search algorithm
  * TODO: add padding separately around obstacles to enable differentiation with visualization
* Reasoning behind Selection of A* compared to others
* Selection of data structures with performance estimations

* Testing
  * Scenario I- no obstacles
  * Scenario II- non-existent path
  * Scenario III- simple path, following diagonal
  * Scenario IV- more complex path 
  * Scenario V- random obstacle generation
* Performance evaluation:
  * Profiling output- what are the bottlenecks, and how might I address them?
  * Scalability-
    * add timing output
    * Take Scenario V above, and scale all dimensions across various scales
    * Report average time to run each case (repeat each ~3-5 times)
  * Heuristic selection- provide means for specifying which heuristic to use
    * h = 0 for Dijikstra
    * h = diagonal, euclidean, manhattan
    * Repeat scalability study above
* Live Demo

* Class design
  * Interaction diagram
  * UML diagram
* Highlight designs that helped improve expressiveness / readability / flexibility
  * GetActiveNeighbors
  * GridIndexer-
    * easily represent 2D points on 1D vector, ensuring better cache performance without having to explicitly calculate 1D index from 2D
    * Ability to index off of x, y, or Point
  * DataMap- using GridIndexer

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
