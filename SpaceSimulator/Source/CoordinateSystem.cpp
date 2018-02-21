#include "CoordinateSystem.h"

namespace Game
{
	CoordinateSystem* CoordinateSystem::getParent() const
	{
		return parent_;
	}

	const std::vector<std::shared_ptr<CoordinateSystem>>& CoordinateSystem::getChildren() const
	{
		return children_;
	}

	const std::string& CoordinateSystem::getName() const
	{
		return name_;
	}

	float CoordinateSystem::getRadius() const
	{
		return radius_;
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

		glGenBuffers(1, &instanceBuffer_);
		glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer_);
		glBufferData(GL_ARRAY_BUFFER, MAX_IN_DRAW_LIST_ * sizeof(DrawConfiguration), nullptr, GL_DYNAMIC_DRAW);

		GLsizei vec4Size = sizeof(glm::vec4);

		// mat4
		for (int i = 0; i < 4; i++) {
			glEnableVertexAttribArray(i + 1);
			glVertexAttribPointer(i + 1, 4, GL_FLOAT, GL_FALSE, sizeof(DrawConfiguration), (GLvoid*)(i * vec4Size));
			glVertexAttribDivisor(i + 1, 1);
		}
		// mat4
		for (int i = 0; i < 4; i++) {
			glEnableVertexAttribArray(i + 5);
			glVertexAttribPointer(i + 5, 4, GL_FLOAT, GL_FALSE, sizeof(DrawConfiguration), (GLvoid*)((i + 4) * vec4Size));
			glVertexAttribDivisor(i + 5, 1);
		}
		// vec4
		glEnableVertexAttribArray(9);
		glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(DrawConfiguration), (GLvoid*)(8 * vec4Size));
		glVertexAttribDivisor(9, 1);
		// float
		glEnableVertexAttribArray(10);
		glVertexAttribPointer(10, 1, GL_FLOAT, GL_FALSE, sizeof(DrawConfiguration), (GLvoid*)(9 * vec4Size));
		glVertexAttribDivisor(10, 1);

		glBindVertexArray(0);

		shaderProgram_ = new ShaderProgram("Resources/simpleColor.vert", "Resources/simpleColor.frag");
	}

	void CoordinateSystem::drawWithChildren_(
		std::vector<DrawConfiguration>& toDrawList,
		const std::vector<Camera::CameraHierarchyLevel>& hierarchy,	// list of the camera's positions and rotations relative to all its coordinate system's ancestors from inside to outside
		int hierarchyIndex,											// which level of the camera hierarchy should we use for inverse rotations
		glm::mat4 rotations,										// matrix of the combined rotations of all this coordinate system's ancestors
		glm::mat4 anotherMatrix,
		glm::vec3 camPos,											// camera position relative to this coordinate system's parent (we have to use floating point precision because this will be used for coordinate systems the camera is outside of)
		bool useHighRes,
		int descendantGenerationsToDraw
	)
	{
		// define the matrix to be used for drawing
		glm::mat4 modelMatrix;

		// only draw descendants if there are any
		bool drawDescendants = !children_.empty();

		// if this coordinate system is in the hierarchy of the camera's coordinate systems
		if (hierarchyIndex >= 0 && hierarchy[hierarchyIndex].coordinateSystem == this) {
			// if this coordinate system is an ancestor of the camera's coordinate system,
			// use the camera's rotation relative to this coordinate system's ancestors
			// and draw only one descendant generation
			if (hierarchyIndex > 0) {
				modelMatrix = hierarchy[hierarchyIndex].rotation;
				descendantGenerationsToDraw = 1;
			}

			// if this is the camera's coordinate system, we draw the first two descendant generations
			else {
				modelMatrix = glm::mat4(1);
				descendantGenerationsToDraw = 2;
			}

			// set the rotations for the descendants of this coordinate system
			rotations = modelMatrix;

			// if this is not the universe, translate by the
			// camera's position relative to this coordinate system
			if (parent_) {
				float r = getScale() / parent_->getScale();
				glm::vec3 v = -hierarchy[hierarchyIndex].position.toVec3() * r;
				modelMatrix = glm::translate(modelMatrix, v);
			}

			// decrease the hierarchy index
			hierarchyIndex--;

			// add this coordinate system to the list of coordinate systems to draw
			toDrawList.emplace_back(DrawConfiguration{ glm::mat4(1), modelMatrix, getColor(), radius_ });
		}

		// if this coordinate system is not in the hierarchy of the camera's coordinate systems
		else {
			// only draw descendants if we're told to
			drawDescendants &= descendantGenerationsToDraw > 0;

			// rotate by this coordinate system's model matrix
			if (useHighRes) {
				useHighRes = false;

				Vector3 highResCamPos = hierarchy[hierarchyIndex + 1].position;
				modelMatrix = transform.getModelMatrix(highResCamPos);

				if (drawDescendants) {
					camPos = (highResCamPos - transform.getPosition()).toVec3();
				}
			}
			else {
				modelMatrix = transform.getModelMatrix(camPos);

				if (drawDescendants) {
					camPos -= transform.getPosition().toVec3();
				}
			}

			if (drawDescendants) {
				// add the current coordinate system's rotation to the combined rotations to use by its descendants
				rotations = glm::mat3(anotherMatrix * modelMatrix); // use the rotation part of the matrix used for drawing

				// calculate the camera position relative to this coordinate system
				camPos = camPos * glm::conjugate(transform.getOrientation()) * parent_->getScale() / getScale();
			}

			// add this coordinate system to the list of coordinate systems to draw
			toDrawList.emplace_back(DrawConfiguration{ anotherMatrix, modelMatrix, getColor(), radius_ });
		}

		if (drawDescendants) {
			// create a matrix from the combined rotations of all this coordinate system's ancestors
			// up to but excluding the first shared ancestor with the hierarchy
			// of the camera's coordinate systems (which could be the camera's coordinate system itself)
			// and scale by the relative scale of this coordinate system's direct ancestors
			if (parent_) {
				float r = getScale() / parent_->getScale();
				anotherMatrix = glm::scale(rotations, { r, r, r });
			}
			else {
				anotherMatrix = rotations;
			}

			// draw the coordinate system's descendants
			for (auto& child : children_) {
				child->drawWithChildren_(
					toDrawList,
					hierarchy,
					hierarchyIndex,
					rotations,
					anotherMatrix,
					camPos,
					useHighRes,
					descendantGenerationsToDraw - 1
				);
			}
		}
	}

	void CoordinateSystem::draw_(const std::vector<DrawConfiguration>& toDrawList)
	{
		shaderProgram_->use();

		glBindVertexArray(vao_);

		glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer_);
		glBufferSubData(GL_ARRAY_BUFFER, 0, toDrawList.size() * sizeof(DrawConfiguration), &toDrawList[0]);

		glDrawElementsInstanced(GL_POINTS, 24, GL_UNSIGNED_INT, nullptr, toDrawList.size());
		glDrawElementsInstanced(GL_LINES, 24, GL_UNSIGNED_INT, nullptr, toDrawList.size());
	}

	GLuint CoordinateSystem::vao_;
	GLuint CoordinateSystem::instanceBuffer_;
	ShaderProgram* CoordinateSystem::shaderProgram_;
}
