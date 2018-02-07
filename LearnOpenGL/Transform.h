#pragma once

#include <glm\gtc\quaternion.hpp>

namespace Framework
{
	class Transform
	{
	public:
		Transform(glm::vec3 position = { 0, 0, 0 }, glm::quat orientation = { 1, 0, 0, 0 }, glm::vec3 scale = { 1, 1, 1 });
		void setPosition(const glm::vec3& position);
		void setOrientation(const glm::quat& orientation);
		void setScale(const glm::vec3& scale);
		const glm::vec3 getPosition() const;
		const glm::quat getOrientation() const;
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
		void rotate(const glm::quat& rotation);
		void scaleX(float factor);
		void scaleY(float factor);
		void scaleZ(float factor);
		void scale(const glm::vec3& factor);
		const glm::mat4 getModelMatrix() const;

	private:
		bool useModelAxes_ = true;

		glm::vec3 position_;
		glm::quat orientation_;
		glm::vec3 scale_;
	};
}
