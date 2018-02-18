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
		void move(const glm::vec3& vector);
		void move(float x, float y, float z);
		void moveX(float distance);
		void moveY(float distance);
		void moveZ(float distance);
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
		const glm::mat4& getRotationMatrix();
		const glm::mat4& getScaleMatrix();
		const glm::mat4& getModelMatrix();
		const glm::mat4& getModelMatrix(const Vector3<T>& cameraPosition);
		const glm::mat4& getModelMatrix(const glm::vec3& cameraPosition);

	private:
		Vector3<T> position_;
		glm::quat orientation_;
		glm::vec3 scale_;
		bool useModelAxes_ = true;

		glm::vec3 translationMatrixPosition_;
		glm::mat4 translationMatrix_ = glm::mat4(1);
		bool isRotationMatrixValid_;
		glm::mat4 scaleMatrix_;
		bool isScaleMatrixValid_;
		glm::mat4 rotationMatrix_;
		bool isRotationScaleMatrixValid_;
		glm::mat4 rotationScaleMatrix_;
		bool isModelMatrixValid_;
		glm::mat4 modelMatrix_;

		const glm::mat4& getModelMatrix_(const glm::vec3& position);
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

		isRotationMatrixValid_ = false;
		isRotationScaleMatrixValid_ = false;
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

		isScaleMatrixValid_ = false;
		isRotationScaleMatrixValid_ = false;
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
	void Transform<T>::move(float x, float y, float z)
	{
		move({ x, y, z });
	}

	template<typename T>
	void Transform<T>::moveX(float distance)
	{
		move(distance, 0, 0);
	}

	template<typename T>
	void Transform<T>::moveY(float distance)
	{
		move(0, distance, 0);
	}

	template<typename T>
	void Transform<T>::moveZ(float distance)
	{
		move(0, 0, distance);
	}

	template<typename T>
	void Transform<T>::rotate(float angle, const glm::vec3& axis)
	{
		glm::quat rotation = glm::angleAxis(glm::radians(angle), axis);

		if (useModelAxes_) {
			setOrientation(glm::normalize(rotation * orientation_));
		}
		else {
			setOrientation(glm::normalize(orientation_ * rotation));
		}
	}

	template<typename T>
	void Transform<T>::yaw(float angle)
	{
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
		setScale(scale_ * scale);
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
	const glm::mat4& Transform<T>::getRotationMatrix()
	{
		if (!isRotationMatrixValid_) {
			rotationMatrix_ = glm::mat4_cast(glm::conjugate(orientation_));
			isRotationMatrixValid_ = true;
		}

		return rotationMatrix_;
	}

	template<typename T>
	const glm::mat4& Transform<T>::getScaleMatrix()
	{
		if (!isScaleMatrixValid_) {
			scaleMatrix_ = glm::scale(glm::mat4(1), scale_);
			isScaleMatrixValid_ = true;
		}

		return scaleMatrix_;
	}

	template<typename T>
	const glm::mat4& Transform<T>::getModelMatrix()
	{
		return getModelMatrix_({ position_.x, position_.y, position_.z });
	}

	template<typename T>
	const glm::mat4& Transform<T>::getModelMatrix(const Vector3<T>& cameraPosition)
	{
		Vector3<T> distance = position_ - cameraPosition;

		return getModelMatrix_({ distance.x, distance.y, distance.z });
	}

	template<typename T>
	const glm::mat4& Transform<T>::getModelMatrix(const glm::vec3& cameraPosition)
	{
		return getModelMatrix_({
			(float)position_.x - cameraPosition.x,
			(float)position_.y - cameraPosition.y,
			(float)position_.z - cameraPosition.z,
			});
	}

	template<typename T>
	const glm::mat4& Transform<T>::getModelMatrix_(const glm::vec3& position)
	{
		if (position != translationMatrixPosition_) {
			translationMatrix_ = glm::translate(glm::mat4(1), position);
			translationMatrixPosition_ = position;
			isModelMatrixValid_ = false;
		}

		if (!isRotationScaleMatrixValid_) {
			rotationScaleMatrix_ = getRotationMatrix() * getScaleMatrix();
			isRotationScaleMatrixValid_ = true;
			isModelMatrixValid_ = false;
		}

		if (!isModelMatrixValid_) {
			modelMatrix_ = translationMatrix_ * rotationScaleMatrix_;
			isModelMatrixValid_ = true;
		}

		return modelMatrix_;
	}
}
