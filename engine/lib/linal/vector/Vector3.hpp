/*
 * Vector3.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <cmath>
#include "../../arith.hpp"

namespace ue {

  class Vector3
  {
  public:
    Real x;
    Real y;
    Real z;

    Vector3();
    Vector3(Real,Real,Real);

    Vector3 operator+(Vector3 b);
    Vector3 operator-(Vector3 b);
    Vector3 operator-();
    Vector3 operator*(Real val);
    Real operator*(Vector3 b); //Dot product

    bool operator==(Vector3);

    Vector3 cross(Vector3 b);
    Real norm(void);
    Vector3 normalise(void);

    static const Vector3 ZERO;
    static const Vector3 ONE;
    static const Vector3 X;
    static const Vector3 Y;
    static const Vector3 Z;
  };

}

#endif /* VECTOR3_H_ */
