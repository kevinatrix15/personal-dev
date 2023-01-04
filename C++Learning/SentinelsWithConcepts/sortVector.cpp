#include <algorithm>
#include <iostream>
#include <list>
#include <ranges>
#include <vector>

int main() {
    std::list<int> myList{1, -5, 10, 20, 0};
    std::sort(myList.begin(), myList.end());
    for_each(myList.begin(), myList.end(), [] (const int& v) {std::cout << v << ' '; });
}