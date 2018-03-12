#pragma once

#include "Galaxy.h"

namespace GalaxyDevelopment
{
	using namespace GLEngine;

	class Game : public GLEngine::Game
	{
	public:
		Camera camera;

		Galaxy galaxy;

		VertexArray dustCloudVertexArray;
		Shader dustCloudShader;
		Framebuffer dustCloudFramebuffer;
		VertexArray dustCloudFramebufferVertexArray;
		Shader dustCloudFramebufferShader;
		VertexArray starVertexArray;
		Shader starShader;
		VertexArray h2RegionVertexArray;
		Shader h2RegionShader;

		Game()
		{
			graphics.window.setTitle("GalaxyDevelopment");
			//graphics.window.setSize(640, 360);


			galaxy.transform().pitch(90);
		}

	private:
		void initialize()
		{
			graphics.text.loadFont("Resources/consola.ttf");
			graphics.text.setFont("Consolas", 20);

			camera.setAspectRatio(graphics.window.getWidth(), graphics.window.getHeight());
			camera.transform().moveZ(30000);
			camera.setClippingPlanes(1.0f, 100000.0f);

			galaxy.create();
			galaxy.singleTimeStep(0);

			dustCloudVertexArray.setVertexBuffer({ 3, 1, 3 }, galaxy.getDustClouds());
			dustCloudShader.createVertexGeometryFragmentFromFiles(
				"Resources/particle.vert",
				"Resources/particle.geom",
				"Resources/dust-cloud.frag"
			);

			dustCloudFramebuffer.create(256, 256);

			struct Vertex { float x, y, u, v; };
			std::vector<Vertex> dustCloudFramebufferVertices = {
				{ -1, -1,	0, 0 },
				{  1, -1,	1, 0 },
				{ -1,  1,	0, 1 },
				{  1,  1,	1, 1 },
			};

			dustCloudFramebufferVertexArray.setVertexBuffer({ 2, 2 }, dustCloudFramebufferVertices);
			dustCloudFramebufferShader.createVertexFragmentFromFiles(
				"Resources/framebuffer.vert",
				"Resources/framebuffer.frag"
			);

			starVertexArray.setVertexBuffer({ 3, 1, 3 }, galaxy.getStars());
			starShader.createVertexGeometryFragmentFromFiles(
				"Resources/particle.vert",
				"Resources/particle.geom",
				"Resources/star.frag"
			);

			h2RegionVertexArray.setVertexBuffer({ 3, 1, 3 }, galaxy.getH2Regions());
			h2RegionShader.createVertexGeometryFragmentFromFiles(
				"Resources/particle.vert",
				"Resources/particle.geom",
				"Resources/h2-region.frag"
			);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		}

		void update(float deltaSeconds)
		{
			float cameraSpeed = 10000 * deltaSeconds;
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

			camera.setFieldOfView(camera.getFieldOfView() - input.mouse.wheel() * 10);
		}

		void draw()
		{
			dustCloudFramebuffer.bind();

			graphics.clearScreen(0, 0, 0);

			glm::mat4 view = camera.getViewMatrix();
			glm::mat4 projection = camera.getProjectionMatrix();

			dustCloudShader.use();
			dustCloudShader.setUniform("model", galaxy.transform().getModelMatrix());
			dustCloudShader.setUniform("view", view);
			dustCloudShader.setUniform("projection", projection);
			dustCloudVertexArray.draw(GL_POINTS);

			dustCloudFramebuffer.unbind();

			graphics.clearScreen(0, 0, 0, true);

			dustCloudFramebufferShader.use();
			dustCloudFramebufferVertexArray.draw(GL_TRIANGLE_STRIP);

			starShader.use();
			starShader.setUniform("model", galaxy.transform().getModelMatrix());
			starShader.setUniform("view", view);
			starShader.setUniform("projection", projection);
			starVertexArray.draw(GL_POINTS);

			h2RegionShader.use();
			h2RegionShader.setUniform("model", galaxy.transform().getModelMatrix());
			h2RegionShader.setUniform("view", view);
			h2RegionShader.setUniform("projection", projection);
			h2RegionVertexArray.draw(GL_POINTS);

			graphics.text.draw(0, 0) << static_cast<int>(round(graphics.getFps())) << "fps" << std::endl;
		}
	};
}
