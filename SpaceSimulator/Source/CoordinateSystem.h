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
		virtual glm::vec4 getColor() const = 0;

	protected:
		struct DrawConfiguration
		{
			glm::mat4 matrix;
			glm::vec4 color;
		};

		struct CameraHierarchyLevel
		{
			CoordinateSystem* coordinateSystem;
			glm::mat4 rotation;
			Vector3 position;
		};

		static GLuint vao_;
		static ShaderProgram* shaderProgram_;

		void drawRecursively_(
			std::vector<DrawConfiguration>& map,
			const glm::mat4& pv,
			const glm::mat4& pv2,
			const std::vector<CameraHierarchyLevel>& cameraHierarchy,
			int hierarchyIndex,
			int numberOfSubLevelsToDraw = 1,
			glm::mat4 rotations = glm::mat4(1),
			glm::vec3 camPos = glm::vec3(1),
			bool useHighRes = true
		);
		static void draw_(std::vector<DrawConfiguration> drawConfigurations);
	};
}
