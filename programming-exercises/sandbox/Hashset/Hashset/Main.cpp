#include <iostream>
#include <list>
#include <vector>

using namespace std;

// Using chain hashing for collision avoidance
class Hashset
{
public:
	Hashset(const int bins) : NUM_BINS(bins), m_data(bins)
	{
	}

	void insert(const int val)
	{
		const int idx = hashFunc(val);
		m_data[idx].emplace_back(val);
	}

	void remove(const int val)
	{
		const int idx = hashFunc(val);
		if (!m_data[idx].empty()) {
			m_data[idx].remove(val);
		}
	}

	void display()
	{
		//for (const list<int>& bin : m_data) {
		for (size_t i = 0; i < m_data.size(); ++i) {
			cout << "key: " << i << " [";
			const list<int> bin = m_data[i];
			for (const int v : bin) {
				cout << " " << v;
			}
			cout << "]" << endl;
		}
	}

private:
	int hashFunc(const int val)
	{
		return val % NUM_BINS;
	}

	vector<list<int>> m_data;
	const int NUM_BINS;
};

int main()
{
	Hashset set(7);
	vector<int> vals{ 15, 11, 27, 8 };

	for (const auto& v : vals) {
		set.insert(v);
	}

	set.display();

	return 0;
}