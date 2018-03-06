#include "Camera.h"

#include <iomanip>
#include <limits>
#include <sstream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\norm.hpp>
#include "Universe.h"

namespace SpaceSimulator
{
	void Camera::setCoordinateSystem(CoordinateSystem* coordinateSystem)
	{
		if (coordinateSystem != this->coordinateSystem) {
			this->coordinateSystem = coordinateSystem;
			createHierarchy();
		}
	}

	void Camera::setPosition(const Vector3& position)
	{
		myTransform.setPosition(position);
		createHierarchy();
	}

	void Camera::setPosition(Coordinate x, Coordinate y, Coordinate z)
	{
		setPosition({ x, y, z });
	}

	CoordinateSystem* Camera::getCoordinateSystem() const
	{
		return coordinateSystem;
	}

	const std::vector<Camera::CameraHierarchyLevel>& Camera::getHierarchy() const
	{
		return hierarchy;
	}

	float Camera::getSpeed() const
	{
		return glm::length(velocity) * coordinateSystem->getScale();
	}

	std::string Camera::getSpeedString() const
	{
		double perHour = getSpeed() / 3600;

		std::string units;

		if (perHour > 9460730472580800 * 1e9) {
			perHour /= 9460730472580800 * 1e9;
			units = "Gly/h";
		}
		else if (perHour > 9460730472580800 * 1e6) {
			perHour /= 9460730472580800 * 1e6;
			units = "Mly/h";
		}
		else if (perHour > 9460730472580800 * 1e3) {
			perHour /= 9460730472580800 * 1e3;
			units = "kly/h";
		}
		else if (perHour > 9460730472580800) {
			perHour /= 9460730472580800;
			units = "ly/h";
		}
		else if (perHour > 149597870700) {
			perHour /= 149597870700;
			units = "AU/h";
		}
		else if (perHour > 1e9) {
			perHour /= 1e9;
			units = "Gm/h";
		}
		else if (perHour > 1e6) {
			perHour /= 1e6;
			units = "Mm/h";
		}
		else {
			perHour /= 1e3;
			units = "km/h";
		}

		if (perHour) {
			double factor = pow(10.0, 2 - ceil(log10(fabs(perHour))));
			perHour = round(perHour * factor) / factor;
		}

		std::ostringstream out;
		out << std::setprecision(0) << perHour << " " << units;

		return out.str();
	}

	void Camera::move(const glm::vec3& vector)
	{
		acceleration += vector;
	}

	void Camera::move(float x, float y, float z)
	{
		move({ x, y, z });
	}

	void Camera::moveX(float distance)
	{
		move({ distance, 0, 0 });
	}

	void Camera::moveY(float distance)
	{
		move({ 0, distance, 0 });
	}

	void Camera::moveZ(float distance)
	{
		move({ 0, 0, distance });
	}

