/*
 * Matrix3.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#include "Matrix3.hpp"

namespace ue
{
  Matrix3 Matrix3::operator-()
  {
    Matrix3 C;

    C.d[0][0] = -d[0][0];
    C.d[0][1] = -d[0][1];
    C.d[0][2] = -d[0][2];

    C.d[1][0] = -d[1][0];
    C.d[1][1] = -d[1][1];
    C.d[1][2] = -d[1][2];

    C.d[2][0] = -d[2][0];
    C.d[2][1] = -d[2][1];
    C.d[2][2] = -d[2][2];

    return C;
  }
  Matrix3 Matrix3::operator+(Matrix3 &B)
  {
    Matrix3 C;

    C.d[0][0] = d[0][0] + B.d[0][0];
    C.d[0][1] = d[0][1] + B.d[0][1];
    C.d[0][2] = d[0][2] + B.d[0][2];

    C.d[1][0] = d[1][0] + B.d[1][0];
    C.d[1][1] = d[1][1] + B.d[1][1];
    C.d[1][2] = d[1][2] + B.d[1][2];

    C.d[2][0] = d[2][0] + B.d[2][0];
    C.d[2][1] = d[2][1] + B.d[2][1];
    C.d[2][2] = d[2][2] + B.d[2][2];

    return C;
  }
  Matrix3 Matrix3::operator-(Matrix3 &B)
  {
    Matrix3 C;

    C.d[0][0] = d[0][0] - B.d[0][0];
    C.d[0][1] = d[0][1] - B.d[0][1];
    C.d[0][2] = d[0][2] - B.d[0][2];

    C.d[1][0] = d[1][0] - B.d[1][0];
    C.d[1][1] = d[1][1] - B.d[1][1];
    C.d[1][2] = d[1][2] - B.d[1][2];

    C.d[2][0] = d[2][0] - B.d[2][0];
    C.d[2][1] = d[2][1] - B.d[2][1];
    C.d[2][2] = d[2][2] - B.d[2][2];

    return C;
  }
  /*
   * Calculate the product of matrix 3x3 A and B
   */
  Matrix3 Matrix3::operator*(Matrix3 &B)
  {
    Matrix3 C;

    // Unfolded, faster

    C.d[0][0] = d[0][0] * B.d[0][0] + d[0][1] * B.d[1][0] + d[0][2] * B.d[2][0];
    C.d[0][1] = d[0][0] * B.d[0][1] + d[0][1] * B.d[1][1] + d[0][2] * B.d[2][1];
    C.d[0][2] = d[0][0] * B.d[0][2] + d[0][1] * B.d[1][2] + d[0][2] * B.d[2][2];

    C.d[1][0] = d[1][0] * B.d[0][0] + d[1][1] * B.d[1][0] + d[1][2] * B.d[2][0];
    C.d[1][1] = d[1][0] * B.d[0][1] + d[1][1] * B.d[1][1] + d[1][2] * B.d[2][1];
    C.d[1][2] = d[1][0] * B.d[0][2] + d[1][1] * B.d[1][2] + d[1][2] * B.d[2][2];

    C.d[2][0] = d[2][0] * B.d[0][0] + d[2][1] * B.d[1][0] + d[2][2] * B.d[2][0];
    C.d[2][1] = d[2][0] * B.d[0][1] + d[2][1] * B.d[1][1] + d[2][2] * B.d[2][1];
    C.d[2][2] = d[2][0] * B.d[0][2] + d[2][1] * B.d[1][2] + d[2][2] * B.d[2][2];

    return C;
  }

  Vector3 Matrix3::operator*(const Vector3 &v) const
  {
    Vector3 u;

    u.x = v.x * d[0][0] + v.y * d[0][1] + v.z * d[0][2];
    u.y = v.x * d[1][0] + v.y * d[1][1] + v.z * d[1][2];
    u.z = v.x * d[2][0] + v.y * d[2][1] + v.z * d[2][2];

    return u;
  }

  Real *Matrix3::operator[](int i)
  {
    return d[i];
  }
}
