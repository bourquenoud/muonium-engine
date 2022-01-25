/*
 * Quaternion.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include <cmath>

#include "../../arith.hpp"
#include "../vector/Vector3.hpp"
#include "../matrix/Matrix3.hpp"
#include "../matrix/Matrix4.hpp"

namespace ue {

  class Quaternion {
  public:
    Real w;
    Real x;
    Real y;
    Real z;

    /**
     * Default constructor
     */
    Quaternion(void);

    /**
     * Constructor from a vector and an angle
     *
     * Construct the quaternion from a NORMALISED vector
     * indicating the axis and the angle of the rotation.
     *
     * Will break if the vector is not normalised
     *
     * @param axis NORMALISED vector of this rotation axis.
     * @param angle rotation angle in radian
     */
    Quaternion(const Vector3& axis,Real angle);

    /**
     * Constructor from euler angles
     *
     * Construct a quaternion from the euleur angles.
     * Applied in the order of y, z, x
     *
     * @param angles Euler angles in radians
     */
    Quaternion(Vector3 angles);

    /**
     * Constructor from euler angles
     *
     * Construct a quaternion from the euleur angles.
     * Applied in the order of y, z, x
     *
     * @param angles Euler angles in radians
     */
    Quaternion(Real angleX, Real angleY, Real angleZ);

    /**
     * Constructor with the values
     *
     * Construct a quaternion from the w x y z values
     *
     * @param w_ quaternion real part
     * @param x_ quaternion i imaginary part
     * @param y_ quaternion j imaginary part
     * @param z_ quaternion k imaginary part
     */
    Quaternion(Real w_,Real x_,Real y_,Real z_);

    /**
     * Constructor from quaternion
     *
     * Clone a quaternion.
     *
     * @param q Quaternion to clone
     */
    Quaternion(const Quaternion& q);


    Quaternion operator-();
    Quaternion operator+(Quaternion&);
    Quaternion operator-(Quaternion&);
    Quaternion operator*(const Quaternion& q) const;
    Quaternion operator*(Real);


    /**
     * Calculate the inverse
     *
     * @return inverse of the quaternion
     */
    Quaternion inv() const;

    /**
     * Calculate the conjugate.
     *
     * This will compute the conjugate (w - xi - yj - zk)
     * Use this to compute the inverse if the quaternion is normalised.
     *
     * @return quaternion conjugate
     */
    Quaternion conj() const;

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

    /**
     * Compute the rotation matrix
     *
     * Return the rotation matrix 3x3 computed from the quaternion
     *
     * @return rotation matrix
     */
    Matrix3 rotationMatrix3() const;

    /**
     * Compute the 4x4 rotation matrix
     *
     * Return the rotation matrix 4x4 computed from the quaternion,
     * as homogeneous coordinates
     *
     * @return rotation matrix
     */
    Matrix4 rotationMatrix4();

    /**
     * Rotate a given vector
     */
    Vector3 rotateVector(const Vector3&) const;
  };

}

#endif /* QUATERNION_H_ */
