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


