#include <iostream>
#include <stack>
#include <queue>
#include <vector>

struct Node
{
public:
	int data;
	Node* left;
	Node* right;
};

void pushQueueLeft(std::queue<Node*>& q, Node const * const top)
{
	if (top->left) {
		q.push(top->left);
	}
}

void pushQueueRight(std::queue<Node*>& q, Node const * const top)
{
	if (top->right) {
		q.push(top->right);
	}
}

void pushStackLeft(std::stack<Node*>& s, Node const * const top)
{
	if (top->left) {
		s.push(top->left);
	}
}

void pushStackRight(std::stack<Node*>& s, Node const * const top)
{
	if (top->right) {
		s.push(top->right);
	}
}

void printLevelOrder(Node* root)
{
	if (!root) {
		return;
	}

	std::queue<Node*> q;
	q.push(root);

	while (!q.empty()) {
		Node* top = q.front();
		std::cout << top->data << std::endl;
		q.pop();
		pushQueueLeft(q, top);
		pushQueueRight(q, top);
	}
}

void printZigZagOrder(Node* root)
{
	if (!root) {
		return;
	}

	std::stack<Node*> curr;
	curr.push(root);

	bool leftToRight = true;

	std::vector<std::vector<int>> fullVals;
	std::vector<int> currVals;
	std::stack<Node*> next;
	while (!curr.empty()) {
		Node* top = curr.top();
		std::cout << top->data << std::endl;
		currVals.emplace_back(top->data);
		curr.pop();
		if (leftToRight) {
			pushStackLeft(next, top);
			pushStackRight(next, top);
		}
		else {
			pushStackRight(next, top);
			pushStackLeft(next, top);
		}
		if (curr.empty()) {
			curr.swap(next);
			fullVals.emplace_back(currVals);
			currVals.clear();
			leftToRight = !leftToRight;
		}
	}
}

Node* newNode(const int data)
{
	Node* node = new Node();
	node->data = data;
	node->left = node->right = nullptr;
	return node;
}

int main()
{
	Node* root = newNode(1);
	root->left = newNode(2);
	root->right = newNode(3);
	root->left->left = newNode(4);
	root->left->right = newNode(5);
	root->right->left = newNode(6);
	root->right->right = newNode(7);
	std::cout << "level order: " << std::endl;
	printLevelOrder(root);

	std::cout << "zig-zag order: " << std::endl;
	printZigZagOrder(root);

	return 0;
}