/*
 * Vector4.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef VECTOR4_H_
#define VECTOR4_H_

#include <cmath>
#include "../../arith.hpp"

namespace ue
{

  class Vector4
  {
  public:
    Real x;
    Real y;
    Real z;
    Real w;

    Vector4();
    Vector4(Real, Real, Real, Real);

    Vector4 operator+(Vector4 b);
    Vector4 operator-(Vector4 b);
    Vector4 operator-();
    Vector4 operator*(Real val);
    Real operator*(Vector4 b); // Dot product

    bool operator==(Vector4);

    Real norm(void);
    Vector4 normalise(void);
    Vector4 approxNormalise(void);

    static const Vector4 ZERO;
    static const Vector4 ONE;
    static const Vector4 X;
    static const Vector4 Y;
    static const Vector4 Z;
    static const Vector4 W;
  };

}

#endif /* VECTOR4_H_ */
