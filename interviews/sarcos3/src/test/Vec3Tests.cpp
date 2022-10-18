/**
 * @file Vec3Tests.h
 * @brief Unit tests for the three-component vector class.
 * @author Kevin Briggs <kevinabriggs@hotmail.com>
 * @version 1
 * @date 2022-10-18
 */
#include "Vec3.h"

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "catch2.h"

/******************************************************************************
* UNIT TESTS ******************************************************************
******************************************************************************/

TEST_CASE("Vec3 assignment matches inputs", "[ctor]")
{
  Vec3 v{1.0, 2.0, 3.0};
  REQUIRE(1.0 == v.x);
  REQUIRE(2.0 == v.y);
  REQUIRE(3.0 == v.z);
}

TEST_CASE("Orthogonal dot products are zero", "[dot]")
{
  // arrange
  Vec3 v1{1.0, 0.0, 0.0};
  Vec3 v2{0.0, 1.0, 0.0};
  Vec3 v3{0.0, 0.0, 1.0};

  // act & assert
  REQUIRE(0.0 == v1.dot(v2));
  REQUIRE(0.0 == v2.dot(v1));

  REQUIRE(0.0 == v1.dot(v3));
  REQUIRE(0.0 == v3.dot(v1));

  REQUIRE(0.0 == v2.dot(v3));
  REQUIRE(0.0 == v3.dot(v2));
}

TEST_CASE("Dot products of two coincident unit vectors are one", "[dot]")
{
  // arrange
  Vec3 v1{0.0, 1.0, 0.0};
  Vec3 v2{0.0, 1.0, 0.0};

  // act & assert
  REQUIRE(1.0 == v1.dot(v2));
  REQUIRE(1.0 == v2.dot(v1));
}

TEST_CASE("Dot products of opposite unit vectors are negative one", "[dot]")
{
  // arrange
  Vec3 v1{0.0, -1.0, 0.0};
  Vec3 v2{0.0, 1.0, 0.0};

  // act & assert
  REQUIRE(-1.0 == v1.dot(v2));
  REQUIRE(-1.0 == v2.dot(v1));
}

TEST_CASE("Dot product between arbitrary vectors match pre-calculated result",
          "[dot]")
{
  // arrange
  Vec3 v1{1.0, 2.0, 3.0};
  Vec3 v2{4.0, 5.0, 6.0};

  // act & assert
  // res = ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z))
  //     = ((1.0 * 4.0) + (2.0 * 5.0) + (3.0 * 6.0))
  //     = (4 + 10 + 18)
  //     = 32
  REQUIRE(32.0 == v1.dot(v2));
}
