#include "Camera.h"
#include "CoordinateSystem.h"

namespace Game
{
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
			CoordinateSystem* childCS = nullptr;

			for (auto& child : cs->children) {
				Vector3 position = p - child->transform.getPosition();
				if (position.length() < child->radius * 0.98f) {
					childCS = child.get();
				}
			}

			if (childCS) {
				glm::quat q = glm::conjugate(childCS->transform.getOrientation());

				// TODO: add more precision?
				p -= childCS->transform.getPosition();
				p *= q;
				p *= cs->scale / childCS->scale;

				coordinateSystem = childCS;
				transform.setPosition(p);
				transform.setOrientation(transform.getOrientation() * q);

				checkForCoordinateSystemSwap();
			}
		}
	}
}
