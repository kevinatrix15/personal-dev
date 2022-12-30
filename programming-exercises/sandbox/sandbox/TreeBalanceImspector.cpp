#include <cassert>
#include <iostream>
#include <string>
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

class TreeBalanceInspector
{
public:
    bool isBalanced(BinTreeNode const* const rootPtr)
    {
        // assume a leaf is balanced
        if (!rootPtr) {
            return true;
        }

        const int leftHeight = getHeight(rootPtr->left);
        const int rightHeight = getHeight(rootPtr->right);

        // check if current node is out of balance
        if (std::abs(leftHeight - rightHeight) > 1) {
            return false;
        }

        // check each of children for balance
        return isBalanced(rootPtr->left) &&
            isBalanced(rootPtr->right);
    }

private:
    int getHeight(BinTreeNode const* const rootPtr) {
        if (!rootPtr) {
            return 0;
        }
        return std::max(getHeight(rootPtr->left),
            getHeight(rootPtr->right)) + 1;
    }
};

// To execute C++, please define "int main()"
int main() {
    // create a tree for testing
    // print contents of the tree
    BinTreeNode* rootPtr = new BinTreeNode(22);
    BinTreeNode left(10);
    BinTreeNode right(30);

    rootPtr->left = &left;
    rootPtr->right = &right;
    rootPtr->left->left = &left;

    TreeBalanceInspector tbi;

    const bool inBalance = tbi.isBalanced(rootPtr);
    std::cout << "inBalance? " << std::to_string(inBalance) << "\n";
    return 0;
}