/*
 * Camera.hpp
 *
 *  Created on: 8 Apr 2021
 *      Author: mathieu
 */

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <cstdint>

#include "Object3D.hpp"
#include "../linal.hpp"
#include "../arith.hpp"

namespace ue
{
  enum class FitResolutionGate
  {
    fill,
    overscan
  };

  class Camera : public ue::Object3D
  {
  public:
    //XXX probably better to use a 3x3 matrix if no scaling is involved
    Matrix3 worldToCamera;
    Real angleOfView;
    Real b;
    Real l;
    Real t;
    Real r;
    Real near;
    Real far;
    uint32_t width;
    uint32_t height;

    /** TODO: Finish description
     *
     */
    Camera(
        const float &filmApertureWidth,
        const float &filmApertureHeight,
        const uint32_t &imageWidth,
        const uint32_t &imageHeight,
        const FitResolutionGate &fitFilm,
        const float &nearClippingPLane,
        const float &focalLength
    );

    /** TODO: Finish description
     *
     */
    Vector3 toRaster(Vector3);

  };

}

#endif /* CAMERA_HPP_ */
