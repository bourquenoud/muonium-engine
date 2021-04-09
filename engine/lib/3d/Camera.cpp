/*
 * Camera.cpp
 *
 *  Created on: 8 Apr 2021
 *      Author: mathieu
 */

#include "Camera.hpp"

namespace ue
{
  Camera::Camera()
  {

  }

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
    fov_deg = 2 * 180 / M_PI * atanf((filmApertureWidth / 2) / focalLength);

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

    width = imageWidth;
    height = imageHeight;
    near = nearClippingPLane;
  }

#define FOV_FACTOR 8.72664626e-3
  void Camera::computeProjectionMatrix()
  {
    Real S = 1/(tanf((float)fov_deg * FOV_FACTOR));

    worldToCamera[0][0] = S;
    worldToCamera[0][1] = 0;
    worldToCamera[0][2] = 0;
    worldToCamera[0][3] = 0;

    worldToCamera[1][0] = 0;
    worldToCamera[1][1] = S;
    worldToCamera[1][2] = 0;
    worldToCamera[1][3] = 0;

    worldToCamera[2][0] = 0;
    worldToCamera[2][1] = 0;
    worldToCamera[2][2] = -far/(far-near);
    worldToCamera[2][3] = -1;

    worldToCamera[3][0] = 0;
    worldToCamera[3][1] = 0;
    worldToCamera[3][2] = -far*near/(far-near);
    worldToCamera[3][3] = 0;
  }

  Vector3 Camera::toRaster(Vector3 vert)
  {
    /*
    //Rotate
    vert = worldToCamera * vert;
     */

    //Project to screen
    vert = Vector3(
        (R(1.0) + near * vert.x / -vert.z) * R(0.5) * (Real)width, //XXX Maybe store width as real ?
        ((Real)height/(Real)width + near * vert.y / vert.z) * R(0.5) * (Real)width,
        -vert.z);


    return vert;
  }

}
