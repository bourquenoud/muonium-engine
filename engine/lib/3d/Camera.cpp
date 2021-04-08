/*
 * Camera.cpp
 *
 *  Created on: 8 Apr 2021
 *      Author: mathieu
 */

#include "Camera.hpp"

namespace ue
{

  Camera::Camera(
      const float &filmApertureWidth,
      const float &filmApertureHeight,
      const uint32_t &imageWidth,
      const uint32_t &imageHeight,
      const FitResolutionGate &fitFilm,
      const float &nearClippingPLane,
      const float &focalLength
  )
  {
    float filmAspectRatio = filmApertureWidth / filmApertureHeight;
    float deviceAspectRatio = imageWidth / (float)imageHeight;

    t = ((filmApertureHeight / 2) / focalLength) * nearClippingPLane;
    r = ((filmApertureWidth / 2) / focalLength) * nearClippingPLane;

    // field of view (horizontal)
    //float fov = 2 * 180 / M_PI * atanf((filmApertureWidth / 2) / focalLength);

    float xscale = 1;
    float yscale = 1;

    switch (fitFilm) {
    default:
    case FitResolutionGate::fill:
      if (filmAspectRatio > deviceAspectRatio) {
          xscale = deviceAspectRatio / filmAspectRatio;
      }
      else {
          yscale = filmAspectRatio / deviceAspectRatio;
      }
      break;
    case FitResolutionGate::overscan:
      if (filmAspectRatio > deviceAspectRatio) {
          yscale = filmAspectRatio / deviceAspectRatio;
      }
      else {
          xscale = deviceAspectRatio / filmAspectRatio;
      }
      break;
    }

    r *= xscale;
    t *= yscale;

    b = -t;
    l = -r;
  }

  Vector3 Camera::toRaster(Vector3 vert)
  {
    //Rotate
    vert = worldToCamera * vert;

    //Project to screen
    vert = Vector3(
        (R(1.0) + near * vert.x / -vert.z) * R(0.5) * (Real)width, //XXX Maybe store width as real ?
        (R(1.0) + near * vert.y / vert.z) * R(0.5) * (Real)height,
        -vert.z);


    return vert;
  }

}
