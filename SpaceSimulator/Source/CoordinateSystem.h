#pragma once

#include <Framework.hpp>

#define USE_REALISTIC_SCALE

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

		virtual glm::vec4 getColor() const = 0;
		virtual const std::vector<std::unique_ptr<CoordinateSystem>>& getChildren() const = 0;
		static void createMesh();

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
		static GLuint instanceBuffer_;
		static ShaderProgram* shaderProgram_;

		void drawRecursively_(
			std::vector<DrawConfiguration>& toDrawList,
			const std::vector<CameraHierarchyLevel>& cameraHierarchy,
			int hierarchyIndex,
			glm::mat4 rotations = glm::mat4(1),
			glm::vec3 camPos = glm::vec3(1),
			bool useHighRes = true,
			int numberOfSubLevelsToDraw = 1
		);
		static void draw_(const std::vector<DrawConfiguration>& drawConfigurations);
	};
}
