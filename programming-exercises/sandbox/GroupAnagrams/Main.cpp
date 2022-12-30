#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// write a method to sort an array of strings such that all
// anagrams are next to each other

/* example:
 * {"cat", "rat", "act", "tar"} => {"act", "cat", "rat", "tar"};
 *
 * Requirements:
 * - all grouped strings must have same number of characters
 * - all grouped strings must have same combination of characters
 *
 * Approach:
 * - create a hash table mapping from sorted version of word to a
 * list of its anagrams found
 */

vector<string> groupAnagrams(const vector<string>& strings)
{
    unordered_map<string, vector<string>> map;
    for (const auto& unsorted : strings) {
        const string sorted = sort(unsorted.begin(), unsorted.end());
;
        map[sorted].emplace_back(unsorted);
    }

    // collect contents of map into grouped anagrams
    vector<string> groupedAnagrams(strings.size());
    size_t count = 0;
    for (const auto& [key, vals] : map) {
        for (const auto& val : vals) {
            groupedAnagrams[count++] = val;
        }
    }
    return groupedAnagrams;
}

void printStrings(const vector<string>& strings)
{
    for (const auto& s : strings) {
        cout << s << "\n";
    }
}

// To execute C++, please define "int main()"
int main() {
    vector<string> strings{ "abba", "cdefg", "efg", "baba", "baab", "fegcd" };
    const vector<string> grouped = groupAnagrams(strings);

    cout << "Ungrouped strings: \n";
    printStrings(strings);

    cout << "Grouped strings: \n";
    printStrings(grouped);

    return 0;
}