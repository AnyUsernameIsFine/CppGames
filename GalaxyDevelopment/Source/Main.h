#pragma once

#include <algorithm>

#include <glm\gtc\quaternion.hpp>
#include <GLEngine.h>

namespace GalaxyDevelopment
{
	using namespace GLEngine;

	class Game : public GLEngine::Game
	{
	public:
		Camera camera;

		class Galaxy : public GameObject
		{
		public:
			glm::vec2 size;
		};
		Galaxy galaxy;

		struct GalaxyParticle
		{
			glm::vec3 position;
			float size;
			glm::vec4 color;
		};

		const int NUM_GALAXY_PARTICLES = 1 << 14;
		std::vector<GalaxyParticle> galaxyParticles;

		Shader galaxyShader;
		VertexArray galaxyVertexArray;

		Game()
		{
			graphics.window.setTitle("GalaxyDevelopment");
			//graphics.window.setSize(640, 360);

			galaxy.size = { 100, 100 };
			galaxy.transform().pitch(90);
		}

		void createGalaxyParticles(const Galaxy& galaxy, int numGalaxyParticles)
		{
			for (int i = 0; i < numGalaxyParticles; i++) {
				float rndX = Random::randFloat();
				float rndY = Random::randFloat();
				float rndZ = Random::randFloat();
				float rndW = Random::randFloat();

				float size = galaxy.size.x * 0.025f * (rndW * rndW + 0.5f);

				float ecc = 0.8f;
				float rot = 10.0f;
				float off = 0.0f;
				float bar = 0.0f;
				float warp1 = 0.0f;
				float warp2 = 0.0f;

				float erad = rndX;
				float theta = rndY * 2.0f * static_cast<float>(M_PI);
				float height = rndZ;

				glm::vec2 pxy = glm::vec2(cos(theta), sin(theta)) * galaxy.size.x * erad;
				glm::vec3 pos = { pxy.x, height * galaxy.size.y * 0.1f, pxy.y };
				glm::vec3 gax = { 0, 1, 0 };

				gax += glm::vec3(1, 0, 1) * erad * warp1;

				float angl = std::min(std::max(0.0f, erad - bar), 1.0f) * rot;

				gax += glm::vec3(1, 0, 1) * erad * angl * warp2;

				glm::quat q = glm::angleAxis(angl, gax);
				glm::quat qi = glm::angleAxis(-angl, gax);

				pos = pos * q;
				pos.x *= ecc;
				pos.z += erad * galaxy.size.x * off;
				pos = pos * qi;

				GalaxyParticle galaxyParticle = {
					pos,
					size,
					glm::vec4(1, 1, 2, 1) * 0.1f,
				};

				galaxyParticles.emplace_back(galaxyParticle);
			}
		}

		void initialize()
		{
			graphics.text.loadFont("Resources/consola.ttf");
			graphics.text.setFont("Consolas", 20);

			camera.setAspectRatio(graphics.window.getWidth(), graphics.window.getHeight());
			camera.transform().moveZ(150);
			camera.setClippingPlanes(0.1f, 1000.0f);

			createGalaxyParticles(galaxy, NUM_GALAXY_PARTICLES);

			galaxyVertexArray.setVertexBuffer({ 3, 1, 4 }, galaxyParticles);
			galaxyShader.createVertexGeometryFragmentFromFiles(
				"Resources/galaxy.vert",
				"Resources/galaxy.geom",
				"Resources/galaxy.frag"
			);

			//glEnable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
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

			camera.setFieldOfView(camera.getFieldOfView() - input.mouse.wheel() * 10);
		}

		void draw()
		{
			graphics.clearScreen(0.1f, 0.1f, 0.1f, true);

			glm::mat4 view = camera.getViewMatrix();
			glm::mat4 projection = camera.getProjectionMatrix();

			galaxyShader.use();
			galaxyShader.setUniform("view", view);
			galaxyShader.setUniform("projection", projection);
			galaxyShader.setUniform("model", galaxy.transform().getModelMatrix());
			galaxyVertexArray.draw(GL_POINTS);

			graphics.text.draw(0, 0) << static_cast<int>(round(graphics.getFps())) << "fps" << std::endl;
		}
	};
}
