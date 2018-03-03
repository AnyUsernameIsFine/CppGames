#pragma once

#include "Globals.h"

namespace SpaceSimulator
{
	class CoordinateSystem;

	class Camera : public CameraType<Coordinate>
	{
	public:
		struct CameraHierarchyLevel
		{
			CoordinateSystem* coordinateSystem;
			glm::mat4 rotation;
			Vector3 position;
		};

		void setCoordinateSystem(CoordinateSystem* coordinateSystem);
		CoordinateSystem* getCoordinateSystem() const;
		void setPosition(const Vector3& position);
		void setPosition(Coordinate x, Coordinate y, Coordinate z);
		void move(const glm::vec3& vector);
		void move(float x, float y, float z);
		void moveX(float distance);
		void moveY(float distance);
		void moveZ(float distance);
		void update(float deltaSeconds);
		const vector<CameraHierarchyLevel>& getHierarchy() const;
		float getSpeed() const;
		string getSpeedString() const;

	private:
		CoordinateSystem* coordinateSystem;
		glm::vec3 velocity;
		glm::vec3 maxVelocity;
		glm::vec3 acceleration;
		vector<CameraHierarchyLevel> hierarchy;

		void setCorrectCoordinateSystem();
		void createHierarchy();
	};
}
