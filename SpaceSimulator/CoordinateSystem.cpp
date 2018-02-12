#include "CoordinateSystem.h"

namespace Game
{
	void CoordinateSystem::draw(const Camera& camera) const
	{
		// initialize some variables
		std::vector<CameraHierarchy> cameraHierarchy;

		CameraHierarchy ch {
			camera.coordinateSystem,
			glm::mat4(1),
			camera.transform.getPosition(),
		};

		CoordinateSystem* parentCs = (CoordinateSystem*)ch.coordinateSystem->parent;

		// determine initial scale ratio (inverse of the camera's coordinate system's scale to its parent's)
		float r = 1.0f;
		if (parentCs) {
			r = parentCs->scale / ch.coordinateSystem->scale;
		}
		glm::vec3 s = { r, r, r };

		// determine camera coordinate system hierarchy
		while (parentCs) {
			glm::quat q = ch.coordinateSystem->transform.getOrientation();
			ch.rotation *= glm::mat4_cast(q);

			cameraHierarchy.push_back(ch);

			// TODO: reorder these next two operations for added precision?
			ch.position *= q;
			ch.position *= ch.coordinateSystem->scale / parentCs->scale;
			ch.position += ch.coordinateSystem->transform.getPosition();

			ch.coordinateSystem = parentCs;

			parentCs = (CoordinateSystem*)parentCs->parent;
		}

		// save projection and view matrices
		glm::mat4 p = camera.getProjectionMatrix();
		glm::mat4 pr = p * camera.getViewMatrix(true);
		glm::mat4 pv = p * camera.getViewMatrix();

		// draw the universe
		glm::mat4 matrix = pr * ch.rotation;
		draw_(matrix, 0);

		// draw galaxies
		for (auto& cs : descendants) {
			cs.drawRecursively_(
				cameraHierarchy.size() - 1, 
				glm::mat4(1),
				glm::scale(pr, s),
				glm::scale(pv, s),
				cameraHierarchy,
				ch.position, 
				1
			);
		}
	}

	void CoordinateSystem::drawRecursively_(
		int hierarchyIndex,										// which level of the camera hierarchy should we use for inverse rotations
		glm::mat4 rotations,									// matrix of the combined rotations of all this coordinate system's ancestors
		const glm::mat4& pr,									// camera projection matrix * camera rotation matrix
		const glm::mat4& pv,									// camera projection matrix * camera view matrix
		const std::vector<CameraHierarchy>& cameraHierarchy,	// list of the camera's positions and rotations relative to all its coordinate system's ancestors from inside to outside
		Vector3 camPos,											// camera position relative to this coordinate system's parent
		int depth												// current depth of this coordinate system relative to the universe (used for coloring only)
	) const
	{
		// define the matrix to be used for drawing
		glm::mat4 m;

		// if this coordinate system is in the hierarchy of the camera's coordinate systems
		if (hierarchyIndex >= 0 && cameraHierarchy[hierarchyIndex].coordinateSystem == this) {
			bool thisIsCameraSystem = (hierarchyIndex == 0);

			// save the camera position relative to the current coordinate system
			camPos = cameraHierarchy[hierarchyIndex].position;

			hierarchyIndex--;

			// if this is the coordinate system the camera is in 
			// we don't need to rotate it at all, and can use the camera's 
			// actual view matrix in stead of just its rotation
			if (thisIsCameraSystem) {
				m = pv;
			}

			// if this coordinate system is an ancestor of the one the camera is in
			else {
				// use the camera's projection and rotation
				// and the camera's rotation relative to this coordinate system's ancestors
				m = pr;
				if (hierarchyIndex >= 0) {
					m *= cameraHierarchy[hierarchyIndex].rotation;
				}

				// translate by the camera's position relative to this coordinate system
				glm::vec3 v = { -camPos.x, -camPos.y, -camPos.z };
				v *= scale / ((CoordinateSystem*)parent)->scale;
				m = glm::translate(m, v);
			}
		}

		// if this coordinate system is not in the hierarchy of the camera's coordinate systems
		else {
			// use the camera's projection and rotation
			// and the camera's rotation relative to it's coordinate systems and its ancestors
			// up to but excluding the first shared ancestor with this coordinate system
			m = pr;
			if (hierarchyIndex >= 0) {
				m *= cameraHierarchy[hierarchyIndex].rotation;
			}

			// rotate by the combined rotations of all this coordinate system's ancestors
			// up to but exluding the first shared ancestor with the hierarchy of the camera's coordinate systems
			// and rotate by this coordinate system's model matrix
			float r = 1.0f;
			if (parent->parent) {
				r = ((CoordinateSystem*)parent)->scale / ((CoordinateSystem*)parent->parent)->scale;
			}
			m *= glm::scale(rotations, { r, r, r }) * transform.getModelMatrix(camPos);

			// add the current coordinate system's rotation to the combined rotations to use by its descendants
			rotations *= transform.getRotateMatrix();

			// calculate the camera position relative to this coordinate system
			camPos -= transform.getPosition();
			// TODO: reorder these next two operations for added precision?
			camPos *= glm::conjugate(transform.getOrientation());
			camPos *= ((CoordinateSystem*)parent)->scale / scale;
		}

		// make a final scale adjustment according to the coordinate system's radius and draw it
		m = glm::scale(m, { radius, radius, radius });
		draw_(m, depth);

		// draw the coordinate system's descendants
		for (auto& cs : descendants) {
			cs.drawRecursively_(
				hierarchyIndex,
				rotations, 
				pr, 
				pv,
				cameraHierarchy,
				camPos, 
				depth + 1
			);
		}
	}

	void CoordinateSystem::draw_(const glm::mat4& matrix, int depth) const
	{
		glm::vec3 colors[] = {
			{ 0, 1, 0 },
			{ 0, 0, 1 },
			{ 1, 0, 0 },
			{ 1, 1, 0 },
			{ 1, 0, 1 },
			{ 0, 1, 1 },
		};

		shaderProgram_->use();
		shaderProgram_->setUniform("color", glm::vec4(colors[depth], 1));
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
