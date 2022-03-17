/*
 * Quaternion.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#include "Quaternion.hpp"

namespace ue
{
  Quaternion::Quaternion(void)
  {
  }

  Quaternion::Quaternion(const Vector3 &axis, Real angle)
  {
    Real angleSin = (Real)sinf((float)(angle * R(0.5)));
    x = axis.x * angleSin;
    y = axis.y * angleSin;
    z = axis.z * angleSin;
    w = (Real)cosf((float)angle);

    this->normalise();
  }

  Quaternion::Quaternion(Vector3 angles)
  {
    angles = angles * R(0.5);

    // Assuming the angles are in radians.
    Real c1 = (Real)cosf((float)angles.y);
    Real s1 = (Real)sinf((float)angles.y);
    Real c2 = (Real)cosf((float)angles.z);
    Real s2 = (Real)sinf((float)angles.z);
    Real c3 = (Real)cosf((float)angles.x);
    Real s3 = (Real)sinf((float)angles.x);
    Real c1c2 = c1 * c2;
    Real s1s2 = s1 * s2;
    w = c1c2 * c3 - s1s2 * s3;
    x = c1c2 * s3 + s1s2 * c3;
    y = s1 * c2 * c3 + c1 * s2 * s3;
    z = c1 * s2 * c3 - s1 * c2 * s3;
  }

  Quaternion::Quaternion(Real angleX, Real angleY, Real angleZ)
  {
    angleX = angleX * R(0.5);
    angleY = angleY * R(0.5);
    angleZ = angleZ * R(0.5);

    // Assuming the angles are in radians.
    Real c1 = (Real)cosf((float)angleY);
    Real s1 = (Real)sinf((float)angleY);
    Real c2 = (Real)cosf((float)angleZ);
    Real s2 = (Real)sinf((float)angleZ);
    Real c3 = (Real)cosf((float)angleX);
    Real s3 = (Real)sinf((float)angleX);
    Real c1c2 = c1 * c2;
    Real s1s2 = s1 * s2;
    w = c1c2 * c3 - s1s2 * s3;
    x = c1c2 * s3 + s1s2 * c3;
    y = s1 * c2 * c3 + c1 * s2 * s3;
    z = c1 * s2 * c3 - s1 * c2 * s3;
  }

  Quaternion::Quaternion(Real w_, Real x_, Real y_, Real z_)
  {
    w = w_;
    x = x_;
    y = y_;
    z = z_;
  }

  Quaternion::Quaternion(const Quaternion &q)
  {
    w = q.w;
    x = q.x;
    y = q.y;
    z = q.z;
  }

  Quaternion Quaternion::operator-()
  {
    Quaternion tmp;
    tmp.w = -w;
    tmp.x = -x;
    tmp.y = -y;
    tmp.z = -z;
    return tmp;
  }

  Quaternion Quaternion::inv() const
  {
    Real s = R(1.0) / (w * w + x * x + y * y + z * z);
    Quaternion tmp;
    tmp.w = w * s;
    tmp.x = -x * s;
    tmp.y = -y * s;
    tmp.z = -z * s;
    return tmp;
  }

  Quaternion Quaternion::operator+(Quaternion &q)
  {
    Quaternion tmp;
    tmp.w = q.w + w;
    tmp.x = q.x + x;
    tmp.y = q.y + y;
    tmp.z = q.z + z;
    return tmp;
  }

  Quaternion Quaternion::operator-(Quaternion &q)
  {
    Quaternion tmp;
    tmp.w = q.w - w;
    tmp.x = q.x - x;
    tmp.y = q.y - y;
    tmp.z = q.z - z;
    return tmp;
  }

  Quaternion Quaternion::operator*(const Quaternion &q) const
  {
    Quaternion tmp;
    tmp.w = w * q.w - x * q.x - y * q.y - z * q.z;
    tmp.x = w * q.x + x * q.w + y * q.z - z * q.y;
    tmp.y = w * q.y - x * q.z + y * q.w + z * q.x;
    tmp.z = w * q.z + x * q.y - y * q.x + z * q.w;
    return tmp;
  }

  Quaternion Quaternion::operator*(Real s)
  {
    Quaternion tmp;
    tmp.w = w * s;
    tmp.x = x * s;
    tmp.y = y * s;
    tmp.z = z * s;
    return tmp;
  }

  Quaternion Quaternion::conj() const
  {
    Quaternion tmp;
    tmp.w = w;
    tmp.x = -x;
    tmp.y = -y;
    tmp.z = -z;
    return tmp;
  }

  Quaternion Quaternion::normalised()
  {
    Real s = R(1.0) / magnitude();
    Quaternion tmp;
    tmp.w = w * s;
    tmp.x = x * s;
    tmp.y = y * s;
    tmp.z = z * s;
    return tmp;
  }

  Real Quaternion::magnitude()
  {
    return (Real)sqrtf((float)(w * w + x * x + y * y + z * z));
  }

  void Quaternion::normalise()
  {
    Real s = R(1.0) / magnitude();
    w *= s;
    x *= s;
    y *= s;
    z *= s;
  }

  Matrix3 Quaternion::rotationMatrix3() const
  {
    Matrix3 matrix;

    matrix[0][0] = x * x + y * y - z * z - w * w;
    matrix[0][1] = R(2.0) * y * z - R(2.0) * x * w;
    matrix[0][2] = R(2.0) * x * z + R(2.0) * y * w;

    matrix[1][0] = R(2.0) * x * w + R(2.0) * y * z;
    matrix[1][1] = x * x - y * y + z * z - w * w;
    matrix[1][2] = R(2.0) * z * w - R(2.0) * x * y;

    matrix[2][0] = R(2.0) * y * w - R(2.0) * x * z;
    matrix[2][1] = R(2.0) * x * y + R(2.0) * z * w;
    matrix[2][2] = x * x - y * y - z * z + w * w;

    return matrix;
  }

  Matrix4 Quaternion::rotationMatrix4()
  {
    Matrix4 matrix;

    matrix[0][0] = x * x + y * y - z * z - w * w;
    matrix[0][1] = R(2.0) * y * z - R(2.0) * x * w;
    matrix[0][2] = R(2.0) * x * z + R(2.0) * y * w;

    matrix[1][0] = R(2.0) * x * w + R(2.0) * y * z;
    matrix[1][1] = x * x - y * y + z * z - w * w;
    matrix[1][2] = R(2.0) * z * w - R(2.0) * x * y;

    matrix[2][0] = R(2.0) * y * w - R(2.0) * x * z;
    matrix[2][1] = R(2.0) * x * y + R(2.0) * z * w;
    matrix[2][2] = x * x - y * y - z * z + w * w;

    // For homogeneous coordinates
    matrix[3][3] = R(1.0);

    // Set the rest to 0
    matrix[3][0] = R(0.0);
    matrix[3][1] = R(0.0);
    matrix[3][2] = R(0.0);

    matrix[0][3] = R(0.0);
    matrix[1][3] = R(0.0);
    matrix[2][3] = R(0.0);

    return matrix;
  }

  Vector3 Quaternion::rotateVector(const Vector3 &v) const
  {
    Quaternion qv(0, v.x, v.y, v.z);

    qv = (*this) * qv * (this->conj());

    Vector3 tmp(qv.x, qv.y, qv.z);
    return tmp;
  }
}
