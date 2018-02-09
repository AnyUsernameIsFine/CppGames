#include "CoordinateSystem.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
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
			CoordinateSystem* cameraSystem = (CoordinateSystem*)camera.coordinateSystem->parent;
			while (cameraSystem) {
				universeRotate *= cameraSystem->transform.getRotateMatrix();
				cameraSystem = (CoordinateSystem*)cameraSystem->parent;
			}

			// draw the universe
			glm::mat4 matrix = p * v * glm::inverse(universeRotate);
			draw(matrix, { 0, 1, 0 });

			// save camera matrices
			p = camera.getProjectionMatrix();
			v = camera.getViewMatrix();

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
					}
				}
			}
		}
		if (method & 2) {
			/*
			DRAW UNIVERSE
			*/
			// save camera matrices
			glm::mat4 p = camera.getProjectionMatrix();
			glm::mat4 v = camera.getViewMatrix(true);

			// determine the ultimate rotation of the universe as seen through the camera
			glm::mat4 universeRotate = glm::mat4(1);
			CoordinateSystem* cameraSystem = (CoordinateSystem*)camera.coordinateSystem->parent;
			while (cameraSystem) {
				universeRotate *= cameraSystem->transform.getRotateMatrix();
				cameraSystem = (CoordinateSystem*)cameraSystem->parent;
			}

			// draw the universe
			glm::mat4 matrix = p * v * glm::inverse(universeRotate);
			draw(matrix, { 0, 1, 0 });

			/*
			DRAW GALAXIES
			*/
			// save camera matrices
			p = camera.getProjectionMatrix();
			v = camera.getViewMatrix();

			// determine camera coordinate sytem hierarchy
			glm::mat4 rotate = glm::mat4(1);
			std::vector<CoordinateSystem*> cameraSystems;
			cameraSystem = (CoordinateSystem*)(camera.coordinateSystem);
			while (cameraSystem->parent) {
				if (cameraSystem->parent->parent) {
					float ratio =
						((CoordinateSystem*)cameraSystem->parent)->scale /
						((CoordinateSystem*)cameraSystem->parent->parent)->scale;

					rotate *= glm::scale(glm::mat4(1), { ratio, ratio, ratio });
				}
				rotate *= cameraSystem->transform.getModelMatrix();

				cameraSystems.push_back(cameraSystem);

				cameraSystem = (CoordinateSystem*)(cameraSystem->parent);
			}

			//output("moi");
			//output(cameraSystems[0]);
			//output(&coordinateSystems[0]);

			// draw the galaxies
			for (auto &subSystem : coordinateSystems) {
				subSystem.drawRecursively_(p * v * glm::inverse(rotate), cameraSystems, 1);
			}
		}
	}

	void CoordinateSystem::drawRecursively_(const glm::mat4& matrix, std::vector<CoordinateSystem*> cameraSystems, int depth) const
	{
		//output(this);
		glm::mat4 nextMatrix = matrix;

		if (this->parent->parent) {
			float ratio =
				((CoordinateSystem*)this->parent)->scale /
				((CoordinateSystem*)this->parent->parent)->scale;

			nextMatrix *= glm::scale(glm::mat4(1), { ratio, ratio, ratio });
		}

		if (std::find(cameraSystems.begin(), cameraSystems.end(), this) != cameraSystems.end()) {
			//nextMatrix *= this->transform.getModelMatrix();
			output("Moi");
		}

		//CoordinateSystem* moi = cameraSystems.back();
		//cameraSystems.pop_back();

		nextMatrix *= this->transform.getModelMatrix();

		glm::mat4 radiusScale = glm::scale(glm::mat4(1), { this->radius, this->radius, this->radius });
		this->draw(nextMatrix * radiusScale, { depth > 1 && depth < 4, depth > 2, depth < 2 });

		for (const auto& subSystem : this->coordinateSystems) {
			subSystem.drawRecursively_(nextMatrix, cameraSystems, depth + 1);
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
