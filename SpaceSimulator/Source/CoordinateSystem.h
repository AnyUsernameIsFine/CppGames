#pragma once

#define UNIVERSE_SCALE 0

#include "Camera.h"

#include <memory>

namespace SpaceSimulator
{
	class CoordinateSystem abstract : public GameObject
	{
	public:
		static const int MAX_IN_DRAW_LIST = 1 << 13;

		static void initialize();

		CoordinateSystem* getParent() const;
		const vector<std::shared_ptr<CoordinateSystem>>& getChildren() const;
		const string& getName() const;
		float getRadius() const;
		virtual float getScale() const = 0;
		virtual const glm::vec4& getColor() const = 0;
		virtual float getCameraNearPlane() const = 0;

	protected:
		struct DrawConfiguration
		{
			glm::mat4 m1;
			glm::mat4 m2;
			glm::vec4 color;
			float radius;
			CoordinateSystem* cs;
		};

		CoordinateSystem* parent;
		vector<std::shared_ptr<CoordinateSystem>> children;
		string name;
		float radius;

		void drawWithChildren(
			vector<vector<vector<DrawConfiguration>>>& toDrawList,
			const vector<Camera::CameraHierarchyLevel>& cameraHierarchy,
			int hierarchyIndex = -1,
			glm::mat4 rotations = glm::mat4(1),
			glm::mat4 anotherMatrix = glm::mat4(1),
			glm::vec3 camPos = glm::vec3(1),
			bool useHighRes = true,
			int descendantGenerationsToDraw = 0
		);

		static VertexArray vertexArray;
		static Shader shader;

		static void myDraw(const vector<vector<vector<DrawConfiguration>>>& drawConfigurations, const Camera& camera);
	};
}
