/*
 * Matrix2.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef MATRIX2_H_
#define MATRIX2_H_

#include "../../arith.hpp"
#include "../vector/Vector2.hpp"

namespace ue {

  class Matrix2
  {
  public:
    Real d[2][2];
    Matrix2 operator-();
    Matrix2 operator+(Matrix2& B);
    Matrix2 operator-(Matrix2& B);
    Matrix2 operator*(Matrix2& B);
    Vector2 operator*(Vector2& v);
    Real* operator[](int);

  };

}

#endif /* MATRIX2_H_ */
