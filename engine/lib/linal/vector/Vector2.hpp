/*
 * Vector2.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef VECTOR2_H_
#define VECTOR2_H_

#include <cmath>
#include "../../arith.hpp"

namespace ue {

  class Vector2
  {
  public:
    Real x;
    Real y;

    Vector2();
    Vector2(Real,Real);

    Vector2 operator+(Vector2 b);
    Vector2 operator-(Vector2 b);
    Vector2 operator-();
    Vector2 operator*(Real val);
    Real operator*(Vector2 b); //Dot product

    bool operator==(Vector2);

    Real norm(void);
    Vector2 normalise(void);

    static const Vector2 ZERO;
    static const Vector2 ONE;
    static const Vector2 X;
    static const Vector2 Y;
  };

}

#endif /* VECTOR2_H_ */
