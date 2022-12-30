#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

struct Node
{
	int val;
	vector<Node> children;
	bool visited;

	Node(const int val) : val(val), children(), visited(false)
	{
	}
};

class Graph
{
public:
	vector<Node> nodes;

	Graph(const vector<Node> nodes) : nodes(nodes)
	{
	}

	void resetVisitedNodes()
	{
		for (auto& node : nodes) {
			node.visited = false;
		}
	}
};

class RouteFinder
{
public:
	// approach- search both nodes check whether they are connected
	bool doesRouteExist(const Node& a, const Node& b, Graph& g)
	{
		return (isRouteFound(a, b, g) || isRouteFound(b, a, g));
	}
private:
	bool isRouteFound(const Node& root, const Node& target, Graph& g)
	{
		g.resetVisitedNodes();

		queue<Node> q;
		q.emplace(root);

		while (!q.empty()) {
			Node top = q.front();
			if (top.val == target.val) {
				// TODO: add overloaded == operator to check for same
				// children as well
				return true;
			}
			q.pop();
			top.visited = true;
			for (const auto& c : top.children) {
				if (!c.visited) {
					q.emplace(c);
				}
			}
		}
		return false;
	}
};

int main()
{
	Node A(1);
	Node B(2);
	Node C(3);

	B.children = { C };
	A.children = { B };

	Graph g({ A, B, C });

	RouteFinder finder;
	cout << "Route found: " << finder.doesRouteExist(A, C, g) << endl;
}