#include <numeric>
#include <vector>
#include <iostream>
#include <concepts>

template <typename T> 
requires std::integral<T> || std::floating_point<T>
constexpr double average(const std::vector<T>& vec) {
    const double sum = std::accumulate(vec.begin(), vec.end(), 0.0);        
    return sum / vec.size();
}

int main() {
    std::vector ints { 1, 2, 3, 4, 5};
    std::cout << "avg of ints: " << average(ints) << '\n';                                      

    std::vector dbls { 1.0, 2.0, 3.0, 4.0, 5.0};
    std::cout << "avg of doubles: " << average(dbls) << '\n';                                      

    std::vector strs {"abc", "xyz", "def"};
    //std::cout << "avg of strings: " << average(strs) << '\n';                                      
}