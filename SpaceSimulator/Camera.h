#pragma once

#include "Transform.h"

namespace Game
{
	class CoordinateSystem;

	class Camera : public Framework::Camera<Coordinate>
	{
	public:
		CoordinateSystem* coordinateSystem;

		void move(const glm::vec3& vector);
		void move(float x, float y, float z);
		void moveX(float distance);
		void moveY(float distance);
		void moveZ(float distance);
		void update(float deltaSeconds);
		float getSpeed() const;
		std::string getSpeedString() const;

	private:
		glm::vec3 velocity_;
		glm::vec3 maxVelocity_;
		glm::vec3 acceleration_;

		void setCorrectCoordinateSystem_();
	};
}
