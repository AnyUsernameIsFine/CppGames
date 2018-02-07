#pragma once

#include <glm\gtc\quaternion.hpp>

namespace Framework
{
	class Transform
	{
	public:
		Transform() = default;
		Transform(glm::vec3 position);
		const glm::vec3 getPosition() const;
		const glm::quat getOrientation() const;
		void useModelAxes(bool use = true);
		void moveX(float distance);
		void moveY(float distance);
		void moveZ(float distance);
		void move(const glm::vec3& vector);
		void rotate(float angle, const glm::vec3& axis);
		void rotate(const glm::quat& rotation);
		void yaw(float angle);
		void pitch(float angle);
		void roll(float angle);
		const glm::mat4 getModelMatrix() const;

	private:
		bool useModelAxes_ = true;

		glm::vec3 position_;
		glm::quat orientation_ = glm::quat(1, 0, 0, 0);
	};
}
