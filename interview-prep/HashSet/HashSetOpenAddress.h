#include <cassert>
#include <vector>

namespace
{
	constexpr int DELETED = -1;
	constexpr double REHASH_LIMIT = 0.8;
	// TODO: replace with int_min
	constexpr int EMPTY = -99999;
}

class HashSetOpenAddress
{
public:
	HashSetOpenAddress(const size_t numBins) : m_keys(numBins, EMPTY), m_capacity(numBins), m_size(0)
	{
		assert(m_capacity > 0);
	}

	// Modifiers
	void add(const int key)
	{
		// don't add duplicates or EMPTY keys
		if (has(key) || key == EMPTY) {
			return;
		}
		// get hash index
		const auto bin = nextFreeBin(key);
		m_keys[bin] = key;
		++m_size;

		// TODO: check if we are approaching our load factor threshold,
		// rehashing if necessary
		checkLoadFactorAndRehash();

	}

	void remove(const int key)
	{
		auto bin = getBinIdx(key);
		while (isBinTaken(bin)) {
			if (m_keys[bin] == key) {
				m_keys[bin] = DELETED;
				--m_size;
			}
			++bin;
		}
		// NOTE: if the key isn't found, do nothing
		// TODO: consider down-sizing if decreased to half of capacity??
	}

	void clear()
	{
		std::fill(m_keys.begin(), m_keys.end(), EMPTY);
	}

	// Capacity
	bool has(const int key) const
	{
		auto bin = getBinIdx(key);
		while (isBinTaken(bin)) {
			if (m_keys[bin] == key) {
				return true;
			}
			++bin;
		}
		return false;
	}

	bool empty() const
	{
		return m_size == 0;
	}

	size_t size() const
	{
		return m_size;
	}

	size_t capacity() const
	{
		return m_capacity;
	}

private:
	std::vector<int> m_keys;
	size_t m_capacity;
	size_t m_size;

	size_t getBinIdx(int key) const
	{
		size_t h = std::hash<int>{}(key);
		return h % m_capacity;
	}

	size_t nextFreeBin(const int key)
	{
		auto bin = getBinIdx(key);
		// TODO: the below can get into an infinite loop
		// if all bins are full. Need to check this?
		while (isBinTaken(bin)) {
			++bin;
			bin %= m_capacity;
		}
		return bin;

	}

	bool isBinTaken(const size_t bin) const
	{
		return m_keys[bin] != EMPTY && m_keys[bin] != DELETED;
	}

	void checkLoadFactorAndRehash()
	{
		//if (static_cast<double>(m_size / m_capacity) >= REHASH_LIMIT) {
		assert(m_capacity > m_size);
		if (m_capacity - m_size <= 1U) {
			rehash();
		}
	}

	void rehash()
	{
		m_capacity *= 2;
		const auto tmp = m_keys;
		m_keys = std::vector<int>(m_capacity, EMPTY);
		// reset the size, which will be reassigned in the add() call below
		m_size = 0;
		for (const auto k : tmp) {
			add(k);
		}
	}
};