#include <iostream>

class A
{
    public:
    A()
    {
        std::cout << "A::Default ctor" << std::endl;
    }

    A(const int v) : val_(v)
    {
        std::cout << "A::Custom ctor" << std::endl;
    }

    A(const A& other)
    {
        val_ = other.val_;
        std::cout << "A::Copy ctor" << std::endl;
    }

    A& operator=(const A& other)
    {
        if (this != &other) {
            val_ = other.val_;
        }
        std::cout << "A::Assignment operator" << std::endl;
        return *this;
    }

    // Move operations
    A(A&& other) noexcept = default;
    A& operator=(A&& other) noexcept = default;

    ~A()
    {
        std::cout << "A::Destructor" << std::endl;
    }

    private:
    int val_{0};
};

class B
{
    public:
    B()
    {
        std::cout << "B::Default ctor" << std::endl;
    }

    B(int* data) : data_(data)
    {
        std::cout << "B::Custom ctor" << std::endl;
    }

    B(const B& other)
    {
        // perform a deep copy of a_ptr_
        data_ = new int;
        *data_ = *other.data_;

        std::cout << "B::Copy ctor" << std::endl;
    }

    B& operator=(const B& other)
    {
        // avoid self-assignment
        if (this != &other) {
            // release any heap resources we're holding
            delete data_;

            // deep copy of the resource
            data_ = new int;
            *data_ = *other.data_;
        }
        
        std::cout << "B::Assignment operator" << std::endl;
        return *this;
    }

    // Move semantics
    B(B&& other) noexcept
    {
        // copy other's resources
        data_ = other.data_;

        // reset other's recources
        other.data_ = nullptr;

        std::cout << "B::Move ctor" << std::endl;
    }

    B& operator=(B&& other) noexcept
    {
        // avoid self-assignment
        if (this != &other) {
            // release any heap resources we're holding
            delete data_;

            // copy the other pointer
            data_ = other.data_;
        
            // reset the other pointer's data
            other.data_ = nullptr;
        }

        std::cout << "B::Move assignment operator" << std::endl;
        return *this;
    }

    ~B()
    {
        // delete the pointer if not already deleted
        if (data_ != nullptr) {
            delete data_;
        }
        std::cout << "B::Destructor" << std::endl;
    }

    private:
    int* data_{nullptr};

};

class C
{
    public:
    C()
    {
        std::cout << "C::Default ctor" << std::endl;
    }

    C(int* data) : data_(data)
    {
        std::cout << "C::Custom ctor" << std::endl;
    }

    C(const C& other)
    {
        // perform a deep copy of a_ptr_
        data_ = new int;
        *data_ = *other.data_;

        std::cout << "C::Copy ctor" << std::endl;
    }

    C& operator=(const C& other)
    {
        // avoid self-assignment
        if (this != &other) {
            // release any heap resources we're holding
            // delete data_;

            // deep copy of the resource
            data_ = new int;
            *data_ = *other.data_;
        }
        
        std::cout << "C::Assignment operator" << std::endl;
        return *this;
    }

    // Un-specified moves- not declared since we defined custom
    // copy ctor and assignment operator. An attempt to call
    // std::move will result in a copy instead

    // C(C&& other) noexcept = default;
    // C& operator=(C&& other) noexcept = default;

    ~C()
    {
        // delete the pointer if not already deleted
        if (data_ != nullptr) {
            delete data_;
        }
        std::cout << "C::Destructor" << std::endl;
    }

    private:
    int* data_{nullptr};

};

int main(int, char**) {
    A a0;           // default ctor
    A a1(42);       // custom ctor
    A a2(a1);       // copy ctor
    A a3 = a1;      // copy ctor*
    a1 = a2;        // assignment op - called when assigning to an alreadh initialized object

    int val1 = 7;
    B b0;                   // default ctor
    B b1(&val1);            // custom ctor
    B b2(std::move(b1));    // move ctor
    b1 = std::move(b2);     // move assignment

    int val2 = 51;
    C c1(&val2);            // custom ctor
    C c2(std::move(c1));    // move ctor not defined. Calls copy ctor instead
    c1 = std::move(c2);

    return 0;
}