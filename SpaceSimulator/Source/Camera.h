#pragma once

#include "Transform.h"

namespace Game
{
	class CoordinateSystem;

	class Camera : public Framework::Camera<Coordinate>
	{
	public:
		struct CameraHierarchyLevel
		{
			CoordinateSystem* coordinateSystem;
			glm::mat4 rotation;
			Vector3 position;
		};

		CoordinateSystem* coordinateSystem;

		void move(const glm::vec3& vector);
		void move(float x, float y, float z);
		void moveX(float distance);
		void moveY(float distance);
		void moveZ(float distance);
		void update(float deltaSeconds);
		std::vector<CameraHierarchyLevel> getHierarchy() const;
		float getSpeed() const;
		std::string getSpeedString() const;

	private:
		glm::vec3 velocity_;
		glm::vec3 maxVelocity_;
		glm::vec3 acceleration_;

		void setCorrectCoordinateSystem_();
	};
}
