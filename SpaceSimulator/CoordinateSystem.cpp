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

		v *= s;
		v2 *= s;

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

		int method = 2;
		if (method & 1) {
			// draw galaxies
			for (auto& galaxy : coordinateSystems) {
				glm::mat4 matrix;

				glm::mat4 m2 = galaxy.transform.getModelMatrix();

				if (!cameraSystems.empty() && cameraSystems.back() == &galaxy) {
					matrix = p * v;
				}
				else {
					matrix = p * v;
					for (auto rotation : rotations) {
						matrix *= rotation;
					}
					matrix *= m2;
				}

				if (!cameraSystems.empty() && cameraSystems.back() == &galaxy) {
					cameraSystems.pop_back();
				}

				glm::mat4 rs = glm::scale(glm::mat4(1), { galaxy.radius, galaxy.radius, galaxy.radius });
				galaxy.draw(matrix * rs, { 0, 0, 1 });

				// draw stars
				for (auto& star : galaxy.coordinateSystems) {
					glm::mat4 matrix;

					float r = galaxy.scale / scale;
					glm::mat4 s3 = glm::scale(glm::mat4(1), { r, r, r });
					glm::mat4 m3 = star.transform.getModelMatrix();

					if (!cameraSystems.empty() && cameraSystems.front() == &star) {
						matrix = p * v;
					}
					else {
						matrix = p * v;
						for (auto rotation : rotations) {
							matrix *= rotation;
						}
						matrix *= m2 * s3 * m3;
					}

					if (!cameraSystems.empty() && cameraSystems.back() == &star) {
						cameraSystems.pop_back();
					}

					glm::mat4 rs = glm::scale(glm::mat4(1), { star.radius, star.radius, star.radius });
					star.draw(matrix * rs, { 1, 0, 0 });

					// draw planets
					for (auto& planet : star.coordinateSystems) {
						glm::mat4 matrix;

						float r = star.scale / galaxy.scale;
						glm::mat4 s4 = glm::scale(glm::mat4(1), { r, r, r });
						glm::mat4 m4 = planet.transform.getModelMatrix();

						if (!cameraSystems.empty() && cameraSystems.front() == &planet) {
							matrix = p * v;
						}
						else {
							matrix = p * v;
							for (auto rotation : rotations) {
								matrix *= rotation;
							}
							matrix *= m2 * s3 * m3 * s4 * m4;
						}

						if (!cameraSystems.empty() && cameraSystems.back() == &planet) {
							cameraSystems.pop_back();
						}

						glm::mat4 rs = glm::scale(glm::mat4(1), { planet.radius, planet.radius, planet.radius });
						planet.draw(matrix * rs, { 1, 1, 0 });

						// draw moons
						for (auto& moon : planet.coordinateSystems) {
							glm::mat4 matrix;

							float r = planet.scale / star.scale;
							glm::mat4 s5 = glm::scale(glm::mat4(1), { r, r, r });
							glm::mat4 m5 = moon.transform.getModelMatrix();

							if (!cameraSystems.empty() && cameraSystems.front() == &moon) {
								matrix = p * v;
							}
							else if (moon.parent == camera.coordinateSystem) {
								matrix = p * v2;

								matrix *= s5 * moon.transform.getModelMatrix(camera.transform.getPosition());
							}
							else {
								matrix = p * v;
								for (auto rotation : rotations) {
									matrix *= rotation;
								}
								matrix *= m2 * s3 * m3 * s4 * m4 * s5 * m5;
							}

							if (!cameraSystems.empty() && cameraSystems.back() == &moon) {
								cameraSystems.pop_back();
							}

							glm::mat4 rs = glm::scale(glm::mat4(1), { moon.radius, moon.radius, moon.radius });
							moon.draw(matrix * rs, { 1, 0, 1 });
						}
					}
				}
			}
		}

		if (method & 2) {
			// draw galaxies
			for (auto& galaxy : coordinateSystems) {
				galaxy.drawRecursively(s, glm::mat4(1), camera, cameraSystems, rotations, 1);
			}
		}
	}

	void CoordinateSystem::drawRecursively(const glm::mat4& initialScaleMatrix, glm::mat4 passMatrix, const Camera& camera, std::vector<CoordinateSystem*>& cameraSystems, std::vector<glm::mat4> rotations, int depth) const
	{
		glm::mat4 drawMatrix = camera.getProjectionMatrix() * camera.getViewMatrix() * initialScaleMatrix;

		float r = 1.0f;
		if (parent->parent) {
			r = ((CoordinateSystem*)parent)->scale / ((CoordinateSystem*)parent->parent)->scale;
		}
		glm::mat4 relativeScaleMatrix = glm::scale(glm::mat4(1), { r, r, r });

		glm::mat4 modelMatrix = transform.getModelMatrix();

		passMatrix *= relativeScaleMatrix * modelMatrix;

		if (!cameraSystems.empty() && cameraSystems.front() == this) {
			//passMatrix = camera.getProjectionMatrix() * camera.getViewMatrix(true) * initialScaleMatrix;
		}
		else {
			for (auto rotation : rotations) {
				drawMatrix *= rotation;
			}
			drawMatrix *= passMatrix;
		}

		if (!cameraSystems.empty() && cameraSystems.back() == this) {
			cameraSystems.pop_back();

			//rotations.pop_back();
		}

		drawMatrix *= glm::scale(glm::mat4(1), { radius, radius, radius });
		draw(drawMatrix, depth);

		for (auto& coordinateSystem : coordinateSystems) {
			coordinateSystem.drawRecursively(initialScaleMatrix, passMatrix, camera, cameraSystems, rotations, depth + 1);
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
