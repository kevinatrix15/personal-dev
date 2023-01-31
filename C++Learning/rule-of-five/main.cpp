#include <iostream>

// Based on answer here: stackoverflow.com/questions/49961811/must-a-c-interface-obey-the-rule-of-five 

// TODO: for examples, would like to easily switch between different definitions of parents
// see https://stackoverflow.com/questions/60155403/select-the-implementation-of-the-parent-class-at-runtime
// for possible solution

class BaseDeleted
{
    public:
    virtual ~BaseDeleted() = default;

    virtual int getVal() const = 0;

    protected:
    // Constructors
    BaseDeleted() = default;
    BaseDeleted(const BaseDeleted&) = default;
    BaseDeleted(BaseDeleted&&) = default;

    // Assignment Operators
    BaseDeleted& operator=(const BaseDeleted&) = default;
    BaseDeleted& operator=(BaseDeleted&&) = default;
};

class Derived : public BaseDeleted
{
    public:
    Derived(const int val) : val_(val)
    {
    }

    private:
    int val_{};
};

int main(int, char**) {
    std::cout << "Hello, world!\n";
}