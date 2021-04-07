/*
 * Matrix3.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef MATRIX3_H_
#define MATRIX3_H_

#include "../../arith.hpp"
#include "../vector/Vector3.hpp"

namespace ue {

  class Matrix3
  {
  public:
    //Matrix data
    Real d[3][3];
    Matrix3 operator-();
    Matrix3 operator+(Matrix3 B);
    Matrix3 operator-(Matrix3 B);
    Matrix3 operator*(Matrix3 B);
    Vector3 operator*(Vector3 v);
    Real* operator[](int);
  };

}

#endif /* MATRIX3_H_ */
