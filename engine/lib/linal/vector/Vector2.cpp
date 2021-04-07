/*
 * Vector2.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#include "Vector2.hpp"

namespace ue
{

  Vector2 Vector2::operator+(Vector2 b)
  {
    Vector2 c;

    c.x = this->x + b.x;
    c.y = this->y + b.y;

    return c;
  }

  Vector2 Vector2::operator-(Vector2 b)
  {
    Vector2 c;

    c.x = this->x - b.x;
    c.y = this->y - b.y;

    return c;
  }

  Vector2 Vector2::operator-()
  {
    Vector2 c;

    c.x = -this->x;
    c.y = -this->y;

    return c;
  }

  Real Vector2::operator*(Vector2 b)
  {
    return this->x*b.x + this->y*b.y;
  }

  /*
   * Return the vector2 multiplied by a scalar
   */
  Vector2 Vector2::operator*(Real scal)
  {
    Vector2 c;

    c.x = this->x * scal;
    c.y = this->y * scal;

    return c;
  }

  /*
   * Return the norm of a vector 4
   */
  Real Vector2::norm()
  {
    return sqrtf(this->x*this->x + this->y*this->y);
  }

  /*
   * Normalise a vector2
   */
  Vector2 Vector2::normalise()
  {
    Real norm = this->norm();

    if(norm == R(0.0))
      return *this;

    return *this * (R(1.0)/norm);
  }

  bool Vector2::operator==(Vector2 b)
  {
    return(x==b.x && y==b.y);
  }

  //****Constants****
  const Vector2 Vector2::ZERO = {0,0};
  const Vector2 Vector2::ONE = {1,1};
  const Vector2 Vector2::X = {1,0};
  const Vector2 Vector2::Y = {0,1};
}
