#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#if 0
vector<int> numberOfItems(string s, vector<int> startIndices, vector<int> endIndices) {
    // NOTE: indices are 1-based!
    // approach: iterate over each char in the string between start and end indices, finding number of '*'s contained between two '|'
    assert(startIndices.size() == endIndices.size());
    const size_t cases = startIndices.size();
    vector<int> items(cases, 0);
    for (size_t c = 0; c < cases; ++c) {
        const int start = startIndices[c] - 1;
        const int end = endIndices[c] - 1;
        assert(start > 0);
        assert(end < s.size() - 1);
        bool candidate = false;
        int tmpCount = 0;
        for (int i = start; i < end; ++i) {
            if (!candidate && s[i] == '|') {
                candidate = true;
            }
            else if (candidate && s[i] == '*') {
                tmpCount++;
            }
            else if (candidate && s[i] == '|') {
                items[c]++;
                tmpCount = 0;
            }
        }
    }
    return items;
}

int main()
{
    string s = "**|*|*";
    vector<int> starts{ 1 };
    vector<int> ends{ 6 };


}
#endif


 // Definition for a binary tree node.
 struct TreeNode {
     int val;
     TreeNode *left;
     TreeNode *right;
     TreeNode() : val(0), left(nullptr), right(nullptr) {}
     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 };

class Solution {
public:
    TreeNode* sortedArrayToBST(const vector<int>& nums) {
        return insertNode(nums, 0, nums.size());
    }
private:
    TreeNode* insertNode(const vector<int>& nums, const size_t begin, const size_t end) {
        const size_t mid = (end + begin) / 2;
        TreeNode node(nums[mid]);
        if (mid > begin) {
            assert(mid > 0);
            node.left = insertNode(nums, begin, mid);
        }
        cout << "Val: " << node.val << endl;
        if (mid < end - 1) {
            assert(mid < nums.size() - 1);
            node.right = insertNode(nums, mid + 1, end);
        }
        return &node;
    }

};

int main()
{
    Solution sln;
    TreeNode* tree = sln.sortedArrayToBST(vector<int>{-10, -3, 0, 5, 9});
}
