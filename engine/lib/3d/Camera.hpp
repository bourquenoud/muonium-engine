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
    Matrix4 worldToCamera;
    Real fov_deg;
    Real b;
    Real l;
    Real t;
    Real r;
    Real near;
    Real far;
    uint32_t width;
    uint32_t height;

    Camera();

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
     * Convert a a world position to a raster position
     */
    Vector3 toRaster(Vector3 v);

  private:
    //Compute the projection matrix according to the camera parameters
    void computeProjectionMatrix();
  };

}

#endif /* CAMERA_HPP_ */
