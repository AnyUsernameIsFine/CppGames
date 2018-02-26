#pragma once

#include "Vector3.hpp"

#define GLM_FORCE_INLINE 
#include <glm\gtc\quaternion.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace Framework
{
	template<typename T>
	class TransformType
	{
	public:
		TransformType(
			const Vector3Type<T>& position = { 0, 0, 0 },
			const glm::vec3& eulerAngles = { 0, 0, 0 },
			const glm::vec3& scale = { 1, 1, 1 }
		);
		void useModelAxes(bool use = true);
		void setPosition(const Vector3Type<T>& position);
		void setPosition(T x, T y, T z);
		void setOrientation(const glm::quat& orientation);
		void setEulerAngles(const glm::vec3& eulerAngles);
		void setEulerAngles(float yaw, float pitch, float roll);
		void setScale(const glm::vec3& scale);
		void setScale(float x, float y, float z);
		void setScale(float scale);
		const Vector3Type<T>& getPosition() const;
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
		void scale(const glm::vec3& factor);
		void scale(float x, float y, float z);
		void scale(float factor);
		void scaleX(float factor);
		void scaleY(float factor);
		void scaleZ(float factor);
		const glm::mat4& getRotationMatrix();
		const glm::mat4& getModelMatrix();
		const glm::mat4& getModelMatrix(const Vector3Type<T>& cameraPosition);
		const glm::mat4& getModelMatrix(const glm::vec3& cameraPosition);

	private:
		Vector3Type<T> position;
		glm::quat orientation;
		glm::vec3 myScale;
		bool modelAxes = true;
		bool isScaled = false;
		bool isRotationMatrixValid;
		glm::mat4 rotationMatrix;
		bool isModelMatrixValid;
		glm::mat4 modelMatrix;
		glm::vec3 modelMatrixPosition;
		glm::vec3 modelMatrixPositionRotated;

		const glm::mat4& myGetModelMatrix(const glm::vec3& position);
	};

	typedef TransformType<float> Transform;
}

namespace Framework
{
	template<typename T>
	TransformType<T>::TransformType(const Vector3Type<T>& position, const glm::vec3& eulerAngles, const glm::vec3& scale)
	{
		setPosition(position);
		setEulerAngles(eulerAngles);
		setScale(scale);
	}

	template<typename T>
	void TransformType<T>::setPosition(const Vector3Type<T>& position)
	{
		this->position = position;
	}

	template<typename T>
	void TransformType<T>::setPosition(T x, T y, T z)
	{
		setPosition({ x, y, z });
	}

	template<typename T>
	void TransformType<T>::setOrientation(const glm::quat& orientation)
	{
		this->orientation = orientation;

		isRotationMatrixValid = false;
		isModelMatrixValid = false;
	}

	template<typename T>
	void TransformType<T>::setEulerAngles(const glm::vec3& eulerAngles)
	{
		setOrientation(glm::quat(glm::radians(eulerAngles)));
	}

	template<typename T>
	void TransformType<T>::setEulerAngles(float yaw, float pitch, float roll)
	{
		setEulerAngles({ yaw, pitch, roll });
	}

	template<typename T>
	void TransformType<T>::setScale(const glm::vec3& scale)
	{
		myScale = scale;

		isScaled = (scale != glm::vec3(1));
		isModelMatrixValid = false;
	}

	template<typename T>
	void TransformType<T>::setScale(float x, float y, float z)
	{
		setScale({ x, y, z });
	}

	template<typename T>
	void TransformType<T>::setScale(float scale)
	{
		setScale(scale, scale, scale);
	}

	template<typename T>
	void TransformType<T>::useModelAxes(bool use)
	{
		modelAxes = use;
	}

	template<typename T>
	const Vector3Type<T>& TransformType<T>::getPosition() const
	{
		return position;
	}

	template<typename T>
	const glm::quat& TransformType<T>::getOrientation() const
	{
		return orientation;
	}

