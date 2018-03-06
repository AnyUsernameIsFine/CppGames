#pragma once

#include "GameObject.h"

namespace GLEngine
{
	template<typename T>
	class CameraType;

	using Camera = CameraType<float>;

	template<typename T>
	class CameraType : public GameObjectType<T>
	{
	public:
		void usePerspective(bool use = true);
		void setFieldOfView(float degrees);
		void setAspectRatio(int width, int height);
		void setClippingPlanes(float near, float far);
		void setSize(float size);
		bool usesPerspective() const;
		float getFieldOfView() const;
		float getAspectRatio() const;
		glm::mat4 getRotationMatrix() const;
		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjectionMatrix() const;

	private:
		bool perspective = true;
		float fieldOfView = 60.0f;
		float aspectRatio = 1.0f;
		float nearClippingPlane = 0.1f;
		float farClippingPlane = 100.0f;
		float size = 10.0f;
	};

	template<typename T>
	void CameraType<T>::usePerspective(bool use)
	{
		perspective = use;
	}

	template<typename T>
	void CameraType<T>::setAspectRatio(int width, int height)
	{
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	}

	template<typename T>
	void CameraType<T>::setFieldOfView(float degrees)
	{
		fieldOfView = degrees;
	}

	template<typename T>
	void CameraType<T>::setClippingPlanes(float near, float far)
	{
		nearClippingPlane = near;
		farClippingPlane = far;
	}

	template<typename T>
	void CameraType<T>::setSize(float size)
	{
		this->size = size;
	}

	template<typename T>
	bool CameraType<T>::usesPerspective() const
	{
		return perspective;
	}

	template<typename T>
	float CameraType<T>::getFieldOfView() const
	{
		return fieldOfView;
	}

	template<typename T>
	float CameraType<T>::getAspectRatio() const
	{
		return aspectRatio;
	}

	template<typename T>
	glm::mat4 CameraType<T>::getRotationMatrix() const
	{
		return glm::mat4_cast(this->myTransform.getOrientation());
	}

	template<typename T>
	glm::mat4 CameraType<T>::getViewMatrix() const
	{
		return glm::translate(getRotationMatrix(), -this->myTransform.getPosition().toVec3());
	}

	template<typename T>
	glm::mat4 CameraType<T>::getProjectionMatrix() const
	{
		if (perspective) {
			return glm::perspective(
				glm::radians(fieldOfView),
				aspectRatio,
				nearClippingPlane,
				farClippingPlane
			);
		}
		else {
			float halfHeight = size * 0.5f;
			float halfWidth = halfHeight * aspectRatio;

			return glm::ortho(
				-halfWidth,
				halfWidth,
				-halfHeight,
				halfHeight,
				nearClippingPlane,
				farClippingPlane
			);
		}
	}
}
