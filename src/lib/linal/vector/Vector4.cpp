/*
 * Vector4.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#include "Vector4.hpp"

namespace ue
{
  Vector4::Vector4()
  {
  }

  Vector4::Vector4(Real x_, Real y_, Real z_, Real w_)
  {
    x = x_;
    y = y_;
    z = z_;
    w = w_;
  }

  Vector4 Vector4::operator+(Vector4 b)
  {
    Vector4 c;

    c.w = this->w + b.w;
    c.x = this->x + b.x;
    c.y = this->y + b.y;
    c.z = this->z + b.z;

    return c;
  }

  Vector4 Vector4::operator-(Vector4 b)
  {
    Vector4 c;

    c.w = this->w - b.w;
    c.x = this->x - b.x;
    c.y = this->y - b.y;
    c.z = this->z - b.z;

    return c;
  }

  Vector4 Vector4::operator-()
  {
    Vector4 c;

    c.w = -this->w;
    c.x = -this->x;
    c.y = -this->y;
    c.z = -this->z;

    return c;
  }

  Real Vector4::operator*(Vector4 b)
  {
    return this->w * b.w + this->x * b.x + this->y * b.y + this->z * b.z;
  }

  /*
   * Return the vector3 multiplied by a scalar
   */
  Vector4 Vector4::operator*(Real scal)
  {
    Vector4 c;

    c.w = this->w * scal;
    c.x = this->x * scal;
    c.y = this->y * scal;
    c.z = this->z * scal;

    return c;
  }

  /*
   * Return the norm of a vector 4
   */
  Real Vector4::norm()
  {
    return sqrtf((float)(this->w * this->w + this->x * this->x + this->y * this->y + this->z * this->z));
  }

  /*
   * Normalise a vector4
   */
  Vector4 Vector4::normalise()
  {
    Real norm = this->norm();

    if (norm == R(0.0))
      return *this;

    return *this * (R(1.0) / norm);
  }

  /*
   * Fast very approximate normalisation
   */
  Vector4 Vector4::approxNormalise()
  {
    Real norm = Real::abs(w) + Real::abs(x) + Real::abs(y) + Real::abs(z);

    if (norm == R(0.0))
      return *this;

    return *this * (R(1.0) / norm);
  }

  bool Vector4::operator==(Vector4 b)
  {
    return (w == b.w && x == b.x && y == b.y && z == b.z);
  }

  //****Constants****
  const Vector4 Vector4::ZERO = {0, 0, 0, 0};
  const Vector4 Vector4::ONE = {1, 1, 1, 1};
  const Vector4 Vector4::X = {1, 0, 0, 0};
  const Vector4 Vector4::Y = {0, 1, 0, 0};
  const Vector4 Vector4::Z = {0, 0, 1, 0};
  const Vector4 Vector4::W = {0, 0, 0, 1};
}