	template<typename T>
	glm::vec3 TransformType<T>::getEulerAngles() const
	{
		return glm::degrees(glm::eulerAngles(orientation));
	}

	template<typename T>
	const glm::vec3& TransformType<T>::getScale() const
	{
		return myScale;
	}

	template<typename T>
	void TransformType<T>::move(const glm::vec3& vector)
	{
		if (modelAxes) {
			position += vector * orientation;
		}
		else {
			position += vector;
		}
	}

	template<typename T>
	void TransformType<T>::move(float x, float y, float z)
	{
		move({ x, y, z });
	}

	template<typename T>
	void TransformType<T>::moveX(float distance)
	{
		move({ distance, 0, 0 });
	}

	template<typename T>
	void TransformType<T>::moveY(float distance)
	{
		move({ 0, distance, 0 });
	}

	template<typename T>
	void TransformType<T>::moveZ(float distance)
	{
		move({ 0, 0, distance });
	}

	template<typename T>
	void TransformType<T>::rotate(float angle, const glm::vec3& axis)
	{
		glm::quat rotation = glm::angleAxis(glm::radians(angle), axis);

		if (modelAxes) {
			setOrientation(glm::normalize(rotation * orientation));
		}
		else {
			setOrientation(glm::normalize(orientation * rotation));
		}
	}

	template<typename T>
	void TransformType<T>::yaw(float angle)
	{
		rotate(angle, { 0, 1, 0 });
	}

	template<typename T>
	void TransformType<T>::pitch(float angle)
	{
		rotate(angle, { 1, 0, 0 });
	}

	template<typename T>
	void TransformType<T>::roll(float angle)
	{
		rotate(angle, { 0, 0, 1 });
	}

	template<typename T>
	void TransformType<T>::scale(const glm::vec3& factor)
	{
		setScale(myScale * factor);
	}

	template<typename T>
	void TransformType<T>::scale(float x, float y, float z)
	{
		scale({ x, y, z });
	}

	template<typename T>
	void TransformType<T>::scale(float factor)
	{
		scale({ factor, factor, factor });
	}

	template<typename T>
	void TransformType<T>::scaleX(float factor)
	{
		scale({ factor, 0, 0 });
	}

	template<typename T>
	void TransformType<T>::scaleY(float factor)
	{
		scale({ 0, factor, 0 });
	}

	template<typename T>
	void TransformType<T>::scaleZ(float factor)
	{
		scale({ 0, 0, factor });
	}

	template<typename T>
	const glm::mat4& TransformType<T>::getRotationMatrix()
	{
		if (!isRotationMatrixValid) {
			isRotationMatrixValid = true;

			rotationMatrix = glm::mat4_cast(glm::conjugate(orientation));
		}

		return rotationMatrix;
	}

	template<typename T>
	const glm::mat4& TransformType<T>::getModelMatrix()
	{
		return myGetModelMatrix(position.toVec3());
	}

	template<typename T>
	const glm::mat4& TransformType<T>::getModelMatrix(const Vector3Type<T>& cameraPosition)
	{
		return myGetModelMatrix((position - cameraPosition).toVec3());
	}

	template<typename T>
	const glm::mat4& TransformType<T>::getModelMatrix(const glm::vec3& cameraPosition)
	{
		return myGetModelMatrix(position.toVec3() - cameraPosition);
	}

	template<typename T>
	const glm::mat4& TransformType<T>::myGetModelMatrix(const glm::vec3& position)
	{
		if (position != modelMatrixPosition || !isRotationMatrixValid) {
			modelMatrixPosition = position;
			modelMatrixPositionRotated = orientation * position;

			isModelMatrixValid = false;
		}

		if (!isModelMatrixValid) {
			isModelMatrixValid = true;

			if (isScaled) {
				modelMatrix = glm::scale(glm::translate(getRotationMatrix(), modelMatrixPositionRotated), myScale);
			}
			else {
				modelMatrix = glm::translate(getRotationMatrix(), modelMatrixPositionRotated);
			}
		}

		return modelMatrix;
	}
}
