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
		int method = 1;
		if (method & 1) {
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


			float ratio;
				if (camera.coordinateSystem->parent) {
					ratio = ((CoordinateSystem*)camera.coordinateSystem->parent)->scale / ((CoordinateSystem*)camera.coordinateSystem)->scale;
				}
				else {
					ratio = 1.0f;
				}
			glm::mat4 s = glm::scale(glm::mat4(1), { ratio, ratio, ratio });

			v *= s;

			// determine camera coordinate sytem hierarchy
			std::vector<glm::mat4> rotations;
			cameraSystem = (CoordinateSystem*)(camera.coordinateSystem);
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
				cameraSystem = (CoordinateSystem*)(cameraSystem->parent);
			}

			// draw galaxies
			for (auto galaxy : coordinateSystems) {
				glm::mat4 m2 = galaxy.transform.getModelMatrix();
				glm::mat4 rs = glm::scale(glm::mat4(1), { galaxy.radius, galaxy.radius, galaxy.radius });
				glm::mat4 matrix = p * v;
				for (auto rotation : rotations) {
					matrix *= rotation;
				}
				matrix *= m2 * rs;
				galaxy.draw(matrix, { 0, 0, 1 });

				// draw stars
				for (auto star : galaxy.coordinateSystems) {
					float r2 = galaxy.scale / scale;
					glm::mat4 s2 = glm::scale(glm::mat4(1), { r2, r2, r2 });

					glm::mat4 m3 = star.transform.getModelMatrix();
					glm::mat4 rs = glm::scale(glm::mat4(1), { star.radius, star.radius, star.radius });
					glm::mat4 matrix = p * v;
					for (auto rotation : rotations) {
						matrix *= rotation;
					}
					matrix *= m2 * s2 * m3 * rs;
					star.draw(matrix, { 1, 0, 0 });

					// draw planets
					for (auto planet : star.coordinateSystems) {
						float r3 = star.scale / galaxy.scale;
						glm::mat4 s3 = glm::scale(glm::mat4(1), { r3, r3, r3 });

						glm::mat4 m4 = planet.transform.getModelMatrix();
						glm::mat4 rs = glm::scale(glm::mat4(1), { planet.radius, planet.radius, planet.radius });
						glm::mat4 matrix = p * v;
						for (auto rotation : rotations) {
							matrix *= rotation;
						}
						matrix *= m2 * s2 * m3 * s3 * m4 * rs;
						planet.draw(matrix, { 1, 1, 0 });

						// draw moons
						for (auto moon : planet.coordinateSystems) {
							float r4 = planet.scale / star.scale;
							glm::mat4 s4 = glm::scale(glm::mat4(1), { r4, r4, r4 });

							glm::mat4 m5 = moon.transform.getModelMatrix();
							glm::mat4 rs = glm::scale(glm::mat4(1), { moon.radius, moon.radius, moon.radius });
							glm::mat4 matrix = p * v;
							for (auto rotation : rotations) {
								matrix *= rotation;
							}
							matrix *= m2 * s2 * m3 * s3 * m4 * s4 * m5 * rs;
							moon.draw(matrix, { 1, 0, 1 });
						}
					}
				}
			}
		}
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
