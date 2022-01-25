/*
 * CubeMap.cpp
 *
 *  Created on: 6 Sep 2021
 *      Author: mathieu
 */

#include "CubeMap.hpp"

namespace ue {

  Colour CubeMap::getPixelAt(Vector3& direction)
  {
    int index;
    Vector2 uvCoordinates(CubeMap::samplingCoordinates(direction, index));

    return textures[index].getPixelAt(uvCoordinates);
  }

}
