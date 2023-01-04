#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

/* Problem Statement: Design a method for finding the frequency of occurrences of any given word in a book. What if we were runnign the algorithm multiple times?
*/

using namespace std;

class WordCounter
{
public:
    int count(const string& word) {
        return m_wordMap[toLower(word)];
    }

    explicit WordCounter(const vector<string>& book)
    {
        // create hash table of all words
        bookToHashMap(book);
    }

private:
    unordered_map<string, int> m_wordMap;

    string toLower(const string& word)
    {
        string lowerWord(word);
        transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(),
            [](unsigned char c) { return tolower(c); });
        return lowerWord;
    }

    void bookToHashMap(const vector<string>& book)
    {
        for (const string& word : book) {
            const string lowerWord = toLower(word);
            m_wordMap[lowerWord]++;
        }
    }
};

/* Requirements:
 * - Should it be case sensitive?
 * - Should book be stored as a single long string, or collection of strings?
 * - We need to flush the object if running the algorithm multiple times?
 */

TEST_CASE("Populated book tests", "[WordCounter]") {

    std::vector<string> book{ "Once", "upon", "a", "time", "there", "was", "a", "Square" };
    WordCounter counter(book);

    SECTION("Word is found") {
        const string target("was");
        const int instances = counter.count(target);
        REQUIRE(1 == instances);
    }

    SECTION("Different case word is found") {
        const string target("square");
        const int instances = counter.count(target);
        REQUIRE(1 == instances);
    }

    SECTION("Word not found") {
        const string target("zebra");
        const int instances = counter.count(target);
        REQUIRE(0 == instances);
    }
}

TEST_CASE("Empty book tests", "[WordCounter]") {

    std::vector<string> book{ "" };
    WordCounter counter(book);

    SECTION("Find word in empty book") {
        const string target("was");
        const int instances = counter.count(target);
        REQUIRE(0 == instances);
    }
}

#if 0
// To execute C++, please define "int main()"
int main() {

    const vector<int> vals{ 30, 32, 34, 37, 5, 7, 8, 12 };
    return 0;
}
#endif