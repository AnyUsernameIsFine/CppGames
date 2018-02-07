#pragma once

#include "Position.h"

#include <glm\gtc\quaternion.hpp>

namespace Game
{
	class Transform
	{
	public:
		Transform(const Position& position = { 0, 0, 0 }, const glm::vec3& orientation = { 0, 0, 0 }, const glm::vec3& scale = { 1, 1, 1 });
		void setPosition(const Position& position);
		void setOrientation(const glm::vec3& orientation);
		void setScale(float scale);
		void setScale(const glm::vec3& scale);
		const Position getPosition() const;
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
		void rotate(const glm::quat& rotation);
		void scaleX(float factor);
		void scaleY(float factor);
		void scaleZ(float factor);
		void scale(float factor);
		void scale(const glm::vec3& factor);
		const glm::mat4 getModelMatrix() const;
		const glm::mat4 getModelMatrix(const Position& cameraPosition) const;

	private:
		Position position_;
		glm::quat orientation_;
		glm::vec3 scale_;
		bool useModelAxes_ = true;

		const glm::mat4 getModelMatrix_(const glm::vec3& position) const;
	};
}
