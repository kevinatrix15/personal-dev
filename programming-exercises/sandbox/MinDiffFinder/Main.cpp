#include <algorithm>
#include <cassert>
#include <climits>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <map>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace std;

/* Problem Statement:
 * Given two arrays of integers, compute the pair of values (one value in each
 * array) with the smallest (non-negative) difference. Return the difference.
 *
 * Requirements:
 * - smallest (non-negative) difference
 * - can arrays contain negative values?
 * - does this mean taking an abs(), or strictly pairs that produce a positive diff?
 */

class MinDiffFinder
{
public:
    static int find(vector<int> arrA,
        vector<int> arrB)
    {
        // Approach:
        // 1. sort a and b
        // O(n log(n));
        sort(arrA.begin(), arrA.end());
        // O(m log(m));
        sort(arrB.begin(), arrB.end());

        // 2. march along each, computing difference
        size_t i = 0;
        size_t j = 0;
        int minDiff = INT_MAX;
        while (i < arrA.size() && j < arrB.size()) {
            int diff = abs(arrA[i] - arrB[j]);
            if (diff == 0) {
                return diff;
            }

            if (diff < minDiff) {
                minDiff = diff;
            }
            if (arrA[i] < arrB[j]) {
                ++i;
            }
            else {
                ++j;
            }
        }

        return minDiff;
    }
};


/* Requirements:
 *
 * Inputs:
 * partial overlap (after sorting):
    vector<int> a{1, 2, 3, 11, 15};
    vector<int> b{8, 19, 23, 127, 235};
 * "full" overlap:
    vector<int> a{1, 2, 3, 11, 15};
    vector<int> b{0, 1, 5, 13, 14};
 * no overlap:
    vector<int> a{1, 2, 3, 11, 15};
    vector<int> b{20, 31, 35, 43, 66};

 * After sorting:
    vector<int> a{1, 2, 3, 11, 15};
    vector<int> b{8, 19, 23, 127, 235};
 */

TEST_CASE("Find min difference", "[MinDiffFinder]") {


    SECTION("Small test") {
        vector<int> a{ 1, 3, 15, 11, 2 };
        vector<int> b{ 23, 127, 235, 19, 8 };
        MinDiffFinder finder;
        REQUIRE(3 == finder.find(a, b));
    }

}

#if 0
// To execute C++, please define "int main()"
int main() {

    const vector<int> vals{ 30, 32, 34, 37, 5, 7, 8, 12 };
    return 0;
}
#endif