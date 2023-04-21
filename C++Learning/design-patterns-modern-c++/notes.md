# Design Patterns in Modern C++

## Section 2- SOLID Design Principles

Single Responsibility Principle
- Class should only have one reason to change
- _Separation of concerns_- different classes handling different, independent tasks / problems

Open-Closed Principle
- Classes should be open for extension, but closed for modification
- If needing to modify existing, already tested code for added functionality, should consider using inheritance to extend it instead.

Liskov Substituion Principle
- Should be able to substitute a base type for subtype

Interface Segregation Principle
- Don't put too much into an interface; split into separate interfaces

Dependency Inversion Principle
- High-level modules should not depend upon low-level ones; use abstractions

## Section 3- Builder

### Gamma Categorization
Design patterns are typically split into 3 categories

Creational Patterns
- Deal with creation of objects
- Explicit (constructor) vs. implicit (DI, reflection, etc.)
- Wholesale (single statement) vs. piecewise (step-by-step)

Structural Patterns
- Concerned with the structure of the pattern
- Concerned with the structure (class members)
- Many patterns are wrappers that mimic the underlying class' interface
- Stress the importance of good API design- to make objects usable

Behavioral Patterns
- Don't follow central theme, unlike above patterns groups

### Builder Patterns
Some objects are simple, created in single constructor call

Other objects require lot of ceremony to create

Too many ctor args is not productive. Instead, should opt for piecewise construction

Builder provides a convenient API for constructing an object step-by-step

#### Fluent Builder

This allows the stringing together of calls to add elements before building. It is achieved through the following:

```c++
struct Builder
{
    Builder& add_child(const string element)
    {
        ...
    }

    string str()
    {
        return str_;
    }
};

main()
{
    Bulder bldr;
    bldr.add_child("foo").add_child("bar").add_child(baz);
    cout << bldr.str() << endl;
}
```
#### Force Use of Builder
Simply make constructors of base class being built private. NOTE: you will need to make
the builder a friend of the base class.

Side notes:
- Can provide an `initializer_list` as a constructor argument. This is treated similar to a vector (and is implicitly convertible to a vector).

### Faceted Builder
Can use multiple builders to build a more complex object. To do this, we use:

- Builder class- this is used to provide a reference to the BuilderBase (see next), which avoids creating multiple instances of the Builder object.
- BuilderBase- this is the actual class inhereited from
- Builder1 : BuilderBase- First builder object to create its portion of the object.
- Builder2 : BuilderBase- Second builder object for its portion.
- Object- The object to be created. Contains a static `create()` method which returns a Builder object.


## Section 4- Factories

Motivatation:
- object creation logic becomes too convoluted
- Ctor is not  descriptive
- Object creation (non-piecewise, unline Builder) can be outsourced to
- Can create hierarchy of factories w/ Abstract Factories

### Factory Method (of the class it's constructing)
- Make ctor private
- Make static methods to construct that object from a specific set of inputs
[example](./sec4-factories/Creational.Creational.FactoryMethod.cpp)

### Factory
- Separate factory class with static (or non-static) methods for different ways of constructing the object.
- NOTE: if class's ctor is private, must make the factory a friend class. This does break the open-closed principle.
    - If don't want to violate this, can make the ctor public and then there's not need to add the friend.
    This enables directly creating the object, so need to confirm whether this is the desired behavior.

### Inner Factory
- For clarity, the factory class can be defined within the class definition it is constructing
- NOTE: Inner classes have access to private members of the class. This alleviates the OCP violation.
- Options: 
    - can make factory public
    - Can make it private- make it a singleton by declaring an instance of the factory as a (public) static member of the class.

Benefits of inner factory:
- Has access to private class members
- Control over its lifetime- if want to prevent anyone from making an instance of the factory, can make ctor private and then expose the factory as public field or through getter.

### Abstract Factory
Idea: enable families of factories. Use inheritance to allow calling on polymorphic factories.

### Functional Factory
Idea: instead of using a factory, we'll use an anonymous function. For the factory of factories, the example defines a map of functions, where each
function is provided as a lambda. For example:

```c++
class DrinkWithVolumeFactory
{
  map<string, function<unique_ptr<HotDrink>()>> factories;
public:

  DrinkWithVolumeFactory()
  {
    factories["tea"] = [] {
      auto tea = make_unique<Tea>();
      tea->prepare(200);
      return tea;
    };
  }

  unique_ptr<HotDrink> make_drink(const string& name);
};
```

The method to make a particular drink (invoking the factory) is done with something like the following:

```c++
inline unique_ptr<HotDrink> DrinkWithVolumeFactory::make_drink(const string& name)
{
  return factories[name](); // calls factory lambda
}
```


## Summary
- Factory is just a static method that creates objects
- Factory can take care of object creation
    - Can leave class ctor public to enable creation with either approach, or make private to force use of thfactory
- Factory can be external or reside inside the object as an inner nested class
    - If nested class, has access to private members
    - Otherwise, declare as a friend class of the object, can break open-close principle if making factory much later than original object.
- May have hierarchies of factories to create related objectes


## Section 5- Prototype Pattern
About object copying- when easier to copy than create from scratch.

- Complicated objects aren't designed from scratch
- An existing (partially or fully constructed) design is a prototype
- We make a copy (clone) the prototype and customize it
    - Requires deep copy support
- Make cloning convenient (e.g., using a factory)

### Example
Create John Doe and Jane Smith at same address with different suites.
- Create Jane initially as a copy of John, and then modify her address suite.
- Problem: When suite is stored as a pointer, we've also modified John's address
    - This is result of creating a shallow copy
    - We need the ability to create a deep copy to avoid this

