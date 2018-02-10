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
		std::vector<CoordinateSystem> coordinateSystems;

		glm::mat4 getModelMatrix(Position<Coordinate> cameraPosition);

		static void createMesh();
		void draw(const Camera& camera) const;
		void draw(const glm::mat4& matrix, glm::vec3 color) const;

		void drawRecursively(
			const glm::mat4& initialScaleMatrix,
			glm::mat4 passMatrix,
			const Camera& camera,
			std::vector<CoordinateSystem*>& cameraSystems,
			std::vector<glm::mat4> rotations,
			int depth
		) const;
		void draw(const glm::mat4& matrix, int depth) const;

	protected:
		static GLuint vao_;
		static ShaderProgram* shaderProgram_;
	};
}
