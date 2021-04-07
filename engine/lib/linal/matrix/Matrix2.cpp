/*
 * Matrix2.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#include "Matrix2.hpp"

namespace ue
{
  Matrix2 Matrix2::operator-()
  {
    Matrix2 C;

    C.d[0][0] = -d[0][0];
    C.d[0][1] = -d[0][1];

    C.d[1][0] = -d[1][0];
    C.d[1][1] = -d[1][1];

    return C;
  }
  Matrix2 Matrix2::operator+(Matrix2 B)
  {
    Matrix2 C;

    C.d[0][0] = d[0][0] + B.d[0][0];
    C.d[0][1] = d[0][1] + B.d[0][1];

    C.d[1][0] = d[1][0] + B.d[1][0];
    C.d[1][1] = d[1][1] + B.d[1][1];

    return C;
  }
  Matrix2 Matrix2::operator-(Matrix2 B)
  {
    Matrix2 C;

    C.d[0][0] = d[0][0] - B.d[0][0];
    C.d[0][1] = d[0][1] - B.d[0][1];

    C.d[1][0] = d[1][0] - B.d[1][0];
    C.d[1][1] = d[1][1] - B.d[1][1];

    return C;
  }

  /*
   * Calculate the product of matrix 2x2 A and B
   */
  Matrix2 Matrix2::operator*(Matrix2 B)
  {
    Matrix2 C;

    //Unfolded, faster

    C.d[0][0] = d[0][0]*B.d[0][0] + d[0][1]*B.d[1][0];
    C.d[0][1] = d[0][0]*B.d[0][1] + d[0][1]*B.d[1][1];


    C.d[1][0] = d[1][0]*B.d[0][0] + d[1][1]*B.d[1][0];
    C.d[1][1] = d[1][0]*B.d[0][1] + d[1][1]*B.d[1][1];

    return C;
  }

  Vector2 Matrix2::operator*(Vector2 v)
  {
    Vector2 u;

    u.x = v.x * d[0][0] + v.y * d[0][1];
    u.y = v.x * d[1][0] + v.y * d[1][1];

    return u;
  }

  Real* Matrix2::operator[](int i)
  {
    return d[i];
  }
}
