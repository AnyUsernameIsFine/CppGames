#include "CoordinateSystem.h"

namespace Game
{
	void CoordinateSystem::drawRecursively_(
		std::vector<DrawConfiguration>& map,
		const glm::mat4& pr,									// camera projection matrix * camera rotation matrix
		const std::vector<CameraHierarchyLevel>& hierarchy,		// list of the camera's positions and rotations relative to all its coordinate system's ancestors from inside to outside
		int hierarchyIndex,										// which level of the camera hierarchy should we use for inverse rotations
		int numberOfSubLevelsToDraw,
		glm::mat4 rotations,									// matrix of the combined rotations of all this coordinate system's ancestors
		glm::vec3 camPos,										// camera position relative to this coordinate system's parent (we have to use floating point precision because this will be used for coordinate systems the camera is outside of)
		bool useHighRes
	)
	{
		// define the matrix to be used for drawing
		glm::mat4 m;

		// if this coordinate system is in the hierarchy of the camera's coordinate systems
		if (hierarchyIndex >= 0 && hierarchy[hierarchyIndex].coordinateSystem == this) {
			hierarchyIndex--;

			// if this is the coordinate system the camera is in 
			// we don't need to rotate it at all, and can use the camera's 
			// actual view matrix in stead of just its rotation,
			// except when this is the universe
			if (hierarchyIndex == -1) {
				m = pr;

				if (parent) {
					float r = scale / ((CoordinateSystem*)parent)->scale;
					glm::vec3 v = -hierarchy[hierarchyIndex + 1].position.toVec3() * r;
					m = glm::translate(m, v);
				}

				numberOfSubLevelsToDraw = 2;
			}

			// if this coordinate system is an ancestor of the one the camera is in
			else {
				// use the camera's projection and rotation
				// and the camera's rotation relative to this coordinate system's ancestors
				m = pr;
				if (hierarchyIndex >= 0) {
					m *= hierarchy[hierarchyIndex + 1].rotation;
				}

				// translate by the camera's position relative to this coordinate system
				if (parent) {
					float r = scale / ((CoordinateSystem*)parent)->scale;
					glm::vec3 v = -hierarchy[hierarchyIndex + 1].position.toVec3() * r;
					m = glm::translate(m, v);
				}

				numberOfSubLevelsToDraw = 1;
			}
		}

		// if this coordinate system is not in the hierarchy of the camera's coordinate systems
		else {
			// use the camera's projection and rotation
			// and the camera's rotation relative to it's coordinate systems and its ancestors
			// up to but excluding the first shared ancestor with this coordinate system
			m = pr;
			if (hierarchyIndex >= 0) {
				m *= hierarchy[hierarchyIndex + 1].rotation;
			}

			// rotate by the combined rotations of all this coordinate system's ancestors
			// up to but exluding the first shared ancestor with the hierarchy of the camera's coordinate systems
			float r = 1.0f;
			if (parent->parent) {
				r = ((CoordinateSystem*)parent)->scale / ((CoordinateSystem*)parent->parent)->scale;
			}
			m *= glm::scale(rotations, { r, r, r });

			// rotate by this coordinate system's model matrix
			if (useHighRes) {
				useHighRes = false;

				Vector3 highResCamPos = hierarchy[hierarchyIndex + 1].position;
				m *= transform.getModelMatrix(highResCamPos);

				camPos = (highResCamPos - transform.getPosition()).toVec3();
			}
			else {
				m *= transform.getModelMatrix(camPos);

				camPos -= transform.getPosition().toVec3();
			}

			// add the current coordinate system's rotation to the combined rotations to use by its descendants
			rotations *= transform.getRotationMatrix();

			// calculate the camera position relative to this coordinate system
			camPos = camPos * glm::conjugate(transform.getOrientation());
			camPos *= (((CoordinateSystem*)parent)->scale / scale);
		}

		// make a final scale adjustment according to the coordinate system's radius and draw it
		m = glm::scale(m, { radius, radius, radius });

		map.push_back({ m, this->getColor() });

		// draw the coordinate system's descendants
		if (numberOfSubLevelsToDraw > 0) {
			for (auto& cs : children) {
				cs->drawRecursively_(
					map,
					pr,
					hierarchy,
					hierarchyIndex,
					numberOfSubLevelsToDraw - 1,
					rotations,
					camPos,
					useHighRes
				);
			}
		}
	}

	void CoordinateSystem::draw_(std::vector<DrawConfiguration> drawConfigurations)
	{
		shaderProgram_->use();

		glBindVertexArray(vao_);

		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, drawConfigurations.size() * (sizeof(glm::mat4) + sizeof(glm::vec4)), &drawConfigurations[0], GL_STATIC_DRAW);

		GLsizei vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(DrawConfiguration), (GLvoid*)0);
		glVertexAttribDivisor(1, 1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(DrawConfiguration), (GLvoid*)(1 * vec4Size));
		glVertexAttribDivisor(2, 1);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(DrawConfiguration), (GLvoid*)(2 * vec4Size));
		glVertexAttribDivisor(3, 1);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(DrawConfiguration), (GLvoid*)(3 * vec4Size));
		glVertexAttribDivisor(4, 1);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(DrawConfiguration), (GLvoid*)(4 * vec4Size));
		glVertexAttribDivisor(5, 1);

		glDrawElementsInstanced(GL_POINTS, 24, GL_UNSIGNED_INT, nullptr, drawConfigurations.size());
		glDrawElementsInstanced(GL_LINES, 24, GL_UNSIGNED_INT, nullptr, drawConfigurations.size());
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
