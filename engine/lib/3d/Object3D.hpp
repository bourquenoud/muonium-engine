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
  };

}

#endif /* OBJECT3D_H_ */
