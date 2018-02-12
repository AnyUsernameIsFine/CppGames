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
		if (parentCs && p.length() > cs->radius * (parentCs->scale / cs->scale)) {
			glm::quat q = cs->transform.getOrientation();

			// TODO: reorder these next two operations for added precision?
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

			for (auto& descendant : cs->descendants) {
				Vector3 position = p - descendant.transform.getPosition();
				if (position.length() < descendant.radius) {
					childCS = &descendant;
				}
			}

			if (childCS) {
				glm::quat q = glm::conjugate(childCS->transform.getOrientation());

				p -= childCS->transform.getPosition();
				// TODO: reorder these next two operations for added precision?
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
