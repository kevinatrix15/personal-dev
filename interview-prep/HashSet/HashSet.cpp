#include <cassert>
#include <climits>
#include <exception>
#include <iostream>
#include <vector>

#include "HashSetOpenAddress.h"

namespace
{
	constexpr double LF_LIMIT = 0.7;
}

struct Node
{
public:
	int data;
	Node* next;
	Node() : data(), next(nullptr)
	{
	}

	Node(const int val) : data(val), next(nullptr)
	{
	}

};

class LinkedList
{
public:
	LinkedList() : m_headPtr(nullptr), m_len(0)
	{
	}

	bool has(const int val) const
	{
		Node* n = m_headPtr;
		while (n) {
			if (n->data == val) {
				return true;
			}
			n = n->next;
		}
		return false;
	}

	void insert(Node* prevNode, const int val)
	{
		// TODO: check if prevNode is null
		if (!prevNode) {
			throw std::exception("Previous node must exist for insert.");
		}
		Node* newNode = new Node(val);
		newNode->next = prevNode->next;
		prevNode->next = newNode;
		++m_len;
	}

	void remove(const int val)
	{
		// edge case- empty list- skip
		if (!m_headPtr) {
			return;
		}

		// edge case- we only have head and it matches the value
		if (m_headPtr->data == val) {
			m_headPtr = m_headPtr->next;
			--m_len;
			return;
		}

		// search the list for 'val'
		Node* n = m_headPtr;
		if (n->data == val) {
			n = nullptr;
		}
		while (n && n->next) {
			if (n->next->data == val) {
				// make parent point to n->next
				if (n->next->next) {
					n->next = n->next->next;
				}
				else {
					n->next = nullptr;
				}
				--m_len;
				return;
			}
			n = n->next;
		}
	}

	void push_front(const int val)
	{
		Node* newNode = new Node(val);
		newNode->next = m_headPtr;
		m_headPtr = newNode;
		++m_len;
	}

	void push_back(const int val)
	{
		Node* newNode = new Node(val);

		if (!m_headPtr) {
			m_headPtr = newNode;
			++m_len;
			return;
		}

		Node* n = m_headPtr;
		// traverse to tend of our linked list
		while (n->next) {
			n = n->next;
		}
		n->next = newNode;
		++m_len;
	}

	int front() const
	{
		// TODO: handle m_headPtr == nullptr
		return m_headPtr->data;
	}

	void pop_front()
	{
		if (!m_headPtr) {
			return;
		}

		const int val = m_headPtr->data;
		m_headPtr = m_headPtr->next;
		--m_len;
	}

	int back() const
	{
		// TODO: handle m_headPtr == nullptr
		Node* n = m_headPtr;
		while (n->next) {
			n = n->next;
		}
		return n->data;
	}

	void pop_back()
	{
		// if m_headPtr is null, error out
		if (!m_headPtr) {
			//throw std::exception("Can't pop back of empty list.");
			return;
		}

		// get to second to last node
		Node* n = m_headPtr;
		while (n && n->next && n->next->next) {
			n = n->next;
		}
		n->next = nullptr;
		--m_len;
	}

	bool empty() const
	{
		return m_len == 0U;
	}

	void clear()
	{
		m_headPtr = nullptr;
		m_len = 0U;
	}

	void print() const
	{
		Node* n = m_headPtr;
		while (n) {
			std::cout << n->data << " ";
			n = n->next;
		}
		std::cout << "\n";
	}

	size_t len() const
	{
		assert(m_len >= 0);
		return static_cast<size_t>(m_len);
	}


private:
	Node* m_headPtr;
	int m_len;
};

class HashSet
{
public:
	HashSet(const size_t initNumBuckets) : m_numBuckets(initNumBuckets), m_keys(initNumBuckets, LinkedList())
	{
		assert(m_numBuckets > 0);
	}

	// Modifiers
	void add(const int val)
	{
		// get hash index
		const auto bin = getBucketIdx(val);
		if (!m_keys[bin].has(val)) {
			m_keys[bin].push_front(val);
		}

		// check if we are approaching our load factor threshold,
		// rehashing if necessary
		checkLoadFactorAndRehash();

	}

	void remove(const int val)
	{
		const auto bin = getBucketIdx(val);
		m_keys[bin].remove(val);
	}

	void clear()
	{
		std::fill(m_keys.begin(), m_keys.end(), LinkedList());
	}

	// Capacity
	bool has(const int val) const
	{
		const auto bin = getBucketIdx(val);
		return m_keys[bin].has(val);
	}

	bool empty() const
	{
		return size() == 0U;
	}

	size_t size() const
	{
		size_t count = 0;
		for (const auto& l : m_keys) {
			count += l.len();
		}
		return count;
	}

	size_t maxSize() const
	{
		return m_numBuckets;
	}

private:
	size_t m_numBuckets;
	std::vector<LinkedList> m_keys;

	size_t getBucketIdx(int key) const
	{
		size_t h = std::hash<int>{}(key);
		return h % m_numBuckets;
	}

	void checkLoadFactorAndRehash()
	{
		if (size() / m_numBuckets > LF_LIMIT) {
			rehash();
		}
	}

	void rehash()
	{
		// double num bins
		// iterate through entries and reassign 
		auto tmp = m_keys;
		m_numBuckets *= 2;
		m_keys = std::vector<LinkedList>(m_numBuckets, LinkedList());

		for (auto& list : tmp) {
			// pop from list and assign
			while (!list.empty()) {
				const auto val = list.front();
				list.pop_front();
				add(val);
			}
		}
	}
};

int main()
{
	HashSetOpenAddress set(5);
	set.add(1);
	set.add(1);
	set.add(1);
	set.add(2);
	set.add(3);
	set.add(4);
	set.add(5);
	set.add(6);
	set.add(7);
	set.remove(8);
	set.remove(7);
	set.remove(6);
	set.add(6);
#if 0
	std::vector<int> vals{1,2,3,4,5,6};
	LinkedList list0;
	LinkedList list1;
	for (const auto& v : vals) {
		list0.push_back(v);
		list1.push_front(v);
	}
	list0.print();
	list1.print();

	list1.clear();
	list1.print();

	auto f = list0.pop_front();
	auto b = list0.pop_back();
	std::cout << "front: " << f << ", back: " << b << std::endl;
	std::cout << "updated list: " << std::endl;
	list0.print();
#endif
	return 0;
}