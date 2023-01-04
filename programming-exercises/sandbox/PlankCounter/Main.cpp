#include <cassert>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace std;

class PlankCalculator
{
public:
    vector<int> findLengthCombos(const int numPlanks) const
    {
        checkForMinimumPlanks(numPlanks);

        const int numCombos = numPlanks + 1;
        vector<int> totalLengths(numCombos);

        for (int i = 0; i < numCombos; ++i) {
            const int numLong = i;
            const int numShort = numPlanks - numLong;
            assert(numShort >= 0);
            totalLengths[i] = (numShort * m_shortLen) + (numLong * m_longLen);
        }

        return totalLengths;
    }

    PlankCalculator(const int shortLen,
        const int longLen)
        : m_shortLen(shortLen),
        m_longLen(longLen)
    {
        assert(m_longLen > m_shortLen);
        assert(m_longLen > 0);
        assert(m_shortLen > 0);
    }

private:
    int m_shortLen;
    int m_longLen;

    void checkForMinimumPlanks(const int numPlanks) const
    {
        if (numPlanks < 2) {
            throw runtime_error("Diving board requires at least two planks!");
        }
    }
};

TEST_CASE("", "[]") {
    const int shortLen = 1;
    const int longLen = 2;
    PlankCalculator calculator(shortLen, longLen);

    SECTION("TwoPlankTest") {
        const vector<int> combos = calculator.findLengthCombos(2);
        REQUIRE(3 == combos.size());
        REQUIRE(combos[0] == 2 * shortLen);
        REQUIRE(combos[1] == 1 * shortLen + 1 * longLen);
        REQUIRE(combos[2] == 2 * longLen);
    }

    SECTION("FourPlankTest") {
        const vector<int> combos = calculator.findLengthCombos(4);
        REQUIRE(5 == combos.size());
        REQUIRE(combos[0] == 4 * shortLen);
        REQUIRE(combos[1] == 3 * shortLen + 1 * longLen);
        REQUIRE(combos[2] == 2 * shortLen + 2 * longLen);
        REQUIRE(combos[3] == 1 * shortLen + 3 * longLen);
        REQUIRE(combos[4] == 4 * longLen);
    }

}

#if 0
// To execute C++, please define "int main()"
int main() {

    const vector<int> vals{ 30, 32, 34, 37, 5, 7, 8, 12 };
    return 0;
}
#endif