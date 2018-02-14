#pragma once

#include <Framework.hpp>

//#define USE_REALISTIC_SCALE

#include "GameObject.h"
#include "Camera.h"

#include <memory>

namespace Game
{
	using namespace Framework;

	class CoordinateSystem abstract : public GameObject
	{
	public:
		std::string name;
		float scale;
		float radius;
		std::vector<std::unique_ptr<CoordinateSystem>> children;

		static void createMesh();
		void draw(const Camera& camera) const;

	protected:
		struct CameraHierarchy
		{
			CoordinateSystem* coordinateSystem;
			glm::mat4 rotation;
			Vector3 position;
		};

		static GLuint vao_;
		static ShaderProgram* shaderProgram_;

		void drawRecursively_(
			const glm::mat4& pv,
			const glm::mat4& pv2,
			const std::vector<CameraHierarchy>& cameraHierarchy,
			int hierarchyIndex,
			int numberOfSubLevelsToDraw,
			glm::mat4 rotations = glm::mat4(1),
			glm::vec3 camPos = glm::vec3(1),
			int depth = 1,
			bool useHighRes = true
		) const;
		void draw_(const glm::mat4& matrix, int depth) const;
	};
}
