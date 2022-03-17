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
		Real width;
		Real height;

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
			const float &focalLength);

		/** TODO: Finish description
		 * Convert a a world position to a raster position
		 */
		inline Vector3 toRaster(const Vector3 &vert) const
		{

			// Project to screen
			Vector3 result(
				projAx + projB * vert.x / -vert.z,
				projAy + projB * vert.y / -vert.z,
				-vert.z);

			return result;
		}

		/*
		 * TODO : raster or screen position ?
		 * Convert from a raster position to a world position
		 */
		inline Vector3 fromRaster(const Vector3 &v) const
		{
			Vector3 result(
				(v.x - projAx) * v.z / projB,
				(v.y - projAy) * v.z / projB,
				-v.z);

			return result;
		}

		/*
		 * TODO : raster or screen position ?
		 * Convert from a raster position to a world position
		 * Implies a z = -1
		 */
		inline Vector3 fromRaster(const Vector2 &v) const
		{
			Vector3 result(
				(v.x - projAx) / projB,
				(v.y - projAy) / projB,
				1);

			return result;
		}

		/*
		 * TODO : raster or screen position ?
		 * Convert from a raster position to a world position
		 * Implies a z = -1
		 */
		inline Vector3 fromRaster(Real x, Real y)
		{
			Vector3 result(
				(x - projAx) / projB,
				(y - projAy) / projB,
				1);

			return result;
		}

	private:
		// Precompute factors for the projections
		Real projAx;
		Real projAy;
		Real projB;

		// TODO: fucking use it
		// Compute the projection matrix according to the camera parameters
		void computeProjectionMatrix();
	};

}

#endif /* CAMERA_HPP_ */
