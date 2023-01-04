#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct Node
{
    int val;
    vector<Node> children{};
    bool visited;

    Node(const int val) : val(val) {}

    bool operator == (const Node& other) const
    {
        return (val == other.val && 
                children == other.children && 
                visited == other.visited);
    }
};

class Graph
{
    public:
    vector<Node> nodes;
    Graph(vector<Node> nodes) : nodes(nodes) {}

    void resetVisited()
    {
        for (auto& node : nodes) {
            node.visited = false;
        }
    }
};

class RouteFinder
{
    public:
    static bool doesRouteExist(const Node& a, const Node& b, Graph& g)
    {
        return (isRouteFound(a, b, g) || isRouteFound(b, a, g));
    }

    private:

    static bool isRouteFound(const Node& root, const Node& target, Graph& g)
    {
        g.resetVisited();
        
        queue<Node> q;
        q.emplace(root);

        while(!q.empty()) {
            Node top = q.front();
            if (top == target) {
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

int main(int, char**) {
    Node A(1);
    Node B(2);
    Node C(3);
    Node D(4);

    B.children.emplace_back(C);
    A.children.emplace_back(B);

    Graph g({A, B, C, D});
    cout << "A->C: " << RouteFinder::doesRouteExist(A, C, g) << endl;
    cout << "C->A: " << RouteFinder::doesRouteExist(C, A, g) << endl;
    cout << "A->D: " << RouteFinder::doesRouteExist(A, D, g) << endl;

    return 0;
}