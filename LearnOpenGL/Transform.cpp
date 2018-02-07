#include "Transform.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Game
{
	Transform::Transform(const glm::vec3& position, const glm::vec3& orientation, const glm::vec3& scale)
	{
		setPosition(position);
		setOrientation(orientation);
		setScale(scale);
	}

	void Transform::setPosition(const glm::vec3& position)
	{
		position_ = position;
	}

	void Transform::setOrientation(const glm::vec3& orientation)
	{
		orientation_ = glm::quat(glm::radians(orientation));
	}

	void Transform::setScale(const glm::vec3& scale)
	{
		scale_ = scale;
	}

	const glm::vec3 Transform::getPosition() const
	{
		return position_;
	}

	const glm::vec3 Transform::getOrientation() const
	{
		return glm::degrees(glm::eulerAngles(orientation_));
	}

	const glm::quat Transform::getOrientationQuaternion() const
	{
		return orientation_;
	}

	const glm::vec3 Transform::getScale() const
	{
		return scale_;
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
		//glm::quat q = glm::angleAxis(glm::radians(angle), orientation_ * glm::vec3(0, 1, 0));
		//rotate(q);
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

	void Transform::scaleX(float factor)
	{
		scale({ factor, 0, 0 });
	}

	void Transform::scaleY(float factor)
	{
		scale({ 0, factor, 0 });
	}

	void Transform::scaleZ(float factor)
	{
		scale({ 0, 0, factor });
	}

	void Transform::scale(const glm::vec3& factor)
	{
		scale_ *= factor;
	}

	const glm::mat4 Transform::getModelMatrix() const
	{
		glm::mat4 scale = glm::scale(glm::mat4(1), scale_);
		glm::mat4 rotate = glm::inverse(glm::mat4_cast(orientation_));
		glm::mat4 translate = glm::translate(glm::mat4(1), position_);

		return translate * rotate * scale;
	}
}
