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
		std::wstring name;
		float scale = 1;
		float radius = 0;
		std::vector<CoordinateSystem> coordinateSystems;

		glm::mat4 getModelMatrix(Position<Coordinate> cameraPosition);

		static void createMesh();
		void draw(const Camera& camera) const;
		void draw(const glm::mat4& matrix, glm::vec3 color) const;

	protected:
		static GLuint vao_;
		static ShaderProgram* shaderProgram_;

		void drawRecursively_(const glm::mat4& matrix, std::vector<CoordinateSystem*> cameraSystems, int depth) const;
	};
}
