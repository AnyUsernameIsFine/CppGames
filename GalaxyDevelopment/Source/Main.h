#pragma once

#include <GLEngine.h>

namespace GalaxyDevelopment
{
	using namespace GLEngine;

	class Game : public GLEngine::Game
	{
	public:
		Camera camera;
		GameObject cube;
		Texture2D texture;
		Shader shader;
		VertexArray vertexArray;

		Game()
		{
			graphics.window.setTitle("GalaxyDevelopment");
		}

		void initialize()
		{
			graphics.text.loadFont("Resources/consola.ttf");
			graphics.text.setFont("Consolas", 20);

			camera.setAspectRatio(graphics.window.getWidth(), graphics.window.getHeight());
			camera.transform().moveZ(5);

			cube.setMesh<CubeMesh>();

			texture.createFromFile("Resources/texture.png");

			shader.createFromFiles("Resources/cube.vert", "Resources/cube.frag");

			vertexArray.setVertexBuffer({ 3, 3, 2 }, cube.mesh().getVertices());
			vertexArray.setIndexBuffer(cube.mesh().getIndices());

			glEnable(GL_DEPTH_TEST);
		}

		void update(float deltaSeconds)
		{
			float cameraSpeed = 10 * deltaSeconds;
			camera.transform().move(
				cameraSpeed * (input.keyboard.key(SDLK_d) - input.keyboard.key(SDLK_a) + input.controller.leftXAxis()),
				cameraSpeed * (input.keyboard.key(SDLK_r) - input.keyboard.key(SDLK_f) + input.controller.rightShoulder() - input.controller.leftShoulder()),
				cameraSpeed * (input.keyboard.key(SDLK_s) - input.keyboard.key(SDLK_w) + input.controller.leftYAxis())
			);

			float rollSpeed = 90 * deltaSeconds;
			camera.transform().roll(rollSpeed *	(
				input.keyboard.key(SDLK_e) - input.keyboard.key(SDLK_q) +
				input.controller.rightTrigger() - input.controller.leftTrigger() +
				input.mouse.right() - input.mouse.left())
			);

			float mouseSensitivity = 0.05f;
			float controllerSensitivity = 90 * deltaSeconds;
			camera.transform().yaw(mouseSensitivity * input.mouse.deltaX() + controllerSensitivity * input.controller.rightXAxis());
			camera.transform().pitch(mouseSensitivity * input.mouse.deltaY() + controllerSensitivity * input.controller.rightYAxis());
		}

		void draw()
		{
			graphics.clearScreen(0.1f, 0.1f, 0.1f, true);

			glm::mat4 model = cube.transform().getModelMatrix();
			glm::mat4 view = camera.getViewMatrix();
			glm::mat4 projection = camera.getProjectionMatrix();

			texture.use();

			shader.use();
			shader.setUniform("model", model);
			shader.setUniform("view", view);
			shader.setUniform("projection", projection);

			vertexArray.draw(GL_TRIANGLES);

			graphics.text.draw(0, 0) << static_cast<int>(round(graphics.getFps())) << "fps" << std::endl;
		}

		void finalize()
		{

		}
	};
}
