#include "Camera.h"
//#include "CoordinateSystem.h"
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

	void Camera::update(float deltaSeconds)
	{
		if (glm::length2(velocity_) > FLT_EPSILON || glm::length2(acceleration_) > FLT_EPSILON) {
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
				speed = ((int_least64_t)1 << 7) * Galaxy::MAXIMUM_RADIUS * (Galaxy::SCALE / Universe::SCALE);
			}

			// if there are any children in this coordinate system,
			// we need to move slower when close to one
			if (!cs->getChildren().empty()) {
				Vector3 p = transform.getPosition();

				// determine the distance to the edge of the closest child coordinate system
				CoordinateSystem* childCs = nullptr;
				float smallestDistance = std::numeric_limits<float>::max();
				for (auto& child : cs->getChildren()) {
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

	float Camera::getSpeed() const
	{
		return glm::length(velocity_) * coordinateSystem->scale;
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

			velocity_ *= cs->scale / parentCs->scale;

			setCorrectCoordinateSystem_();
		}

		// go to child
		else if (!cs->getChildren().empty()) {
			CoordinateSystem* childCs = nullptr;

			for (auto& child : cs->getChildren()) {
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

				velocity_ *= cs->scale / childCs->scale;

				setCorrectCoordinateSystem_();
			}
		}
	}
}
