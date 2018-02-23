#pragma once

#include <Framework.hpp>

#define UNIVERSE_SCALE 0

#include "Camera.h"
#include "Mesh.h"

#include <memory>

namespace Game
{
	using namespace Framework;

	class CoordinateSystem abstract
	{
	public:
		Transform transform;
		Mesh* mesh = nullptr;

		static const int MAX_IN_DRAW_LIST_ = 8192;

		static void initialize();

		~CoordinateSystem();
		CoordinateSystem* getParent() const;
		const std::vector<std::shared_ptr<CoordinateSystem>>& getChildren() const;
		const std::string& getName() const;
		float getRadius() const;
		virtual float getScale() const = 0;
		virtual const glm::vec4& getColor() const = 0;
		virtual float getCameraNearPlane() const = 0;

	protected:
		struct DrawConfiguration
		{
			CoordinateSystem* cs;
			glm::mat4 m1;
			glm::mat4 m2;
			glm::vec4 color;
			float radius;
		};

		CoordinateSystem* parent_;
		std::vector<std::shared_ptr<CoordinateSystem>> children_;
		std::string name_;
		float radius_;

		void drawWithChildren_(
			std::vector<std::vector<std::vector<DrawConfiguration>>>& toDrawList,
			const std::vector<Camera::CameraHierarchyLevel>& cameraHierarchy,
			int hierarchyIndex = -1,
			glm::mat4 rotations = glm::mat4(1),
			glm::mat4 anotherMatrix = glm::mat4(1),
			glm::vec3 camPos = glm::vec3(1),
			bool useHighRes = true,
			int descendantGenerationsToDraw = 0
		);

		static GLuint vertexArray_;
		static GLuint vertexBuffer_;
		static GLuint indexBuffer_;
		static GLuint instanceBuffer_;
		static ShaderProgram* shaderProgram_;

		static void draw_(const std::vector<std::vector<std::vector<DrawConfiguration>>>& drawConfigurations, const Camera& camera, float deltaSeconds);
	};
}
