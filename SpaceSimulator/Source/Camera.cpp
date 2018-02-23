#include "Camera.h"
#include "Universe.h"

#define GLM_FORCE_INLINE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\norm.hpp>

#include <limits>
#include <cmath>
#include <sstream>
#include <iomanip>

namespace Game
{
	void Camera::setCoordinateSystem(CoordinateSystem* coordinateSystem)
	{
		if (coordinateSystem != coordinateSystem_) {
			coordinateSystem_ = coordinateSystem;
			createHierarchy_();
		}
	}

	CoordinateSystem* Camera::getCoordinateSystem() const
	{
		return coordinateSystem_;
	}

	void Camera::setPosition(const Vector3& position)
	{
		transform.setPosition(position);
		createHierarchy_();
	}

	void Camera::setPosition(Coordinate x, Coordinate y, Coordinate z)
	{
		setPosition({ x, y, z });
	}

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

	const std::vector<Camera::CameraHierarchyLevel>& Camera::getHierarchy() const
	{
		return hierarchy_;
	}

	float Camera::getSpeed() const
	{
		return glm::length(velocity_) * coordinateSystem_->getScale();
	}

	void Camera::update(float deltaSeconds)
	{
		if (glm::length2(velocity_) > FLT_EPSILON || glm::length2(acceleration_) > FLT_EPSILON) {
			CoordinateSystem* parentCs = coordinateSystem_->getParent();

			// determine the maximum speed
			float speed;

			// if we are inside a descendant coordinate system of the universe,
			// we move through it at max speed in four seconds (2 for its radius)
			if (parentCs) {
				speed = coordinateSystem_->getRadius() * parentCs->getScale() / coordinateSystem_->getScale() / 2;
			}

			// the maximum speed when moving through the universe
			else {
				speed = ((int_least64_t)1 << 7) * Galaxy::MAX_RADIUS * (Galaxy::SCALE / Universe::SCALE);
			}

			// if there are any children in this coordinate system,
			// we need to move slower when close to one
			if (!coordinateSystem_->getChildren().empty()) {
				Vector3 p = transform.getPosition();

				// determine the distance to the edge of the closest child coordinate system
				CoordinateSystem* childCs = nullptr;
				float smallestDistance = std::numeric_limits<float>::max();
				for (auto& child : coordinateSystem_->getChildren()) {
					float distance = p.distance(child->transform.getPosition()) - child->getRadius();
					if (distance < smallestDistance) {
						childCs = child.get();
						smallestDistance = distance;
					}
				}

				if (parentCs) {
					smallestDistance *= 5.0f * coordinateSystem_->getScale() / (parentCs->getScale() * coordinateSystem_->getRadius());
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

			transform.move(velocity_ * deltaSeconds);

			velocity_ *= powf(0.5f, deltaSeconds * 10);

			if (glm::length2(velocity_) < glm::length2(maxVelocity_) * 0.00001f) {
				velocity_ = glm::vec3(0);
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
		CoordinateSystem* cs = coordinateSystem_;
		CoordinateSystem* parentCs = cs->getParent();
		Vector3 p = transform.getPosition();

		// go to parent
		if (parentCs && p.length() > cs->getRadius() * (parentCs->getScale() / cs->getScale()) * 0.99f) {
			glm::quat q = cs->transform.getOrientation();

			p *= q;
			p *= cs->getScale() / parentCs->getScale();
			p += cs->transform.getPosition();
			transform.setPosition(p);

			glm::quat o = transform.getOrientation() * q;
			transform.setOrientation(transform.getOrientation() * q);

			velocity_ *= cs->getScale() / parentCs->getScale();
			maxVelocity_ *= cs->getScale() / parentCs->getScale();

			coordinateSystem_ = parentCs;

			setCorrectCoordinateSystem_();
		}

		// go to child
		else if (!cs->getChildren().empty()) {
			CoordinateSystem* childCs = nullptr;

			for (auto& child : cs->getChildren()) {
				Vector3 position = p - child->transform.getPosition();
				float r = child->getRadius() * 0.98f;
				if (position.lengthSquared() < r * r) {
					childCs = child.get();
				}
			}

			if (childCs) {
				glm::quat q = glm::conjugate(childCs->transform.getOrientation());

				p -= childCs->transform.getPosition();
				p *= q;
				p *= cs->getScale() / childCs->getScale();
				transform.setPosition(p);

				transform.setOrientation(transform.getOrientation() * q);

				velocity_ *= cs->getScale() / childCs->getScale();
				maxVelocity_ *= cs->getScale() / childCs->getScale();

				coordinateSystem_ = childCs;

				setCorrectCoordinateSystem_();
			}
			else {
				createHierarchy_();
			}
		}

		else {
			createHierarchy_();
		}
	}

	void Camera::createHierarchy_()
	{
		CameraHierarchyLevel ch = {
		coordinateSystem_,
		glm::mat4(1),
		transform.getPosition(),
		};

		hierarchy_ = { ch };

		CoordinateSystem* parentCs = coordinateSystem_->getParent();

		while (parentCs) {
			glm::quat q = ch.coordinateSystem->transform.getOrientation();
			ch.rotation *= glm::mat4_cast(q);

			ch.position *= q;
			ch.position *= ch.coordinateSystem->getScale() / parentCs->getScale();
			ch.position += ch.coordinateSystem->transform.getPosition();

			ch.coordinateSystem = parentCs;

			hierarchy_.insert(hierarchy_.begin(), ch); // quite inefficient

			parentCs = parentCs->getParent();
		}
	}
}
