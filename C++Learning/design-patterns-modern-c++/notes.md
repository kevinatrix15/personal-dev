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
