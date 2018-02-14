#include "Camera.h"
#include "CoordinateSystem.h"

#include <limits>
#include <cmath>

namespace Game
{
	void Camera::move(const glm::vec3& vector)
	{
		CoordinateSystem* cs = coordinateSystem;
		CoordinateSystem* parentCs = (CoordinateSystem*)cs->parent;

		// determine the maximum speed
		float speed;

		// if we are inside a descendant coordinate system of the universe,
		// we move through it at max speed in four seconds (2 for its radius)
		if (parentCs) {
			speed = cs->radius * parentCs->scale / cs->scale / 2;
		}
		
		// the maximum speed when moving through the universe
		else {
#ifdef USE_REALISTIC_SCALE
			speed = 200000000.0f;
#else
			speed = ((int_least64_t)1 << 62);
#endif
		}

		// if there are any children in this coordinate system,
		// we need to move slower when close to one
		if (!cs->children.empty()) {
			Vector3 p = transform.getPosition();

			// determine the distance to the edge of the closest child coordinate system
			CoordinateSystem* childCs = nullptr;
			float smallestDistance = std::numeric_limits<float>::max();
			for (auto& child : cs->children) {
				Vector3 position = p - child->transform.getPosition();
				float distance = position.length() - child->radius;
				if (distance < smallestDistance) {
					childCs = child.get();
					smallestDistance = distance;
				}
			}

			if (parentCs) {
				smallestDistance *= 5.0f * cs->scale / (parentCs->scale * cs->radius);
			}
			else {
				smallestDistance *= 0.02f / childCs->radius;
			}

			float lerp = (1 - cosf(fminf(fmaxf(0, smallestDistance), 1) * (float)M_PI)) / 2;

			float minSpeed = childCs->radius / 2;

			speed = minSpeed * (1 - lerp) + speed * lerp;
		}

		transform.move(vector * speed);
	}

	void Camera::move(float x, float y, float z)
	{
		move({ x, y, z });
	}

	void Camera::moveX(float distance)
	{
		move(distance, 0, 0);
	}

	void Camera::moveY(float distance)
	{
		move(0, distance, 0);
	}

	void Camera::moveZ(float distance)
	{
		move(0, 0, distance);
	}

	void Camera::checkForCoordinateSystemSwap()
	{
		CoordinateSystem* cs = coordinateSystem;
		CoordinateSystem* parentCs = (CoordinateSystem*)cs->parent;
		Vector3 p = transform.getPosition();

		// go to parent
		if (parentCs && p.length() > cs->radius * (parentCs->scale / cs->scale) * 0.99f) {
			glm::quat q = cs->transform.getOrientation();

			// TODO: add more precision?
			p *= q;
			p *= cs->scale / parentCs->scale;
			p += cs->transform.getPosition();

			glm::quat o = transform.getOrientation() * q;

			coordinateSystem = parentCs;
			transform.setPosition(p);
			transform.setOrientation(transform.getOrientation() * q);

			checkForCoordinateSystemSwap();
		}
		// go to child
		else {
			CoordinateSystem* childCs = nullptr;

			for (auto& child : cs->children) {
				Vector3 position = p - child->transform.getPosition();
				if (position.length() < child->radius * 0.98f) {
					childCs = child.get();
				}
			}

			if (childCs) {
				glm::quat q = glm::conjugate(childCs->transform.getOrientation());

				// TODO: add more precision?
				p -= childCs->transform.getPosition();
				p *= q;
				p *= cs->scale / childCs->scale;

				coordinateSystem = childCs;
				transform.setPosition(p);
				transform.setOrientation(transform.getOrientation() * q);

				checkForCoordinateSystemSwap();
			}
		}
	}
}
