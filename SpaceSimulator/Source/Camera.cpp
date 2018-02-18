#include "Camera.h"
#include "Universe.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\norm.hpp>

#include <limits>
#include <cmath>
#include <sstream>
#include <iomanip>

namespace Game
{
	void Camera::move(const glm::vec3& vector)
	{
		acceleration_ += vector;
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

	std::vector<Camera::CameraHierarchyLevel> Camera::getHierarchy() const
	{
		CameraHierarchyLevel ch{
			coordinateSystem,
			glm::mat4(1),
			transform.getPosition(),
		};

		std::vector<Camera::CameraHierarchyLevel> hierarchy = { ch };

		CoordinateSystem* parentCs = coordinateSystem->getParent();

		while (parentCs) {
			glm::quat q = ch.coordinateSystem->transform.getOrientation();
			ch.rotation *= glm::mat4_cast(q);

			// TODO: add more precision?
			ch.position *= q;
			ch.position *= ch.coordinateSystem->getScale() / parentCs->getScale();
			ch.position += ch.coordinateSystem->transform.getPosition();

			ch.coordinateSystem = parentCs;

			hierarchy.push_back(ch);

			parentCs = parentCs->getParent();
		}

		return hierarchy;
	}

	float Camera::getSpeed() const
	{
		return glm::length(velocity_) * coordinateSystem->getScale();
	}

	void Camera::update(float deltaSeconds)
	{
		if (glm::length2(velocity_) > FLT_EPSILON || glm::length2(acceleration_) > FLT_EPSILON) {
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
				speed = ((int_least64_t)1 << 7) * Galaxy::MAXIMUM_RADIUS * (Galaxy::SCALE / Universe::SCALE);
			}

			// if there are any children in this coordinate system,
			// we need to move slower when close to one
			if (!coordinateSystem->getChildren().empty()) {
				Vector3 p = transform.getPosition();

				// determine the distance to the edge of the closest child coordinate system
				CoordinateSystem* childCs = nullptr;
				float smallestDistance = std::numeric_limits<float>::max();
				for (auto& child : coordinateSystem->getChildren()) {
					Vector3 position = p - child->transform.getPosition();
					float distance = position.length() - child->getRadius();
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

				float lerp = (1 - cosf(fminf(fmaxf(0, smallestDistance), 1) * (float)M_PI)) / 2;

				float minSpeed = childCs->getRadius() / 2;

				speed = minSpeed * (1 - lerp) + speed * lerp;
			}

			if (glm::length2(acceleration_) > FLT_EPSILON) {
				velocity_ = (velocity_ + acceleration_ * speed) * 0.5f;
				maxVelocity_ = velocity_;
				acceleration_ = glm::vec3(0);
			}
			else if (glm::length2(velocity_) > speed * speed) {
				velocity_ = (velocity_ + glm::normalize(velocity_) * speed) * 0.5f;
			}
		}

		if (glm::length2(velocity_) > FLT_EPSILON) {
			transform.move(velocity_ * deltaSeconds);

			velocity_ *= powf(0.5f, deltaSeconds * 10);

			if (glm::length2(velocity_) < glm::length2(maxVelocity_) * 0.00001f) {
				velocity_ = { 0 };
			}

			setCorrectCoordinateSystem_();
		}
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

	void Camera::setCorrectCoordinateSystem_()
	{
		CoordinateSystem* parentCs = coordinateSystem->getParent();
		Vector3 p = transform.getPosition();

		// go to parent
		if (parentCs && p.length() > coordinateSystem->getRadius() * (parentCs->getScale() / coordinateSystem->getScale()) * 0.99f) {
			glm::quat q = coordinateSystem->transform.getOrientation();

			// TODO: add more precision?
			p *= q;
			p *= coordinateSystem->getScale() / parentCs->getScale();
			p += coordinateSystem->transform.getPosition();
			transform.setPosition(p);

			glm::quat o = transform.getOrientation() * q;
			transform.setOrientation(transform.getOrientation() * q);

			velocity_ *= coordinateSystem->getScale() / parentCs->getScale();

			coordinateSystem = parentCs;

			setCorrectCoordinateSystem_();
		}

		// go to child
		else if (!coordinateSystem->getChildren().empty()) {
			CoordinateSystem* childCs = nullptr;

			for (auto& child : coordinateSystem->getChildren()) {
				Vector3 position = p - child->transform.getPosition();
				if (position.length() < child->getRadius() * 0.98f) {
					childCs = child.get();
				}
			}

			if (childCs) {
				glm::quat q = glm::conjugate(childCs->transform.getOrientation());

				// TODO: add more precision?
				p -= childCs->transform.getPosition();
				p *= q;
				p *= coordinateSystem->getScale() / childCs->getScale();
				transform.setPosition(p);

				transform.setOrientation(transform.getOrientation() * q);

				velocity_ *= coordinateSystem->getScale() / childCs->getScale();

				coordinateSystem = childCs;

				setCorrectCoordinateSystem_();
			}
		}
	}
}
