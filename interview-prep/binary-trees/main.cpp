#include <iostream>
#include <vector>

using namespace std;

struct Node
{
    public:
    Node(const int val) : val(val), left(nullptr), right(nullptr)
    {
    }

    Node* left;
    Node* right;
    int val;
};

class BinarySearchTree
{
    public:
    BinarySearchTree(const vector<int> vals) : m_root(nullptr)
    {
        m_root = new Node(vals[0]);
        for (size_t i = 1; i < vals.size(); ++i) {
            insert(m_root, vals[i]);
        }
    }

    void print()
    {
        inOrderTraversal(m_root);
    }

    bool isFound(const int val)
    {
        Node* found = findVal(m_root, val);
        return found;
    }

    private:
    Node* m_root;

    Node* insert(Node* root, const int val)
    {
        // check for null root
        if (!root) {
            return root = new Node(val);
        }

        if (val < root->val) {
            root->left = insert(root->left, val);
        } else {
            root->right = insert(root->right, val);
        }
        return root;
    }

    Node* findVal(Node* root, const int val)
    {
        // reached leaf without finding the value, or found the value
        if (!root || val == root->val) {
            return root;
        }

        if (val < root->val) {
            return findVal(root->left, val);
        } else {
            return findVal(root->right, val);
        }
    }

    void visit(Node* node)
    {
        std::cout << node->val << std::endl;
    }

    void inOrderTraversal(Node* node)
    {
        if (node) {
            inOrderTraversal(node->left);
            visit(node);
            inOrderTraversal(node->right);
        }
    }

    void preOrderTraversal(Node* node)
    {
        if (node) {
            visit(node);
            inOrderTraversal(node->left);
            inOrderTraversal(node->right);
        }
    }

    void postOrderTraversal(Node* node)
    {
        if (node) {
            inOrderTraversal(node->left);
            inOrderTraversal(node->right);
            visit(node);
        }
    }

};


int main(int, char**) {
    const vector<int> vals{1, 4, 6, 8, 5, 2};
    BinarySearchTree tree(vals);
    tree.print();

    cout << "isFound(2): " << tree.isFound(2) << endl;
    cout << "isFound(7): " << tree.isFound(7) << endl;
    return 0;
}
