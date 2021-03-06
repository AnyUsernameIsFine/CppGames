#include "CoordinateSystem.h"

namespace SpaceSimulator
{
	CoordinateSystem* CoordinateSystem::getParent() const
	{
		return parent;
	}

	const std::vector<std::shared_ptr<CoordinateSystem>>& CoordinateSystem::getChildren() const
	{
		return children;
	}

	const std::string& CoordinateSystem::getName() const
	{
		return name;
	}

	float CoordinateSystem::getRadius() const
	{
		return radius;
	}

	void CoordinateSystem::initialize()
	{
		vertexArray.setVertexBuffer({ 3, 3, 2 }, 60);
		vertexArray.setIndexBuffer(108 + 1); // why one more???
		vertexArray.setInstanceBuffer({ 16, 16, 4, 1, 1 }, MAX_IN_DRAW_LIST);
		shader.createVertexFragmentFromFiles("Resources/coordinateSystem.vert", "Resources/coordinateSystem.frag");
	}

	void CoordinateSystem::drawWithChildren(
		std::vector<std::vector<std::vector<DrawConfiguration>>>& toDrawList,
		// list of the camera's positions and rotations relative to all its coordinate system's ancestors from outside in
		const std::vector<Camera::CameraHierarchyLevel>& hierarchy,
		// which level of the camera hierarchy should we use for inverse rotations
		int hierarchyIndex,
		// matrix of the combined rotations of all this coordinate system's ancestors
		glm::mat4 rotations,
		glm::mat4 anotherMatrix,
		// camera position relative to this coordinate system's parent (we have to use float{ing point precision because this will be used for coordinate systems the camera is outside of)
		glm::vec3 camPos,
		bool useHighRes,
		int descendantGenerationsToDraw
	)
	{
		// define the matrix to be used for drawing
		glm::mat4 modelMatrix;

		// only draw descendants if there are any
		bool drawDescendants = !children.empty();

		DrawConfiguration drawConfiguration;
		int toDrawListIndex;

		// if this coordinate system is in the hierarchy of the camera's coordinate systems
		if (hierarchyIndex < static_cast<int>(hierarchy.size()) - 1 && hierarchy[hierarchyIndex + 1].coordinateSystem == this) {
			hierarchyIndex++;
			//toDrawListIndex = 0;
			toDrawListIndex = 2;

			// if this coordinate system is an ancestor of the camera's coordinate system,
			// use the camera's rotation relative to this coordinate system's ancestors
			// and draw only one descendant generation
			if (hierarchyIndex < static_cast<int>(hierarchy.size()) - 1) {
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
			if (parent) {
				float r = getScale() / parent->getScale();
				glm::vec3 v = -hierarchy[hierarchyIndex].position.toVec3() * r;
				modelMatrix = glm::translate(modelMatrix, v);
			}

			drawConfiguration = { glm::mat4(1), modelMatrix, getColor(), radius, this };
		}

		// if this coordinate system is not in the hierarchy of the camera's coordinate systems
		else {
			//toDrawListIndex = 1 + descendantGenerationsToDraw;
			toDrawListIndex = 1 - descendantGenerationsToDraw;

			// only draw descendants if we're told to
			drawDescendants &= descendantGenerationsToDraw > 0;

			// rotate by this coordinate system's model matrix
			if (useHighRes) {
				useHighRes = false;
				Vector3 highResCamPos = hierarchy[hierarchyIndex].position;
				modelMatrix = myTransform.getModelMatrix(highResCamPos);

				if (drawDescendants) {
					camPos = (highResCamPos - myTransform.getPosition()).toVec3();
				}
			}
			else {
				modelMatrix = myTransform.getModelMatrix(camPos);

				if (drawDescendants) {
					camPos -= myTransform.getPosition().toVec3();
				}
			}

			if (drawDescendants) {
				// add the current coordinate system's rotation to the combined rotations to use by its descendants
				rotations = glm::mat3(anotherMatrix * modelMatrix); // use the rotation part of the matrix used for drawing

				// calculate the camera position relative to this coordinate system
				camPos = camPos * glm::conjugate(myTransform.getOrientation()) * parent->getScale() / getScale();
			}

			drawConfiguration = { anotherMatrix, modelMatrix, getColor(), radius, this };
		}

		// add this coordinate system to the list of coordinate systems to draw
		toDrawList[hierarchyIndex][toDrawListIndex].emplace_back(drawConfiguration);

		if (drawDescendants) {
			// create a matrix from the combined rotations of all this coordinate system's ancestors
			// up to but excluding the first shared ancestor with the hierarchy
			// of the camera's coordinate systems (which could be the camera's coordinate system itself)
			// and scale by the relative scale of this coordinate system's direct ancestors
			if (parent) {
				float r = getScale() / parent->getScale();
				anotherMatrix = glm::scale(rotations, { r, r, r });
			}
			else {
				anotherMatrix = rotations;
			}

			// draw the coordinate system's descendants
			for (const auto& child : children) {
				child->drawWithChildren(
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

	void CoordinateSystem::myDraw(const std::vector<std::vector<std::vector<DrawConfiguration>>>& toDrawList, const Camera& camera)
	{
		auto hierarchy = camera.getHierarchy();

		float fov = glm::radians(camera.getFieldOfView());
		float ratio = camera.getAspectRatio();

		shader.use();
		shader.setUniform("view", camera.getRotationMatrix());

		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (int i = 0; i < static_cast<int>(toDrawList.size()); i++) {
			glClear(GL_DEPTH_BUFFER_BIT);

			float nearPlane = 0.1f * hierarchy[i].coordinateSystem->getCameraNearPlane();
			float farPlane = nearPlane * 100000.0f;
			shader.setUniform("projection", glm::perspective(fov, ratio, nearPlane, farPlane));

			for (const auto& list : toDrawList[i]) {
				if (!list.empty() && list[0].cs->hasMesh()) {
					vertexArray.updateVertexBuffer(list[0].cs->mesh().getVertices());
					vertexArray.updateIndexBuffer(list[0].cs->mesh().getIndices());
					vertexArray.updateInstanceBuffer(list);
					vertexArray.draw(GL_TRIANGLES);
				}
			}
		}
	}

	VertexArray CoordinateSystem::vertexArray;
	Shader CoordinateSystem::shader;
}
