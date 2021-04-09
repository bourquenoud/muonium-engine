/*
 * Vector3.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#include "Vector3.hpp"

namespace ue
{
  Vector3::Vector3()
  {

  }

  Vector3::Vector3(Real x_, Real y_, Real z_)
  {
    x = x_;
    y = y_;
    z = z_;
  }

  Vector3 Vector3::operator+(Vector3 b)
  {
    Vector3 c;

    c.x = x + b.x;
    c.y = y + b.y;
    c.z = z + b.z;

    return c;
  }

  Vector3 Vector3::operator-(Vector3 b)
  {
    Vector3 c;

    c.x = x - b.x;
    c.y = y - b.y;
    c.z = z - b.z;

    return c;
  }

  Vector3 Vector3::operator-()
  {
    Vector3 c;

    c.x = -x;
    c.y = -y;
    c.z = -z;

    return c;
  }

  /*
   * Return the vector3 multiplied by a scalar
   */
  Vector3 Vector3::operator*(Real scal)
  {
    Vector3 c;

    c.x = x * scal;
    c.y = y * scal;
    c.z = z * scal;

    return c;
  }

  /*
   * Return the dot product
   */
  Real Vector3::operator*(Vector3 b)
  {
    return x*b.x + y*b.y + z*b.z;
  }

  /*
   * Return the cross product
   */
  Vector3 Vector3::cross(Vector3 b)
  {
    Vector3 s1;

    s1.x = y*b.z - z*b.y;
    s1.y = z*b.x - x*b.z;
    s1.z = x*b.y - y*b.x;

    return s1;
  }

  /*
   * Return the norm of a vector 3
   */
  Real Vector3::norm()
  {
    return sqrtf(x*x + y*y + z*z);
  }

  /*
   * Normalise a vector3
   */
  Vector3 Vector3::normalise()
  {
    Real norm = this->norm();

    if(norm == R(0.0))
      return *this;

    return *this * (R(1.0)/norm);
  }

  bool Vector3::operator==(Vector3 b)
               {
    return(x==b.x && y==b.y && z==b.z);
               }

  //****Constants****
  const Vector3 Vector3::ZERO = {0,0,0};
  const Vector3 Vector3::ONE = {1,1,1};
  const Vector3 Vector3::X = {1,0,0};
  const Vector3 Vector3::Y = {0,1,0};
  const Vector3 Vector3::Z = {0,0,1};
}

