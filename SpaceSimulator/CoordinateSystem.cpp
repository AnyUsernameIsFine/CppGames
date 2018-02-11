#include "CoordinateSystem.h"

namespace Game
{
	glm::mat4 CoordinateSystem::getModelMatrix(Position<Coordinate> cameraPosition)
	{
		glm::mat4 scale = glm::scale(glm::mat4(1), { radius, radius, radius });
		return transform.getModelMatrix(cameraPosition) * scale;
	}

	void CoordinateSystem::draw(const Camera& camera) const
	{
		// draw universe
		glm::mat4 p = camera.getProjectionMatrix();
		glm::mat4 v = camera.getViewMatrix(true);

		// determine the ultimate rotation of the universe as seen through the camera
		glm::mat4 universeRotate = glm::mat4(1);
		CoordinateSystem* cameraSystem = (CoordinateSystem*)camera.coordinateSystem;
		while (cameraSystem) {
			universeRotate *= glm::inverse(cameraSystem->transform.getRotateMatrix());
			cameraSystem = (CoordinateSystem*)cameraSystem->parent;
		}

		// draw the universe
		glm::mat4 matrix = p * v * universeRotate;
		draw(matrix, { 0, 1, 0 });

		// save camera matrices
		p = camera.getProjectionMatrix();
		v = camera.getViewMatrix();
		glm::mat4 v2 = camera.getViewMatrix(true);


		float ratio;
		if (camera.coordinateSystem->parent) {
			ratio = ((CoordinateSystem*)camera.coordinateSystem->parent)->scale / ((CoordinateSystem*)camera.coordinateSystem)->scale;
		}
		else {
			ratio = 1.0f;
		}
		glm::mat4 s = glm::scale(glm::mat4(1), { ratio, ratio, ratio });

		// determine camera coordinate sytem hierarchy
		std::vector<glm::mat4> rotations;
		std::vector<CoordinateSystem*> cameraSystems;
		cameraSystem = (CoordinateSystem*)(camera.coordinateSystem);
		bool first = true;
		while (cameraSystem->parent) {
			float ratio = 1;
			if (cameraSystem->parent->parent) {
				ratio =
					((CoordinateSystem*)cameraSystem->parent)->scale /
					((CoordinateSystem*)cameraSystem->parent->parent)->scale;
			}
			glm::mat4 rotate = glm::scale(glm::mat4(1), { ratio, ratio, ratio });
			rotate *= cameraSystem->transform.getModelMatrix();
			rotations.push_back(glm::inverse(rotate));
			cameraSystems.push_back(cameraSystem);
			cameraSystem = (CoordinateSystem*)(cameraSystem->parent);
		}

		//output("here we go");

			// draw galaxies
		for (auto& galaxy : coordinateSystems) {
			galaxy.drawRecursively(glm::mat4(1), s, camera, cameraSystems, rotations, 1);
		}
	}



	void CoordinateSystem::drawRecursively(
		glm::mat4 passMatrix,
		const glm::mat4& initialScaleMatrix,
		const Camera& camera,
		std::vector<CoordinateSystem*>& cameraSystems,
		std::vector<glm::mat4> rotations,
		int depth
	) const
	{
		glm::mat4 drawMatrix = camera.getProjectionMatrix() * camera.getViewMatrix() * initialScaleMatrix;

		float r = 1.0f;
		if (parent->parent) {
			r = ((CoordinateSystem*)parent)->scale / ((CoordinateSystem*)parent->parent)->scale;
		}
		glm::mat4 relativeScaleMatrix = glm::scale(glm::mat4(1), { r, r, r });

		glm::mat4 modelMatrix = transform.getModelMatrix();



		if (!cameraSystems.empty() && cameraSystems.back() == this) {
			bool thisIsCameraSystem = cameraSystems.front() == this;

			cameraSystems.pop_back();
			rotations.pop_back();

			if (thisIsCameraSystem) {
				passMatrix = glm::mat4(1);
			}
			else {
				for (auto rotation : rotations) {
					drawMatrix *= rotation;
				}
				drawMatrix *= passMatrix;
			}
		}
		else {
			for (auto rotation : rotations) {
				drawMatrix *= rotation;
			}
			passMatrix *= relativeScaleMatrix * modelMatrix;
			drawMatrix *= passMatrix;
		}



		drawMatrix *= glm::scale(glm::mat4(1), { radius, radius, radius });
		draw(drawMatrix, depth);

		for (auto& coordinateSystem : coordinateSystems) {
			coordinateSystem.drawRecursively(passMatrix, initialScaleMatrix, camera, cameraSystems, rotations, depth + 1);
		}
	}

	void CoordinateSystem::draw(const glm::mat4& matrix, int depth) const
	{
		glm::vec3 color;
		switch (depth) {
		case 1:
			color = { 0, 0, 1 };
			break;
		case 2:
			color = { 1, 0, 0 };
			break;
		case 3:
			color = { 1, 1, 0 };
			break;
		case 4:
			color = { 1, 0, 1 };
			break;
		}
		draw(matrix, color);
	}

	void CoordinateSystem::draw(const glm::mat4& matrix, glm::vec3 color) const
	{
		shaderProgram_->use();
		shaderProgram_->setUniform("color", glm::vec4(color, 1));
		shaderProgram_->setUniform("matrix", matrix);

		glBindVertexArray(vao_);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, nullptr);
	}

	void CoordinateSystem::createMesh()
	{
		float vertices[] = {
			-1, -1, -1,
			 1, -1, -1,
			-1,  1, -1,
			 1,  1, -1,
			-1, -1,  1,
			 1, -1,  1,
			-1,  1,  1,
			 1,  1,  1,
		};

		int indices[] = {
			0, 1,	0, 2,	0, 4,
			3, 1,	3, 2,	3, 7,
			5, 1,	5, 4,	5, 7,
			6, 2,	6, 4,	6, 7,
		};

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);
		VertexBufferObject vbo({ 3 }, 8, vertices);
		IndexBufferObject ibo(24, indices);
		glBindVertexArray(0);

		shaderProgram_ = new ShaderProgram("Resources/simpleColor.vert", "Resources/simpleColor.frag");
	}

	GLuint CoordinateSystem::vao_;
	ShaderProgram* CoordinateSystem::shaderProgram_;
}
