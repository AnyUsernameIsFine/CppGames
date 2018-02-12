#pragma once

#include "Vector3.hpp"

#include <glm\gtc\quaternion.hpp>

namespace Framework
{
	template<typename T = float>
	class Transform
	{
	public:
		Transform(
			const Vector3<T>& position = { 0, 0, 0 },
			const glm::vec3& eulerAngles = { 0, 0, 0 },
			const glm::vec3& scale = { 1, 1, 1 }
		);
		void useModelAxes(bool use = true);
		void setPosition(const Vector3<T>& position);
		void setPosition(T x, T y, T z);
		void setOrientation(const glm::quat& orientation);
		void setEulerAngles(const glm::vec3& eulerAngles);
		void setEulerAngles(float yaw, float pitch, float roll);
		void setScale(const glm::vec3& scale);
		void setScale(float x, float y, float z);
		void setScale(float scale);
		const Vector3<T>& getPosition() const;
		glm::vec3 getEulerAngles() const;
		const glm::quat& getOrientation() const;
		const glm::vec3& getScale() const;
		void move(const Vector3<T>& vector);
		void move(T x, T y, T z);
		void moveX(T distance);
		void moveY(T distance);
		void moveZ(T distance);
		void rotate(float angle, const glm::vec3& axis);
		void yaw(float angle);
		void pitch(float angle);
		void roll(float angle);
		void scale(const glm::vec3& scale);
		void scale(float x, float y, float z);
		void scale(float scale);
		void scaleX(float scale);
		void scaleY(float scale);
		void scaleZ(float scale);
		glm::mat4 getRotateMatrix() const;
		glm::mat4 getModelMatrix() const;
		glm::mat4 getModelMatrix(const Vector3<T>& cameraPosition) const;

	private:
		Vector3<T> position_;
		glm::quat orientation_;
		glm::vec3 scale_;
		bool useModelAxes_ = true;

		glm::mat4 getModelMatrix_(const Vector3<T>& position) const;
	};
}

//#include "Transform.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Framework
{
	template<typename T>
	Transform<T>::Transform(const Vector3<T>& position, const glm::vec3& eulerAngles, const glm::vec3& scale)
	{
		setPosition(position);
		setEulerAngles(eulerAngles);
		setScale(scale);
	}

	template<typename T>
	void Transform<T>::useModelAxes(bool use)
	{
		useModelAxes_ = use;
	}

	template<typename T>
	void Transform<T>::setPosition(const Vector3<T>& position)
	{
		position_ = position;
	}

	template<typename T>
	void Transform<T>::setPosition(T x, T y, T z)
	{
		setPosition({ x, y, z });
	}

	template<typename T>
	void Transform<T>::setOrientation(const glm::quat& orientation)
	{
		orientation_ = orientation;
	}

	template<typename T>
	void Transform<T>::setEulerAngles(const glm::vec3& eulerAngles)
	{
		setOrientation(glm::quat(glm::radians(eulerAngles)));
	}

	template<typename T>
	void Transform<T>::setEulerAngles(float yaw, float pitch, float roll)
	{
		setEulerAngles({ yaw, pitch, roll });
	}

	template<typename T>
	void Transform<T>::setScale(const glm::vec3& scale)
	{
		scale_ = scale;
	}

	template<typename T>
	void Transform<T>::setScale(float x, float y, float z)
	{
		setScale({ x, y, z });
	}

	template<typename T>
	void Transform<T>::setScale(float scale)
	{
		setScale(scale, scale, scale);
	}

	template<typename T>
	const Vector3<T>& Transform<T>::getPosition() const
	{
		return position_;
	}

	template<typename T>
	const glm::quat& Transform<T>::getOrientation() const
	{
		return orientation_;
	}

	template<typename T>
	glm::vec3 Transform<T>::getEulerAngles() const
	{
		return glm::degrees(glm::eulerAngles(orientation_));
	}

	template<typename T>
	const glm::vec3& Transform<T>::getScale() const
	{
		return scale_;
	}

	template<typename T>
	void Transform<T>::move(const Vector3<T>& vector)
	{
		if (useModelAxes_) {
			position_ += vector * orientation_;
		}
		else {
			position_ += vector;
		}
	}

	template<typename T>
	void Transform<T>::move(T x, T y, T z)
	{
		move({ x, y, z });
	}

	template<typename T>
	void Transform<T>::moveX(T distance)
	{
		move(distance, 0, 0);
	}

	template<typename T>
	void Transform<T>::moveY(T distance)
	{
		move(0, distance, 0);
	}

	template<typename T>
	void Transform<T>::moveZ(T distance)
	{
		move(0, 0, distance);
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
	void Transform<T>::scale(const glm::vec3& scale)
	{
		scale_ *= scale;
	}

	template<typename T>
	void Transform<T>::scale(float x, float y, float z)
	{
		scale({ x, y, z });
	}

	template<typename T>
	void Transform<T>::scale(float scale)
	{
		scale_ *= scale;
	}

	template<typename T>
	void Transform<T>::scaleX(float scale)
	{
		scale(scale, 0, 0);
	}

	template<typename T>
	void Transform<T>::scaleY(float scale)
	{
		scale(0, scale, 0);
	}

	template<typename T>
	void Transform<T>::scaleZ(float scale)
	{
		scale(0, 0, scale);
	}

	template<typename T>
	glm::mat4 Transform<T>::getRotateMatrix() const
	{
		return glm::mat4_cast(glm::conjugate(orientation_));
	}

	template<typename T>
	glm::mat4 Transform<T>::getModelMatrix() const
	{
		return getModelMatrix_({ position_.x, position_.y, position_.z });
	}

	template<typename T>
	glm::mat4 Transform<T>::getModelMatrix(const Vector3<T>& cameraPosition) const
	{
		Vector3<T> distance = position_ - cameraPosition;

		return getModelMatrix_({ distance.x, distance.y, distance.z });
	}

	template<typename T>
	glm::mat4 Transform<T>::getModelMatrix_(const Vector3<T>& position) const
	{
		// TODO: something about loss of precision?
		glm::mat4 translate = glm::translate(glm::mat4(1), { position.x, position.y, position.z });
		glm::mat4 scale = glm::scale(glm::mat4(1), scale_);

		return translate * getRotateMatrix() * scale;
	}
}
