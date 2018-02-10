#include "Camera.h"
#include "CoordinateSystem.h"

namespace Game
{
	void Camera::checkForCoordinateSystemSwap()
	{
		CoordinateSystem* parentCS = (CoordinateSystem*)coordinateSystem->parent;

		if (parentCS) {
			if (transform.getPosition().length() > coordinateSystem->radius * (parentCS->scale / coordinateSystem->scale)) {

			glm::vec3 newPosition = { transform.getPosition().x, transform.getPosition().y, transform.getPosition().z };
			newPosition = newPosition * coordinateSystem->transform.getOrientationQuaternion();
			newPosition *= glm::vec3(coordinateSystem->scale / parentCS->scale, coordinateSystem->scale / parentCS->scale, coordinateSystem->scale / parentCS->scale);
			newPosition += glm::vec3(coordinateSystem->transform.getPosition().x, coordinateSystem->transform.getPosition().y, coordinateSystem->transform.getPosition().z);
			transform.setPosition(newPosition);

				glm::quat newOrientation = transform.getOrientationQuaternion() * coordinateSystem->transform.getOrientationQuaternion();
				transform.setOrientation(newOrientation);


				output("Went from " << coordinateSystem->name << " to " << parentCS->name);

				coordinateSystem = parentCS;
			}
		}
	}
}
