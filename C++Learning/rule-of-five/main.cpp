#include <iostream>

// Parentd on answer here: stackoverflow.com/questions/49961811/must-a-c-interface-obey-the-rule-of-five 

// TODO: for examples, would like to easily switch between different definitions of parents
// see https://stackoverflow.com/questions/60155403/select-the-implementation-of-the-parent-class-at-runtime
// for possible solution

class SuperParent { };

/**
 * Case 1- won't allow any copying.
 * - prevents polymorphic copy (slicing)- good
 * - prevents regular copying- generally bad
*/
class ParentDeleted : public SuperParent
{
   public:
    virtual ~ParentDeleted() = default;

    virtual void printVal() const = 0;

   protected:
    // Constructors
    ParentDeleted() = delete;
    ParentDeleted(const ParentDeleted&) = delete;
    ParentDeleted(ParentDeleted&&) = delete;

    // Assignment Operators
    ParentDeleted& operator=(const ParentDeleted&) = delete;
    ParentDeleted& operator=(ParentDeleted&&) = delete;
};

/**
 * Case 2- allows all copying.
 * - allows regular copying- good
 * - allows polymorphic copying (slicing)- bad
*/
class ParentDefaultedPublic : public SuperParent
{
   public:
    virtual ~ParentDefaultedPublic() = default;

    virtual void printVal() const = 0;

   public:
    // Constructors
    ParentDefaultedPublic() = default;
    ParentDefaultedPublic(const ParentDefaultedPublic&) = default;
    ParentDefaultedPublic(ParentDefaultedPublic&&) = default;

    // Assignment Operators
    ParentDefaultedPublic& operator=(const ParentDefaultedPublic&) = default;
    ParentDefaultedPublic& operator=(ParentDefaultedPublic&&) = default;
};

class PolymorphicBase
{
   public:
    virtual ~PolymorphicBase() = default;

    // Other virtual methods...

   protected:
    // Constructors
    PolymorphicBase() = default;
    PolymorphicBase(const PolymorphicBase& other) = default;
    PolymorphicBase(PolymorphicBase&& other) = default;

    // Assignment Operators
    PolymorphicBase& operator=(const PolymorphicBase& other) = default;
    PolymorphicBase& operator=(PolymorphicBase&& other) = default;
};

/**
 * Case 3- Undefined (breaks rule of 5) (similar to Case 2)
 * - allows regular copying- good
 * - allows polymorphic copying (slicing)- bad
*/
class ParentUndefined : public SuperParent
{
   public:
    virtual ~ParentUndefined() = default;

    virtual void printVal() const = 0;
};

/**
 * Case 4- "best" solution
 * - allows regular copying- good
 * - prevents polymorphic copying (slicing)- good
*/
class ParentDefaultedProtected : public SuperParent
{
   public:
    virtual ~ParentDefaultedProtected() = default;

    virtual void printVal() const = 0;

   protected:
    // Constructors
    ParentDefaultedProtected() = default;
    ParentDefaultedProtected(const ParentDefaultedProtected&) = default;
    ParentDefaultedProtected(ParentDefaultedProtected&&) = default;

    // Assignment Operators
    ParentDefaultedProtected& operator=(const ParentDefaultedProtected&) = default;
    ParentDefaultedProtected& operator=(ParentDefaultedProtected&&) = default;
};

template<class Base>
class Child : public Base
{
   public:
    explicit Child(const int val) : val_(val)
    {
    }

    void printVal() const override
    {
        std::cout << "val: " << val_ << std::endl;
    }

   protected:
    int val_{};
};

template<class Base>
class Grandchild : public Child<Base>
{
   public:
    explicit Grandchild(const int v1, const int v2) : Child<Base>(v1), val_(v2)
    {
    }

    void printVal() const override{
        std::cout << "Grandchild: v1: " << Child<Base>::val_ << ", v2: " << val_ << std::endl;
    }

   private:
    int val_{};
};

template<class ParentType>
void attemptCopying()
{
    Child<ParentType> d1{1};
    Child<ParentType> d2{2};
    ParentType* d1_ptr = &d1;
    ParentType* d2_ptr = &d2;

    // regular copy
    d2 = d1;

    // polymorphic copy
    *d2_ptr = *d1_ptr;
}

int main(int, char**) {
    // Grandchild<ParentDefaultedProtected> d2{2, 3};
    // Child<ParentDefaultedProtected> c = d2;
    // c.printVal();
    // d1.printVal();

    attemptCopying<ParentUndefined >();

    // object slicing
    // d2_ptr->printVal();

    std::cout << "Hello, world!\n";
}