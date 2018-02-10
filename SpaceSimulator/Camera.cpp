#include "Camera.h"
#include "CoordinateSystem.h"

namespace Game
{
	void Camera::checkForCoordinateSystemSwap()
	{
		CoordinateSystem* parentCS = (CoordinateSystem*)coordinateSystem->parent;

		// go to parent
		if (parentCS && transform.getPosition().length() > coordinateSystem->radius * (parentCS->scale / coordinateSystem->scale)) {

			glm::vec3 newPosition = { transform.getPosition().x, transform.getPosition().y, transform.getPosition().z };
			newPosition = newPosition * coordinateSystem->transform.getOrientationQuaternion();
			newPosition *= glm::vec3(coordinateSystem->scale / parentCS->scale, coordinateSystem->scale / parentCS->scale, coordinateSystem->scale / parentCS->scale);
			newPosition += glm::vec3(coordinateSystem->transform.getPosition().x, coordinateSystem->transform.getPosition().y, coordinateSystem->transform.getPosition().z);
			transform.setPosition(newPosition);

			glm::quat newOrientation = transform.getOrientationQuaternion() * coordinateSystem->transform.getOrientationQuaternion();
			transform.setOrientation(newOrientation);

			coordinateSystem = parentCS;

			checkForCoordinateSystemSwap();
		}
		// go to child
		else {
			CoordinateSystem* childCS = nullptr;

			for (auto& subSystem : coordinateSystem->coordinateSystems) {
				if ((transform.getPosition() - subSystem.transform.getPosition()).length() < subSystem.radius) {
					childCS = &subSystem;
				}
			}

			if (childCS) {
				Position<Coordinate> position = transform.getPosition() - childCS->transform.getPosition();

				glm::vec3 newPosition = { position.x, position.y, position.z };
				newPosition = newPosition * glm::conjugate(childCS->transform.getOrientationQuaternion());
				newPosition *= glm::vec3(coordinateSystem->scale / childCS->scale, coordinateSystem->scale / childCS->scale, coordinateSystem->scale / childCS->scale);
				transform.setPosition(newPosition);

				glm::quat newOrientation = transform.getOrientationQuaternion() * glm::conjugate(childCS->transform.getOrientationQuaternion());
				transform.setOrientation(newOrientation);

				coordinateSystem = childCS;

				checkForCoordinateSystemSwap();
			}
		}
	}
}
