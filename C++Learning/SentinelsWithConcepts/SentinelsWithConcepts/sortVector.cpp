#include <algorithm>
#include <list>
#include <ranges>

int main() {
    std::list<int> myList{ 1, -5, 10, 20, 0 };
    std::ranges::sort((myList.begin(), myList.end()));
}

#if 0
#include <numeric>
#include <vector>
#include <iostream>
#include <concepts>

template <typename T>
requires std::integral<T> || std::floating_point<T>
constexpr double average(std::vector<T> const& vec) {
    const double sum = std::accumulate(vec.begin(), vec.end(), 0.0);
    return sum / vec.size();
}

int main() {
    std::vector ints{ 1, 2, 3, 4, 5 };
    std::cout << average(ints) << '\n';

    std::vector doubles{ 1.0 , 2.0 , 3.0 , 4.0 , 5.0  };
    std::cout << average(doubles) << '\n';

    //std::vector strings{"w", "x", "y", "z"};
    //std::cout << average(strings) << '\n';
}
#endif
