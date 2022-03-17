/*
 * CubeMap.hpp
 *
 *  Created on: 6 Sep 2021
 *      Author: mathieu
 */

#ifndef LIB_TEXTURE_CUBEMAP_HPP_
#define LIB_TEXTURE_CUBEMAP_HPP_

#include "Colour.hpp"
#include "Texture.hpp"
#include "../arith.hpp"
#include "../linal.hpp"

namespace ue
{
  class CubeMap
  {
  public:
    Texture textures[6];

    Colour getPixelAt(Vector3& direction);

    inline Vector2 samplingCoordinates(Vector3& direction, int& outFaceIndex)
    {
      Vector3 vAbs = Vector3(
          Real::abs(direction.x),
          Real::abs(direction.y),
          Real::abs(direction.z)
      );

      Vector2 uv(R(0.0), R(0.0));

      int isXPositive = direction.x > R(0.0) ? 1 : 0;
      int isYPositive = direction.y > R(0.0) ? 1 : 0;
      int isZPositive = direction.z > R(0.0) ? 1 : 0;

      Real maxAxis;

      // POSITIVE X
      if (isXPositive && vAbs.x >= vAbs.y && vAbs.x >= vAbs.z) {
          // u (0 to 1) goes from +z to -z
          // v (0 to 1) goes from -y to +y
          maxAxis = vAbs.x;
          uv.x = -direction.z;
          uv.y = -direction.y;
          outFaceIndex = 0;
      }
      // NEGATIVE X
      if (!isXPositive && vAbs.x >= vAbs.y && vAbs.x >= vAbs.z) {
          // u (0 to 1) goes from -z to +z
          // v (0 to 1) goes from -y to +y
          maxAxis = vAbs.x;
          uv.x = direction.z;
          uv.y = -direction.y;
          outFaceIndex = 1;
      }
      // POSITIVE Y
      if (isYPositive && vAbs.y >= vAbs.x && vAbs.y >= vAbs.z) {
          // u (0 to 1) goes from -x to +x
          // v (0 to 1) goes from +z to -z
          maxAxis = vAbs.y;
          uv.x = direction.x;
          uv.y = direction.z;
          outFaceIndex = 2;
      }
      // NEGATIVE Y
      if (!isYPositive && vAbs.y >= vAbs.x && vAbs.y >= vAbs.z) {
          // u (0 to 1) goes from -x to +x
          // v (0 to 1) goes from -z to +z
          maxAxis = vAbs.y;
          uv.x = direction.x;
          uv.y = -direction.z;
          outFaceIndex = 3;
      }
      // POSITIVE Z
      if (isZPositive && vAbs.z >= vAbs.x && vAbs.z >= vAbs.y) {
          // u (0 to 1) goes from -x to +x
          // v (0 to 1) goes from -y to +y
          maxAxis = vAbs.z;
          uv.x = direction.x;
          uv.y = -direction.y;
          outFaceIndex = 4;
      }
      // NEGATIVE Z
      if (!isZPositive && vAbs.z >= vAbs.x && vAbs.z >= vAbs.y) {
          // u (0 to 1) goes from +x to -x
          // v (0 to 1) goes from -y to +y
          maxAxis = vAbs.z;
          uv.x = -direction.x;
          uv.y = -direction.y;
          outFaceIndex = 5;
      }

      Real maxAxisInv = R(0.5)/maxAxis;
      uv.x = uv.x * maxAxisInv + R(0.5);
      uv.y = uv.y * maxAxisInv + R(0.5);

      return uv;
    }
  };

}

#endif /* LIB_TEXTURE_CUBEMAP_HPP_ */
