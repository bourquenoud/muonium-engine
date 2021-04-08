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

  Quaternion::Quaternion(Vector3 axis,Real angle)
  {
    Real angleSin = sinf(angle/R(2));
    x = axis.x * angleSin;
    y = axis.y * angleSin;
    z = axis.z * angleSin;
    w = cosf(angle);
  }

  Quaternion::Quaternion(Real w_,Real x_,Real y_,Real z_)
  {
    w = w_;
    x = x_;
    y = y_;
    z = z_;
  }

  Quaternion::Quaternion(const Quaternion& q)
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

  Quaternion Quaternion::operator~()
          {
    Real s = R(1.0) / (w*w + x*x + y*y + z*z);
    Quaternion tmp;
    tmp.w = w * s;
    tmp.x = -x * s;
    tmp.y = -y * s;
    tmp.z = -z * s;
    return tmp;
          }

  Quaternion Quaternion::operator+(Quaternion q)
  {
    Quaternion tmp;
    tmp.w = q.w + w;
    tmp.x = q.x + x;
    tmp.y = q.y + y;
    tmp.z = q.z + z;
    return tmp;
  }
  Quaternion Quaternion::operator-(Quaternion q)
  {
    Quaternion tmp;
    tmp.w = q.w - w;
    tmp.x = q.x - x;
    tmp.y = q.y - y;
    tmp.z = q.z - z;
    return tmp;
  }
  Quaternion Quaternion::operator*(Quaternion q)
  {
    Quaternion tmp;
    tmp.w = w*q.w - x*q.x - y*q.y - z*q.z;
    tmp.x = y*q.w + w*q.x + y*q.z - z*q.y;
    tmp.y = w*q.y - x*q.z + y*q.w + z*q.x;
    tmp.z = w*q.z + x*q.y - y*q.x + z*q.w;
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

  Quaternion Quaternion::conj()
  {
    Quaternion tmp;
    tmp.w = w;
    tmp.x = -x;
    tmp.y = -y;
    tmp.z = -z;
    return tmp;
  }

  Quaternion  Quaternion::normalised()
  {
    Real s = R(1.0)/magnitude();
    Quaternion tmp;
    tmp.w = w * s;
    tmp.x = x * s;
    tmp.y = y * s;
    tmp.z = z * s;
    return tmp;
  }

  Real  Quaternion::magnitude()
  {
    return sqrt(w*w + x*x + y*y + z*z);
  }

  void  Quaternion::normalise()
  {
    Real s = R(1.0)/magnitude();
    w *= s;
    x *= s;
    y *= s;
    z *= s;
  }
}
