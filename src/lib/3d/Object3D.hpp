/*
 * Object3D.h
 *
 *  Created on: 8 Apr 2021
 *      Author: mathieu
 */

#ifndef OBJECT3D_H_
#define OBJECT3D_H_

#include "../linal.hpp"

namespace ue
{

  class Object3D
  {
  public:
    Quaternion rotation;
    Vector3 position;
    inline Vector3 getForwardVector() const
    {
    	return rotation.rotationMatrix3() * Vector3::Z;
    }
    inline Vector3 getUpVector() const
    {
    	return rotation.rotationMatrix3() * Vector3::Y;
    }
    inline Vector3 getRightVector() const
    {
    	return rotation.rotationMatrix3() * Vector3::X;
    }
  };

}

#endif /* OBJECT3D_H_ */
