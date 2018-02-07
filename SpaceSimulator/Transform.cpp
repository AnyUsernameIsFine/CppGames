#include "Transform.h"

#include <Error.h>

#include <glm\gtc\matrix_transform.hpp>

namespace Framework
{
	Transform::Transform(glm::vec3 position)
	{
		position_ = position;
	}

	const glm::vec3 Transform::getPosition() const
	{
		return position_;
	}

	const glm::quat Transform::getOrientation() const
	{
		return orientation_;
	}

	void Transform::useModelAxes(bool use)
	{
		useModelAxes_ = use;
	}

	void Transform::moveX(float distance)
	{
		move({ distance, 0, 0 });
	}

	void Transform::moveY(float distance)
	{
		move({ 0, distance, 0 });
	}

	void Transform::moveZ(float distance)
	{
		move({ 0, 0, distance });
	}

	void Transform::move(const glm::vec3& vector)
	{
		if (useModelAxes_) {
			position_ += vector * orientation_;
		}
		else {
			position_ += vector;
		}
	}

	void Transform::yaw(float angle)
	{
		rotate(angle, { 0, 1, 0 });
	}

	void Transform::pitch(float angle)
	{
		rotate(angle, { 1, 0, 0 });
	}

	void Transform::roll(float angle)
	{
		rotate(angle, { 0, 0, 1 });
	}

	void Transform::rotate(float angle, const glm::vec3& axis)
	{
		glm::quat quaternion = glm::angleAxis(glm::radians(angle), axis);
		rotate(quaternion);
	}

	void Transform::rotate(const glm::quat& rotation)
	{
		if (useModelAxes_) {
			orientation_ = rotation * orientation_;
		}
		else {
			orientation_ *= rotation;
		}

		orientation_ = glm::normalize(orientation_);
	}

	const glm::mat4 Transform::getModelMatrix() const
	{
		glm::mat4 translate = glm::translate(glm::mat4(1), position_);
		glm::mat4 rotate = glm::inverse(glm::mat4_cast(orientation_));

		return translate * rotate;
	}
}
