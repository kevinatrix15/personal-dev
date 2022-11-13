#include "Point.h"

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one file
#include "catch2.h"

#include <string>

namespace {
	void testDist(const std::string& description, const Point& p1, const Point& p2, const size_t expected) {
		SECTION(description) {
			try {
                // arrange & act
                const size_t dist = p1.distance(p2);

                // assert
				REQUIRE(expected == dist);
			} catch (const std::exception & e) {
				FAIL(e.what());
			}
		}
	}
}


TEST_CASE("Point construction and getters are valid", "[ctor]")
{
    // arrange
    Point p(4, 2);

    // act
    const size_t x = p.x();
    const size_t y = p.y();

    // assert
    REQUIRE(4 == x);
    REQUIRE(2 == y);
}

TEST_CASE("Point setters are valid", "setters")
{
    // arrange
    Point p;

    // act
    p.x() = 4;
    p.y() = 2;

    // assert
    REQUIRE(4 == p.x());
    REQUIRE(2 == p.y());
}

TEST_CASE("Distance is valid for diagonal test", "setters")
{
    testDist("coincident1", {0, 0}, {0, 0}, 0);
    testDist("coincident2", {4, 2}, {4, 2}, 0);

    testDist("unit vectors1", {0, 1}, {0, 0}, 1);
    testDist("unit vectors2", {0, 0}, {1, 0}, 1);

    testDist("unit diagonal- flooring", {1, 1}, {0, 0}, 1);
    // (size_t)(5 * sqrt(2)) = (size_t)(7.0711) = 7
    testDist("diagonal- 5*sqrt(2)", {0, 0}, {5, 5}, 7);

    // 3 4 5 triangles
    testDist("3 4 5 triangle", {0, 3}, {4, 0}, 5);
    testDist("3 4 5 triangle- flipped", {3, 0}, {0, 4}, 5);
}