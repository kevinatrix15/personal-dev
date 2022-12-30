#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

/* Search in rotated array:
 * example input: [30, 32, 34. 37, 5, 7, 8, 12]
 * find: 8

 * brute force:
 * - scan the array to find where the order changes
 *   * cost O(n)
 *   * edge cases / worst case:
 *     + fully sorted
 *     + last element drops
 */

class RotatedArray
{
public:
    /*
     * @brief find the index of a value for a sorted array which
     * has been rotated.
     *
     */
    int find(const int val)
    {
        return binSearch(val, 0, m_vals.size() - 1);
    }

    RotatedArray(const vector<int>& vals)
        : m_vals(vals),
        m_numRotations(0)
    {
        assert(m_vals.size() > 0);
        findNumRotations();
        // TODO: may add method to verify if vals is truly a
        // sorted rotated array??
    }

private:
    vector<int> m_vals;
    int m_numRotations;

    void findNumRotations()
    {
        int prevVal = m_vals[0];
        int rotations;
        const int len = static_cast<int>(m_vals.size());
        for (rotations = 1; rotations < len; ++rotations) {
            if (m_vals[rotations] < prevVal) {
                break;
            }
            prevVal = m_vals[rotations];
        }
        m_numRotations = rotations;
    }

    int getUnrotated(const int rotatedIdx) {
        const int sum = rotatedIdx + m_numRotations;
        return rotatedIdx <= m_numRotations ? sum
            : sum - static_cast<int>(m_vals.size());
    }

    int getRotated(const int unrotatedIdx) {
        const int diff = unrotatedIdx - m_numRotations;
        return unrotatedIdx >= m_numRotations ? diff
            : diff + static_cast<int>(m_vals.size());
    }

    void checkIdxInBounds(const int idx)
    {
        assert(idx >= 0);
        assert(static_cast<size_t>(idx) < m_vals.size());
    }

    int binSearch(const int val, const int uLow, const int uHigh) {
        const int uMid = (uLow + uHigh) / 2;
        const int rMid = getRotated(uMid);

        checkIdxInBounds(uLow);
        checkIdxInBounds(uHigh);
        checkIdxInBounds(uMid);
        checkIdxInBounds(rMid);

        // TODO: check for uLow > uHigh
        if (val < m_vals[rMid]) {
            return binSearch(val, uLow, uMid);
        }
        else if (val > m_vals[rMid]) {
            return binSearch(val, uMid + 1, uHigh);
        }
        else {
            return rMid;
        }
    }
};
// To execute C++, please define "int main()"
int main() {

    const vector<int> vals{ 30, 32, 34, 37, 5, 7, 8, 12 };
    RotatedArray ra(vals);

    const int searchIdx = 3;
    const int searchVal = vals[searchIdx];
    const int idx = ra.find(searchVal);
    cout << "Found val " << searchVal << " at idx " << idx << "\n";
    return 0;
}