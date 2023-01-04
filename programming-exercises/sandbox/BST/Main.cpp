#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct BinTreeNode {
    int val;
    BinTreeNode* left;
    BinTreeNode* right;

    /*
     * @brief Create a new BinTreeNode struct with a value provided.
     *
     * @param val The value to be assigned
     */
    BinTreeNode(const int val) : val(val),
        left(nullptr),
        right(nullptr)
    {
        // do nothing
    }
};

class BST
{
public:
    BST(const vector<int>& vals) : m_rootPtr(nullptr)
    {
        assert(vals.size() > 0);
        m_rootPtr = new BinTreeNode(vals[0]);

        for (size_t i = 1; i < vals.size(); ++i) {
            insert(m_rootPtr, vals[i]);
        }
    }

    void printTree()
    {
        visitInOrder(m_rootPtr, print);
    }

    static bool validBST(BinTreeNode* root) {
        return checkBST(root, INT_MIN, INT_MAX);
    }

private:
    BinTreeNode* m_rootPtr;

    BinTreeNode* insert(BinTreeNode* rootPtr, const int val)
    {
        // found insert location (leaf)
        if (!rootPtr) {
            return new BinTreeNode(val);
        }

        if (val < rootPtr->val) {
            rootPtr->left = insert(rootPtr->left, val);
        }
        else {
            rootPtr->right = insert(rootPtr->right, val);
        }
        return rootPtr;
    }

    void visitInOrder(BinTreeNode const* const nodePtr,
        void func(BinTreeNode const* const))
    {
        if (!nodePtr) {
            return;
        }
        visitInOrder(nodePtr->left, func);
        func(nodePtr);
        visitInOrder(nodePtr->right, func);
    }

    static void print(BinTreeNode const* const nodePtr) {
        cout << "Val: " << to_string(nodePtr->val) << "\n";
    }

    static bool checkBST(BinTreeNode* node, const int min, const int max)
    {
        if (!node) return true;
        if (min >= node->val || max < node->val) {
            return false;
        }
        const bool leftCheck = checkBST(node->left, min, node->val);
        const bool rightCheck = checkBST(node->right, node->val, max);
        return leftCheck && rightCheck;
    }
};

// To execute C++, please define "int main()"
int main() {
    // create a tree for testing
    // print contents of the tree
    BinTreeNode* rootPtr = new BinTreeNode(22);
    rootPtr->left = new BinTreeNode(10);
    rootPtr->left->left = new BinTreeNode(5);
    rootPtr->left->right = new BinTreeNode(15);

    rootPtr->right = new BinTreeNode(30);
    rootPtr->right->left = new BinTreeNode(30);
    rootPtr->right->right = new BinTreeNode(35);
    cout << "Valid BST: " << BST::validBST(rootPtr) << endl;

#if 0
    const vector<int> vals{ 10, 15, 5, 12 };
    BST bst(vals);
    bst.printTree();
#endif

    return 0;
}