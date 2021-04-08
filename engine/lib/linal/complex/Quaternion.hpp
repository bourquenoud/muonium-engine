/*
 * Quaternion.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "../../arith.hpp"

namespace ue {

  class Quaternion {
  public:
    Real w;
    Real x;
    Real y;
    Real z;

    Quaternion operator-();
    Quaternion operator~(); //Calculate the inverse
    Quaternion operator+(Quaternion);
    Quaternion operator-(Quaternion);
    Quaternion operator*(Quaternion);
    Quaternion operator*(Real);

    /**
     * Calculate the conjugate.
     *
     * This will compute the conjugate (w - xi - yj - zk)
     * Use this to compute the inverse if the quaternion is normalised.
     *
     * @return quaternion conjugate
     */
    Quaternion conj();

    /**
     * Calculate the normalised quaternion.
     *
     * Calculate the normalised quaternion and return it.
     * Use "normalise()" to avoid allocating a new quaternion.
     *
     *  @return normalised quaternion
     */
    Quaternion normalised();

    /**
     * Calculate the magnitude
     *
     * Calculate the quaternion magnitude.
     *
     * @return quaternion magnitude
     */
    Real magnitude();

    /**
     * Normalise the quaternion
     *
     * Normalise the quaternion. Using this will normalise
     * the quaternion without allocating a new quaternion memory.
     */
    void normalise();
  };

}

#endif /* QUATERNION_H_ */
