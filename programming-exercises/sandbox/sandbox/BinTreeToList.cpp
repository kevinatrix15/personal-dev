#include <cassert>
#include <iostream>
#include <vector>

struct BinTreeNode {
    int val;
    BinTreeNode* left;
    BinTreeNode* right;

    /*
     * @brief Create a new BinTreeNode struct
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

class TreeToList {
public:
    /*
     * @brief Create a linked list of all the nodes at
     * each depth
     */
    std::vector<std::vector<int>> process(BinTreeNode const* const rootPtr)
    {
        // TODO: perform level by level traversal of the
        // binary tree
        updateLayerList(rootPtr, 0);
        return m_layerList;
    }

    /*
     * @brief Create a new TreeToList object
     *
     * @param treeDepth The depth of the binary tree
     */
    TreeToList(const size_t treeDepth) : m_layerList(treeDepth)
    {
        // do nothing
    }

private:

    /*
     * @brief Update the layer list data for the current
     * tree layer.
     *
     * @param rootPtr Pointer to the current node in the
     * binary tree.
     * @param level The current level of the tree.
     */
    void updateLayerList(BinTreeNode const* const rootPtr,
        const size_t level) {
        if (!rootPtr) {
            return;
        }
        assert(level < m_layerList.size());

        // add node to linked list vector
        m_layerList[level].emplace_back(rootPtr->val);

        // visit its children, adding to list
        updateLayerList(rootPtr->left, level + 1);
        updateLayerList(rootPtr->right, level + 1);
    }

    std::vector<std::vector<int>> m_layerList;
};

void printLayerNodes(const vector<int>& nodes) {
    for (const auto v : nodes) {
        std::cout << to_string(v) << "\n";
    }
}

// To execute C++, please define "int main()"
int main() {
    // create a tree for testing
    // print contents of the tree
    BinTreeNode* rootPtr = new BinTreeNode(22);
    BinTreeNode left(10);
    BinTreeNode right(30);

    rootPtr->left = &left;
    rootPtr->left->left = &right;

    const size_t treeDepth = 3UL;
    TreeToList ttl(treeDepth);
    const auto layerList = ttl.process(rootPtr);
    assert(layerList.size() == treeDepth);

    for (size_t layer = 0; layer < layerList.size(); ++layer) {
        std::cout << "Layer: " << std::to_string(layer) << "\n";
        printLayerNodes(layerList[layer]);
    }
    return 0;
}