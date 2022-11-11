# Sarcos Coding Quiz

## Intro
Implement the following problems in C++. Solutions should be submitted with adequate demonstration of
functionality, a corresponding build script ( make , cmake , qmake , ninja , etc…), and will be evaluated for:
* Correctness
    * Is the submitted answer correct?
* Formatting
    * Regardless of style, is it consistent?
* Commenting/Readability
    * Is the code "Self Documenting"?
    * Are the non-obvious sections clearly commented?
* Testing
    * Did they write their own unit tests to develop their function

## Problems
1. Given the following vector definition, write a function that computes the dot product of two vectors.
2. Write a function that pretty-prints a Vec3
3. Given the additional matrix definition, write a function that takes a matrix and transposes the matrix _in place_.
4. Write a function that pretty-prints a Mat33.
5. Write a function that returns a deep copy of a given Mat33.
6. Using the given Node definition, write a function that pretty-prints the data member of a given Node and all of its descendants (in any order).

## Solutions
Solutions to each of the above problems are demonstrated in the code under `src/Main.cpp`, as well as in the unit tests under `src/test/`. Implementations of the solutions are found in the code under `src/`, in `Vec3.h`, `Mat33.h`, and `Node.h`.

## Building
A build shell script (`build.sh`) has been provided for convenience. The usage is provided if run with the `-h` / `--help` option, but in general it should be sufficient to run as follows:
```
./build.sh
```
Note that the build artifacts may be found under `build/` (created by `build.sh`), and the generated binary (`coding-quiz`) and test binary (`coding-quiz.Test`) will be under `build/bin/`.

## Testing
The [Catch testing framework](https://github.com/catchorg/Catch2) is used in this project for its simplicity (header-only library). Dependencies are located in `catch/`. The unit test files for this project are found under the `src/test/` directory, and CMake is configured to build the test library. The unit test binary is generated under `build/bin/` as `coding-quiz.Test`, and may be run with the following:
```
./coding-quiz.Test
```

## Code Documentation
As directed, in-code documentation was added on an as-needed basis where the code may have needed more explanation / context, but was left otherwise sparse to avoid cluttering the code. 

## Future Work
Some enhancements were identified during development which were not included in this exercise for sake of time. These were called out with a `TODO: FUTURE WORK` comment.