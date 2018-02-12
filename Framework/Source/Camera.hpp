#pragma once

#include "Transform.hpp"

namespace Framework
{
	template<typename T = float>
	class Camera
	{
	public:
		Transform<T> transform;

		bool isPerspective() const;
		float getFieldOfView() const;
		void setPerspective(bool perspective = true);
		void setAspectRatio(float ratio);
		void setFieldOfView(float fov);
		void setClippingPlanes(float near, float far);
		void setSize(float size);
		glm::mat4 getViewMatrix(bool rotationOnly = false) const;
		glm::mat4 getProjectionMatrix() const;

	private:
		bool perspective_ = true;
		float ratio_ = 1.0f;
		float fov_ = 60.0f;
		float near_ = 0.1f;
		float far_ = 100.0f;
		float size_ = 10.0f;
	};
}

//#include "Camera.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Framework
{
	template<typename T>
	bool Camera<T>::isPerspective() const
	{
		return perspective_;
	}

	template<typename T>
	float Camera<T>::getFieldOfView() const
	{
		return fov_;
	}

	template<typename T>
	void Camera<T>::setPerspective(bool perspective)
	{
		perspective_ = perspective;
	}

	template<typename T>
	void Camera<T>::setAspectRatio(float ratio)
	{
		ratio_ = ratio;
	}

	template<typename T>
	void Camera<T>::setFieldOfView(float fov)
	{
		fov_ = fov;
	}

	template<typename T>
	void Camera<T>::setClippingPlanes(float near, float far)
	{
		near_ = near;
		far_ = far;
	}

	template<typename T>
	void Camera<T>::setSize(float size)
	{
		size_ = size;
	}

	template<typename T>
	glm::mat4 Camera<T>::getViewMatrix(bool rotationOnly) const
	{
		glm::mat4 rotation = glm::mat4_cast(transform.getOrientation());

		if (rotationOnly) {
			return rotation;
		}
		else {
			// TODO: something about loss of precision?
			Vector3<T> position = transform.getPosition();
			glm::vec3 positionVec3 = { position.x, position.y, position.z };

			return glm::translate(rotation, -positionVec3);
		}
	}

	template<typename T>
	glm::mat4 Camera<T>::getProjectionMatrix() const
	{
		if (perspective_) {
			return glm::perspective(glm::radians(fov_), ratio_, near_, far_);
		}
		else {
			float halfHeight = size_ * 0.5f;
			float halfWidth = halfHeight * ratio_;
			return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, near_, far_);
		}
	}
}
