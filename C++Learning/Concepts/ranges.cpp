#include <algorithm>
#include <iostream>
#include <list>
#include <ranges>
#include <vector>

template <typename T>
void sort(T& arr) 
{
    std::ranges::sort(arr.begin(), arr.end());
}

template <typename T>
void sort(T& arr) 
{
    std::ranges::sort(arr.begin(), arr.end());
}

int main() {
    std::list<int> val_list = {1, 4, 2, 3};
    sort<std::list<int>>(val_list);

    std::ranges::copy(val_list.begin(), val_list.end(), 
        std::ostream_iterator<int>(std::cout, ","));
}