	void Camera::update(float deltaSeconds)
	{
		if (glm::length2(velocity) > FLT_EPSILON || glm::length2(acceleration) > FLT_EPSILON) {
			CoordinateSystem* parentCs = coordinateSystem->getParent();

			// determine the maximum speed
			float speed;

			// if we are inside a descendant coordinate system of the universe,
			// we move through it at max speed in four seconds (2 for its radius)
			if (parentCs) {
				speed = coordinateSystem->getRadius() * parentCs->getScale() / coordinateSystem->getScale() / 2;
			}

			// the maximum speed when moving through the universe
			else {
				speed = (1 << 7) * Galaxy::MAX_RADIUS * (Galaxy::SCALE / Universe::SCALE);
			}

			// if there are any children in this coordinate system,
			// we need to move slower when close to one
			if (!coordinateSystem->getChildren().empty()) {
				Vector3 p = myTransform.getPosition();

				// determine the distance to the edge of the closest child coordinate system
				CoordinateSystem* childCs = nullptr;
				float smallestDistance = std::numeric_limits<float>::max();
				for (const auto& child : coordinateSystem->getChildren()) {
					float distance = p.distance(child->transform().getPosition()) - child->getRadius();
					if (distance < smallestDistance) {
						childCs = child.get();
						smallestDistance = distance;
					}
				}

				if (parentCs) {
					smallestDistance *= 5.0f * coordinateSystem->getScale() / (parentCs->getScale() * coordinateSystem->getRadius());
				}
				else {
					smallestDistance *= 0.02f / childCs->getRadius();
				}

				float lerp = (1 - cosf(fminf(fmaxf(0, smallestDistance), 1) * static_cast<float>(M_PI))) / 2;

				float minSpeed = childCs->getRadius() / 2;

				speed = minSpeed * (1 - lerp) + speed * lerp;
			}

			if (glm::length2(acceleration) > FLT_EPSILON) {
				velocity = (velocity + acceleration * speed) * 0.5f;
				maxVelocity = velocity;
				acceleration = glm::vec3(0);
			}
			else if (glm::length2(velocity) > speed * speed) {
				velocity = (velocity + glm::normalize(velocity) * speed) * 0.5f;
			}

			myTransform.move(velocity * deltaSeconds);

			velocity *= powf(0.5f, deltaSeconds * 10);

			if (glm::length2(velocity) < glm::length2(maxVelocity) * 0.00001f) {
				velocity = glm::vec3(0);
			}

			setCorrectCoordinateSystem();
		}
	}

	void Camera::setCorrectCoordinateSystem()
	{
		CoordinateSystem* cs = coordinateSystem;
		CoordinateSystem* parentCs = cs->getParent();
		Vector3 p = myTransform.getPosition();

		// go to parent
		if (parentCs && p.length() > cs->getRadius() * (parentCs->getScale() / cs->getScale()) * 0.99f) {
			glm::quat q = cs->transform().getOrientation();

			p *= q;
			p *= cs->getScale() / parentCs->getScale();
			p += cs->transform().getPosition();
			myTransform.setPosition(p);

			glm::quat o = myTransform.getOrientation() * q;
			myTransform.setOrientation(myTransform.getOrientation() * q);

			velocity *= cs->getScale() / parentCs->getScale();
			maxVelocity *= cs->getScale() / parentCs->getScale();

			coordinateSystem = parentCs;

			setCorrectCoordinateSystem();
		}

		// go to child
		else if (!cs->getChildren().empty()) {
			CoordinateSystem* childCs = nullptr;

			for (const auto& child : cs->getChildren()) {
				Vector3 position = p - child->transform().getPosition();
				float r = child->getRadius() * 0.98f;
				if (position.lengthSquared() < r * r) {
					childCs = child.get();
				}
			}

			if (childCs) {
				glm::quat q = glm::conjugate(childCs->transform().getOrientation());

				p -= childCs->transform().getPosition();
				p *= q;
				p *= cs->getScale() / childCs->getScale();
				myTransform.setPosition(p);

				myTransform.setOrientation(myTransform.getOrientation() * q);

				velocity *= cs->getScale() / childCs->getScale();
				maxVelocity *= cs->getScale() / childCs->getScale();

				coordinateSystem = childCs;

				setCorrectCoordinateSystem();
			}
			else {
				createHierarchy();
			}
		}

		else {
			createHierarchy();
		}
	}

	void Camera::createHierarchy()
	{
		CameraHierarchyLevel ch = {
			coordinateSystem,
			glm::mat4(1),
			myTransform.getPosition()
		};

		hierarchy = { ch };

		CoordinateSystem* parentCs = coordinateSystem->getParent();

		while (parentCs) {
			glm::quat q = ch.coordinateSystem->transform().getOrientation();
			ch.rotation *= glm::mat4_cast(q);

			ch.position *= q;
			ch.position *= ch.coordinateSystem->getScale() / parentCs->getScale();
			ch.position += ch.coordinateSystem->transform().getPosition();

			ch.coordinateSystem = parentCs;

			hierarchy.insert(hierarchy.begin(), ch); // quite inefficient

			parentCs = parentCs->getParent();
		}
	}
}