Deep copy: 
This can be done by defining a copy ctor

```c++
Contact(const Contact& other)
: name {other.name}, address { new Address {other.address->street, other.address->city, other.address->suite }}
{

}

...

main()
{
    Contact john{/*...*/};
    Contact jane{john};
    jane.name = "Jane Smith";
    name.address->suite = 103;
}
```

Problem: Become brittle when change data members, must remember to update explicit deep copy. Copy assignment not happening within the Address class, so easier to miss changes.

Possible Solution:
- Give Address its own copy ctor

```c++
Address(const Address& address) : street{address.street}, city{address.city}, suite{address.suite}
{}

///

Contact(const Contact& other) : name {other.name}, address {new Address{*other.address}}
{}
```

### Improvement- give the user a prototype to work with (Prototype Factory)
Could create a namespace-local variable of that type as a prototype.
- Works, but not explicit enough to let users know they can make copies of it.

```c++
struct EmployeeFactory
{
    static unique_ptr<Contact> new_main_office_employee(const string& name, int suite)
    {
        static Contact p{"", new Address {"123 East Dr", "London", 0}};
        return new_employee(name, suite, p);
    }

private:
    static unique_ptr<Contact> new_employee(const string& name, int suite, const Contact& prototype)
    {
        auto result = make_unique<Contact>(prototype);
        result->name = name;
        result->address->suite = suite;
        return result;
    }
};

...

main()
{
    EmployeeFactory::new_main_office_employee("John", 123);
}
```

### Remaining pain point- have to implement object copying yourself
Requires copy ctors, assignment operator, or explicit clone method to replicate object entirely.

Solution: user serialization.
- Take all data of an object, serialize.
- Consumer would take serialized data and deserialize.
- This example uses Boost serialization for this.

```c++
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
```

In Contact class:

```c++
struct Contact
{
    ...
    private:
    friend class serialization::access;

    template <class archive>
    void serialize(archive& ar, const unsigned version)
    {
        ar & name;
        ar & address;   // NOTE: don't have to dereference the pointer
    }
}

...

main()
{
    auto clone = [](const Contact& c)
    {
        ostringstream oss;
        archive::text_oarchive oa(oss); // creates string archive
        oa << c;                        // serialize c to an output archive into string representation
        string s = oss.str();
        cout << s << endl;

        // deserialize into the actual object
        istringstream iss(s);
        archive::text_iarchive ia(iss);
        Contact result;
        ia >> result;
        return result;
    };

    auto john = EmployeeFactory::new_main_office_employee("John", 123);
    auto jane = clone(*john);
}
```
NOTE: must also define serialization in Address

Opinion: this is really only beneficial if we plan to serialize anyway. Still have to define 'copy' with serialization definition (althought with some simplicity as pointer copy doesn't need explicit definition)


## Section 6- Singletons
Motivation:
- for some components, in only makes sense to have one instance
    - Database repository
    - Object factory
- e.g., ctor  call is expensive
    - only want to do it once
    we provide everyone with the same instance
- Want to prevent clients from creating additional copies
- Need to take care of lazy instntiation and thread safety

### Singleton Definition
Singleton: comp. which is instantiated only once, preventing from being instantiated more than once

```c++
class SingletonDatabase : public Database
{
  SingletonDatabase()
  {
    std::cout << "Initializing database" << std::endl;

    std::ifstream ifs("capitals.txt");

    std::string s, s2;
    while (getline(ifs, s))
    {
      getline(ifs, s2);
      int pop = boost::lexical_cast<int>(s2);
      capitals[s] = pop;
    }
    //instance_count++;
  }

  std::map<std::string, int> capitals;

public:
  //static int instance_count;

  // NOTE: we delete the copy and assignment operators to ensure
  // we may only have one
  SingletonDatabase(SingletonDatabase const&) = delete;
  void operator=(SingletonDatabase const&) = delete;

  // This is what makes it a Singleton- we've defined the ctor as private (above)
  // and only provide access to it through a static method, which returns a static
  // Singleton object.
  static SingletonDatabase& get()
  {
    static SingletonDatabase db;
    return db;
  }

  int get_population(const std::string& name) override
  {
    return capitals[name];
  }
};

```

### Testability Issues
Given the above `SingletonDatabase`, say we want a struct that operates on it to e.g., get the sum
of the populations for cities it's provided.

```c++
struct SingletonRecordFinder
{
  int total_population(std::vector<std::string> names)
  {
    int result = 0;
    for (auto& name : names)
      result += SingletonDatabase::get().get_population(name);
    return result;
  }
};
```

To test this function, we are forced to use real values from the 'production' database because of the strong coupling of the singleton.

Idea: allow testing of SingletonRecordFinder with a database we supply for testing purposes.

### Singleton with dependency injection
First, we define a pure virtual base class, with virtual method for getting the population (for above example).

```c++
class Database
{
public:
  virtual int get_population(const std::string& name) = 0;
};

```

Now we can have a dummy database

```c++
class DummyDatabase : public Database
{
  std::map<std::string, int> capitals;
public:


  DummyDatabase()
  {
    capitals["alpha"] = 1;
    capitals["beta"] = 2;
    capitals["gamma"] = 3;
  }

  int get_population(const std::string& name) override {
    return capitals[name];
  }
};
```

We need to change the SingletonRecordFinder to have an injectible database

```c++
struct ConfigurableRecordFinder
{
  explicit ConfigurableRecordFinder(Database& db)
    : db{db}
  {
  }

  int total_population(std::vector<std::string> names) const
  {
    int result = 0;
    for (auto& name : names)
      result += db.get_population(name);
    return result;
  }

  Database& db;
};
```
