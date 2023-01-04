#include <numeric>
#include <vector>
#include <iostream>
#include <concepts>

template <typename T>
void print(const std::vector<T>& vec) {
    for (size_t i = 0; auto& elem : vec)
        std::cout << elem << (++i == vec.size() ? "\n" : ", ");
}

void print2(const std::vector<auto>& vec) {
    for (size_t i = 0; auto& elem : vec)
        std::cout << elem << (++i == vec.size() ? "\n" : ", ");
}

void print3(const std::ranges::range auto& container) {
    for (size_t i = 0; auto && elem : container)
        std::cout << elem << (++i == container.size() ? "\n" : ", ");
};

int main() {
    std::vector d { 1.0, 2.0, 3.0, 4.0, 5.0};

    std::cout << "print: " << '\n';
    print(d);

    std::cout << "print2: " << '\n';
    print2(d);

    std::cout << "print3: " << '\n';
    print3(d);
}