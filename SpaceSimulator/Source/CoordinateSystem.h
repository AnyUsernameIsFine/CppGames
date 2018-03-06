#pragma once

#include <memory>

#define UNIVERSE_SCALE 0
#include "Camera.h"

namespace SpaceSimulator
{
	class CoordinateSystem abstract : public GameObject
	{
	public:
		static constexpr int MAX_IN_DRAW_LIST = 1 << 13;

		CoordinateSystem* getParent() const;
		const std::vector<std::shared_ptr<CoordinateSystem>>& getChildren() const;
		const std::string& getName() const;
		float getRadius() const;
		virtual float getScale() const = 0;
		virtual const glm::vec4& getColor() const = 0;
		virtual float getCameraNearPlane() const = 0;

		static void initialize();

	protected:
		struct DrawConfiguration
		{
			glm::mat4 m1;
			glm::mat4 m2;
			glm::vec4 color;
			float radius;
			CoordinateSystem* cs;
		};

		void drawWithChildren(
			std::vector<std::vector<std::vector<DrawConfiguration>>>& toDrawList,
			const std::vector<Camera::CameraHierarchyLevel>& cameraHierarchy,
			int hierarchyIndex = -1,
			glm::mat4 rotations = glm::mat4(1),
			glm::mat4 anotherMatrix = glm::mat4(1),
			glm::vec3 camPos = glm::vec3(1),
			bool useHighRes = true,
			int descendantGenerationsToDraw = 0
		);

		static void myDraw(const std::vector<std::vector<std::vector<DrawConfiguration>>>& drawConfigurations, const Camera& camera);

		CoordinateSystem* parent;
		std::vector<std::shared_ptr<CoordinateSystem>> children;
		std::string name;
		float radius;

		static VertexArray vertexArray;
		static Shader shader;
	};
}
