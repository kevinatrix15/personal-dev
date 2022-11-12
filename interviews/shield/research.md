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
