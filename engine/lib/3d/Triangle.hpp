/*
 * Triangle.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "../../ue_config.hpp"

#include "../arith.hpp"
#include "../linal.hpp"

namespace ue {
  struct Triangle
  {
    //Vertex
    Vector3* va;
    Vector3* vb;
    Vector3* vc;

#if UE_CONFIG_ENABLE_TEXTURE == true
    //Texture vertex
    Vector2* vta;
    Vector2* vtb;
    Vector2* vtc;
#endif

#if UE_CONFIG_ENABLE_NORMAL == true
    //Normal vector
    Vector3* vna;
    Vector3* vnb;
    Vector3* vnc;

#endif
  };

}

#endif /* TRIANGLE_H_ */
