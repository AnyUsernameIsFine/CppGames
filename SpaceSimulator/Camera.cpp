#include "Camera.h"
#include "CoordinateSystem.h"

namespace Game
{
	void Camera::checkForCoordinateSystemSwap()
	{
		CoordinateSystem* currCs = coordinateSystem;
		CoordinateSystem* parentCs = (CoordinateSystem*)currCs->parent;
		Position<Coordinate> camPos = transform.getPosition();

		// go to parent
		if (parentCs && camPos.length() > currCs->radius * (parentCs->scale / currCs->scale)) {
			glm::quat currCsQuat = currCs->transform.getOrientationQuaternion();

			glm::vec3 p = { camPos.x, camPos.y, camPos.z };
			p = p * currCsQuat;
			float r = currCs->scale / parentCs->scale;
			p *= glm::vec3(r, r, r);
			Position<Coordinate> currCsPos = currCs->transform.getPosition();
			p += glm::vec3(currCsPos.x, currCsPos.y, currCsPos.z);

			glm::quat o = transform.getOrientationQuaternion() * currCsQuat;

			coordinateSystem = parentCs;
			transform.setPosition(p);
			transform.setOrientation(o);

			checkForCoordinateSystemSwap();
		}
		// go to child
		else {
			CoordinateSystem* childCS = nullptr;

			for (auto& subSystem : currCs->coordinateSystems) {
				Position<Coordinate> position = camPos - subSystem.transform.getPosition();
				if (position.length() < subSystem.radius) {
					childCS = &subSystem;
				}
			}

			if (childCS) {
				Position<Coordinate> position = camPos - childCS->transform.getPosition();
				glm::quat childCsQuat = glm::conjugate(childCS->transform.getOrientationQuaternion());

				glm::vec3 p = { position.x, position.y, position.z };
				p = p * childCsQuat;
				float r = currCs->scale / childCS->scale;
				p *= glm::vec3(r, r, r);

				glm::quat o = transform.getOrientationQuaternion() * childCsQuat;

				coordinateSystem = childCS;
				transform.setPosition(p);
				transform.setOrientation(o);

				checkForCoordinateSystemSwap();
			}
		}
	}
}
