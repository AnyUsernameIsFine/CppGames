#pragma once

#include <Framework.hpp>

#include "GameObject.h"
#include "Camera.h"

namespace Game
{
	using namespace Framework;

	class CoordinateSystem : public GameObject
	{
	public:
		std::string name;
		float scale = 1;
		float radius = 0;
		std::vector<CoordinateSystem> descendants;

		static void createMesh();
		void draw(const Camera& camera) const;

	protected:
		struct CameraHierarchy {
			CoordinateSystem* coordinateSystem;
			glm::mat4 rotation;
			Position<Coordinate> position;
		};

		static GLuint vao_;
		static ShaderProgram* shaderProgram_;

		void drawRecursively_(
			int hierarchyIndex,
			glm::mat4 passMatrix,
			const glm::mat4& pv,
			const glm::mat4& pv2,
			const std::vector<CameraHierarchy>& cameraHierarchy,
			Position<Coordinate> camPos,
			int depth
		) const;
		void draw_(const glm::mat4& matrix, int depth) const;
	};
}
