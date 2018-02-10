#pragma once

#include "Position.hpp"

#include <glm\gtc\quaternion.hpp>

namespace Framework
{
	template<typename T = float>
	class Transform
	{
	public:
		Transform(const Position<T>& position = { 0, 0, 0 }, const glm::vec3& orientation = { 0, 0, 0 }, const glm::vec3& scale = { 1, 1, 1 });
		void setPosition(const Position<T>& position);
		void setOrientation(const glm::vec3& orientation);
		void setOrientation(const glm::quat& orientation);
		void setScale(float scale);
		void setScale(const glm::vec3& scale);
		const Position<T> getPosition() const;
		const glm::vec3 getOrientation() const;
		const glm::quat getOrientationQuaternion() const;
		const glm::vec3 getScale() const;
		void useModelAxes(bool use = true);
		void moveX(float distance);
		void moveY(float distance);
		void moveZ(float distance);
		void move(const glm::vec3& vector);
		void yaw(float angle);
		void pitch(float angle);
		void roll(float angle);
		void rotate(float angle, const glm::vec3& axis);
		void scaleX(float factor);
		void scaleY(float factor);
		void scaleZ(float factor);
		void scale(float factor);
		void scale(const glm::vec3& factor);
		const glm::mat4 getRotateMatrix() const;
		const glm::mat4 getModelMatrix() const;
		const glm::mat4 getModelMatrix(const Position<T>& cameraPosition) const;

	private:
		Position<T> position_;
		glm::quat orientation_;
		glm::vec3 scale_;
		bool useModelAxes_ = true;

		const glm::mat4 getModelMatrix_(const glm::vec3& position) const;
	};
}

//#include "Transform.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Framework
{
	template<typename T>
	Transform<T>::Transform(const Position<T>& position, const glm::vec3& orientation, const glm::vec3& scale)
	{
		setPosition(position);
		setOrientation(orientation);
		setScale(scale);
	}

	template<typename T>
	void Transform<T>::setPosition(const Position<T>& position)
	{
		position_ = position;
	}

	template<typename T>
	void Transform<T>::setOrientation(const glm::vec3& orientation)
	{
		orientation_ = glm::quat(glm::radians(orientation));
	}

	template<typename T>
	void Transform<T>::setOrientation(const glm::quat& orientation)
	{
		orientation_ = orientation;
	}

	template<typename T>
	void Transform<T>::setScale(float scale)
	{
		scale_ = { scale, scale, scale };
	}

	template<typename T>
	void Transform<T>::setScale(const glm::vec3& scale)
	{
		scale_ = scale;
	}

	template<typename T>
	const Position<T> Transform<T>::getPosition() const
	{
		return position_;
	}

	template<typename T>
	const glm::vec3 Transform<T>::getOrientation() const
	{
		return glm::degrees(glm::eulerAngles(orientation_));
	}

	template<typename T>
	const glm::quat Transform<T>::getOrientationQuaternion() const
	{
		return orientation_;
	}

	template<typename T>
	const glm::vec3 Transform<T>::getScale() const
	{
		return scale_;
	}

	template<typename T>
	void Transform<T>::useModelAxes(bool use)
	{
		useModelAxes_ = use;
	}

	template<typename T>
	void Transform<T>::moveX(float distance)
	{
		move({ distance, 0, 0 });
	}

	template<typename T>
	void Transform<T>::moveY(float distance)
	{
		move({ 0, distance, 0 });
	}

	template<typename T>
	void Transform<T>::moveZ(float distance)
	{
		move({ 0, 0, distance });
	}

	template<typename T>
	void Transform<T>::move(const glm::vec3& vector)
	{
		if (useModelAxes_) {
			position_ += vector * orientation_;
		}
		else {
			position_ += vector;
		}
	}

	template<typename T>
	void Transform<T>::yaw(float angle)
	{
		//glm::quat q = glm::angleAxis(glm::radians(angle), orientation_ * glm::vec3(0, 1, 0));
		//rotate(q);
		rotate(angle, { 0, 1, 0 });
	}

	template<typename T>
	void Transform<T>::pitch(float angle)
	{
		rotate(angle, { 1, 0, 0 });
	}

	template<typename T>
	void Transform<T>::roll(float angle)
	{
		rotate(angle, { 0, 0, 1 });
	}

	template<typename T>
	void Transform<T>::rotate(float angle, const glm::vec3& axis)
	{
		glm::quat rotation = glm::angleAxis(glm::radians(angle), axis);

		if (useModelAxes_) {
			orientation_ = rotation * orientation_;
		}
		else {
			orientation_ *= rotation;
		}

		orientation_ = glm::normalize(orientation_);
	}

	template<typename T>
	void Transform<T>::scaleX(float factor)
	{
		scale({ factor, 0, 0 });
	}

	template<typename T>
	void Transform<T>::scaleY(float factor)
	{
		scale({ 0, factor, 0 });
	}

	template<typename T>
	void Transform<T>::scaleZ(float factor)
	{
		scale({ 0, 0, factor });
	}

	template<typename T>
	void Transform<T>::scale(float factor)
	{
		scale_ *= factor;
	}

	template<typename T>
	void Transform<T>::scale(const glm::vec3& factor)
	{
		scale_ *= factor;
	}

	template<typename T>
	const glm::mat4 Transform<T>::getRotateMatrix() const
	{
		//return glm::inverse(glm::mat4_cast(orientation_));
		return glm::mat4_cast(glm::conjugate(orientation_));
	}

	template<typename T>
	const glm::mat4 Transform<T>::getModelMatrix() const
	{
		return getModelMatrix_({ position_.x, position_.y, position_.z });
	}

	template<typename T>
	const glm::mat4 Transform<T>::getModelMatrix(const Position<T>& cameraPosition) const
	{
		Position<T> distance = position_ - cameraPosition;

		return getModelMatrix_({ distance.x, distance.y, distance.z });
	}

	template<typename T>
	const glm::mat4 Transform<T>::getModelMatrix_(const glm::vec3& position) const
	{
		glm::mat4 translate = glm::translate(glm::mat4(1), position);
		glm::mat4 scale = glm::scale(glm::mat4(1), scale_);

		return translate * getRotateMatrix() * scale;
	}
}
