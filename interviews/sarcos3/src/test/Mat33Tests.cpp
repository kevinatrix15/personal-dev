/**
 * @file Mat33Tests.h
 * @brief Unit tests for the 3x3 matrix class.
 * @author Kevin Briggs <kevinabriggs@hotmail.com>
 * @version 1
 * @date 2022-10-18
 */
#include "Mat33.h"
#include "catch2.h"

/******************************************************************************
* UNIT TESTS ******************************************************************
******************************************************************************/

TEST_CASE("Mat33 assignment matches inputs", "[ctor]")
{
  // arrange
  Vec3 v0{1.0, 2.0, 3.0};
  Vec3 v1{4.0, 5.0, 6.0};
  Vec3 v2{7.0, 8.0, 9.0};

  // act
  Mat33 m({v0, v1, v2});
  const auto cols = m.get();

  // assert
  REQUIRE(v0 == cols[0]);
  REQUIRE(v1 == cols[1]);
  REQUIRE(v2 == cols[2]);
}

TEST_CASE("Matrix transpose of identity matrix appears unchanged",
          "[transpose]")
{
  // arrange
  Vec3 v0{1.0, 0.0, 0.0};
  Vec3 v1{0.0, 1.0, 0.0};
  Vec3 v2{0.0, 0.0, 1.0};

  // act
  Mat33 I({v0, v1, v2});
  I.transpose();
  const auto cols = I.get();

  // assert
  REQUIRE(v0 == cols[0]);
  REQUIRE(v1 == cols[1]);
  REQUIRE(v2 == cols[2]);
}

TEST_CASE("Matrix transpose of unique matrix matches pre-calculated",
          "[transpose]")
{
  // arrange
  Vec3 v0{1.0, 2.0, 3.0};
  Vec3 v1{4.0, 5.0, 6.0};
  Vec3 v2{7.0, 8.0, 9.0};

  // act
  Mat33 m({v0, v1, v2});
  m.transpose();
  const auto cols = m.get();

  // assert
  REQUIRE(v0.x == cols[0].x);
  REQUIRE(v0.y == cols[1].x);
  REQUIRE(v0.z == cols[2].x);

  REQUIRE(v1.x == cols[0].y);
  REQUIRE(v1.y == cols[1].y);
  REQUIRE(v1.z == cols[2].y);

  REQUIRE(v2.x == cols[0].z);
  REQUIRE(v2.y == cols[1].z);
  REQUIRE(v2.z == cols[2].z);
}

TEST_CASE(
    "Deep copy ensures original matrix is unchanged when copy is modified",
    "[transpose]")
{
  // arrange
  Vec3 v0{1.0, 2.0, 3.0};
  Vec3 v1{4.0, 5.0, 6.0};
  Vec3 v2{7.0, 8.0, 9.0};

  // act
  Mat33 m0({v0, v1, v2});
  Mat33 m1 = m0;
  m1.transpose();

  const auto cols0 = m0.get();
  const auto cols1 = m1.get();

  // assert
  // check that the columns of m0 are unchanged
  REQUIRE(v0 == cols0[0]);
  REQUIRE(v1 == cols0[1]);
  REQUIRE(v2 == cols0[2]);
}
