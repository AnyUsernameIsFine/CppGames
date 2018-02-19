#pragma once

#include <Framework.hpp>

//#define USE_REALISTIC_SCALE

#include "Camera.h"

#include <memory>

namespace Game
{
	using namespace Framework;

	class CoordinateSystem abstract
	{
	public:
		Transform transform;

		static void createMesh();

		CoordinateSystem* getParent() const;
		const std::vector<std::shared_ptr<CoordinateSystem>>& getChildren() const;
		const std::string& getName() const;
		float getRadius() const;
		virtual float getScale() const = 0;
		virtual const glm::vec4& getColor() const = 0;

	protected:
		struct DrawConfiguration
		{
			glm::mat4 matrix;
			glm::vec4 color;
		};

		CoordinateSystem* parent_;
		std::vector<std::shared_ptr<CoordinateSystem>> children_;
		std::string name_;
		float radius_;

		void drawRecursively_(
			std::vector<DrawConfiguration>& toDrawList,
			const std::vector<Camera::CameraHierarchyLevel>& cameraHierarchy,
			int hierarchyIndex,
			glm::mat4 rotations = glm::mat4(1),
			glm::vec3 camPos = glm::vec3(1),
			bool useHighRes = true,
			int descendantGenerationsToDraw = 1
		);

		static GLuint vao_;
		static GLuint instanceBuffer_;
		static ShaderProgram* shaderProgram_;

		static void draw_(const std::vector<DrawConfiguration>& drawConfigurations);
	};
}
