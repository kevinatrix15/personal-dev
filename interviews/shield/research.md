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

# References:
* [Wiki- Motion Planning](https://en.wikipedia.org/wiki/Motion_planning)
* 

# To look at next:
* [Forward kinematics](https://en.wikipedia.org/wiki/Forward_kinematics)- position of robot geometry
* [Collision detection](https://en.wikipedia.org/wiki/Collision_detection) 
* Watchdog timer- prevents infinite searching

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

# Design:

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