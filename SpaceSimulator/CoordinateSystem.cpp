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
			glm::quat q = ch.coordinateSystem->transform.getOrientationQuaternion();
			ch.rotation *= glm::mat4_cast(q);

			cameraHierarchy.push_back(ch);

			// TODO: something about loss of precision?
			float r = ch.coordinateSystem->scale / parentCs->scale;
			glm::vec3 p = { ch.position.x * r, ch.position.y * r, ch.position.z * r };
			p = p * q;
			Position<Coordinate> csPos = ch.coordinateSystem->transform.getPosition();
			p += glm::vec3(csPos.x, csPos.y, csPos.z);
			ch.position = Position<Coordinate>(p);

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
		glm::mat4 rotations,									// matrix of the combined rotations of all this coordinate system's ancestors' rotations
		const glm::mat4& pr,									// camera projection matrix * camera rotation matrix
		const glm::mat4& pv,									// camera projection matrix * camera view matrix
		const std::vector<CameraHierarchy>& cameraHierarchy,	// list of the camera's positions and rotations relative to all its ancestors' coordinate systems from inside to outside
		Position<Coordinate> camPos,							// camera position relative to this coordinate system's parent
		int depth												// current depth of this coordinate system relative to the universe (used for coloring only)
	) const
	{
		// define the matrix to be used for drawing
		glm::mat4 m;

		// if this coordinate system is in the hierarchy of the camera's coordinate systems
		if (hierarchyIndex >= 0 && cameraHierarchy[hierarchyIndex].coordinateSystem == this) {
			bool thisIsCameraSystem = (hierarchyIndex == 0);

			camPos = cameraHierarchy[hierarchyIndex].position;

			hierarchyIndex--;

			// if this is the coordinate system the camera is in 
			// we don't need to rotate it at all, and can use the camera's 
			// actual view matrix in stead of just its rotation
			if (thisIsCameraSystem) {
				m = pv;
			}

			// if this coordinate system is an ancestor of the one the camera is in, we will rotate it by only its 
			else {
				m = pr;
				if (hierarchyIndex >= 0) {
					m *= cameraHierarchy[hierarchyIndex].rotation;
				}

				glm::vec3 v = { -camPos.x, -camPos.y, -camPos.z };
				v *= scale / ((CoordinateSystem*)parent)->scale;
				m = glm::translate(m, v);
			}
		}

		// if this coordinate system is not in the hierarchy of the camera's coordinate systems
		else {
			m = pr;
			if (hierarchyIndex >= 0) {
				m *= cameraHierarchy[hierarchyIndex].rotation;
			}

			float r = 1.0f;
			if (parent->parent) {
				r = ((CoordinateSystem*)parent)->scale / ((CoordinateSystem*)parent->parent)->scale;
			}

			m *= glm::scale(rotations, { r, r, r }) * transform.getModelMatrix(camPos);

			rotations *= transform.getRotateMatrix();

			Position<Coordinate> position = camPos - transform.getPosition();
			glm::quat q = glm::conjugate(transform.getOrientationQuaternion());

			// TODO: something about loss of precision?
			glm::vec3 p = { position.x, position.y, position.z };
			p = p * q;
			r = ((CoordinateSystem*)parent)->scale / scale;
			p *= glm::vec3(r, r, r);
			camPos = p;
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
