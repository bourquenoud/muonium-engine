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

namespace ue
{

  class Vector3
  {
  public:
    Real x;
    Real y;
    Real z;

    // TODO: set everything to const in other VectorX.hpp
    Vector3();
    Vector3(const Vector3 &);
    Vector3(const Real &, const Real &, const Real &);

    Vector3 operator+(const Vector3 b) const;
    Vector3 operator-(const Vector3 b) const;
    Vector3 operator-() const;
    Vector3 operator*(const Real val) const;
    Real operator*(const Vector3 b) const; // Dot product

    bool operator==(const Vector3);

    Vector3 cross(const Vector3 b);
    Real norm(void);
    Vector3 normalise(void);
    Vector3 approxNormalise(void);

    static const Vector3 ZERO;
    static const Vector3 ONE;
    static const Vector3 X;
    static const Vector3 Y;
    static const Vector3 Z;
  };

}

#endif /* VECTOR3_H_ */
