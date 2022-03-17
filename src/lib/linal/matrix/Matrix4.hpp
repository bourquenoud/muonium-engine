/*
 * Matrix4.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef MATRIX4_H_
#define MATRIX4_H_

#include "../../arith.hpp"
#include "../vector/Vector3.hpp"
#include "../vector/Vector4.hpp"

namespace ue {

  class Matrix4
  {
  public:
    Real d[4][4];
    Matrix4 operator-();
    Matrix4 operator+(Matrix4& B);
    Matrix4 operator-(Matrix4& B);
    Matrix4 operator*(Matrix4& B);
    Vector4 operator*(Vector4& v);

    //TODO: create an homogeneous coordinates matrix class to reduce memory usage
    /**
     * Compute the product as homogeneous coordinates
     *
     * @param v input vector
     *
     * @return transformed vector
     */
    Vector3 operator*(Vector3& v);

    Real* operator[](int);
  };


}

#endif /* MATRIX4_H_ */
