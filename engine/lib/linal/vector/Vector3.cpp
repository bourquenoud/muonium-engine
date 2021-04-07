/*
 * Vector3.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#include "Vector3.hpp"

namespace ue
{
  Vector3 Vector3::operator+(Vector3 b)
  {
    Vector3 c;

    c.x = this->x + b.x;
    c.y = this->y + b.y;
    c.z = this->z + b.z;

    return c;
  }

  Vector3 Vector3::operator-(Vector3 b)
  {
    Vector3 c;

    c.x = this->x - b.x;
    c.y = this->y - b.y;
    c.z = this->z - b.z;

    return c;
  }

  Vector3 Vector3::operator-()
  {
    Vector3 c;

    c.x = -this->x;
    c.y = -this->y;
    c.z = -this->z;

    return c;
  }

  /*
   * Return the vector3 multiplied by a scalar
   */
   Vector3 Vector3::operator*(Real scal)
   {
     Vector3 c;

     c.x = this->x * scal;
     c.y = this->y * scal;
     c.z = this->z * scal;

     return c;
   }

   /*
    * Return the norm of a vector 3
    */
   Real Vector3::norm()
   {
     return sqrtf(this->x*this->x + this->y*this->y + this->z*this->z);
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

