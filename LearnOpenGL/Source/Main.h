#pragma once

#include <Framework.h>

#include <glm\gtc\random.hpp>

#include <ctime>

namespace LearnOpenGL
{
	using namespace Framework;

	class Game : public Framework::Game
	{
	public:
		Camera camera;

		Shader fontShader;
		Shader cubeShader;
		Shader lightShader;
		Texture2D texture1;
		Texture2D texture2;
		VertexArrayObject fontVao;
		VertexArrayObject cubeVao;
		VertexArrayObject lightVao;
		std::vector<Transform> cubes;
		Transform light;

		Game()
		{
			graphics.window.setTitle("Learn OpenGL");
			graphics.window.setSize(800, 600);
			//graphics.window.enableFullscreen();
			//graphics.window.enableResizing();
			graphics.window.enableCursor(false);
			//graphics.window.enableVSync(false);
			graphics.window.enableAntiAliasing();
		}

		void initialize()
		{
			Random::setRandSeed((uInt)getGameTimeInSeconds());

			graphics.text.loadFont("Resources/consola.ttf", "Font");
			graphics.text.setFont("Font", 20);

			camera.transform.moveZ(10);

			for (int i = 0; i < 10; i++) {
				Transform cube(Vector3::fromVec3(glm::sphericalRand(3.0f)));
				cube.scale(2, 1, 0.5f);
				cube.rotate(i * 20.0f, { -1.0f, 0.3f, 0.5f });

				cubes.push_back(cube);
			}

			light.scale(0.2f);

			fontShader.createFromFiles("Resources/font.vert", "Resources/font.frag");
			fontShader.use();
			fontShader.setUniform("fontTexture", 0);

			float fontVertices[] = {
				-1, -1,		0, 1,
				 1, -1,		1, 1,
				-1,  1,		0, 0,
				 1,  1,		1, 0,
			};

			fontVao.create();
			VertexBufferObject fontVbo({ 2, 2 }, 4, fontVertices);

			cubeShader.createFromFiles("Resources/cube.vert", "Resources/cube.frag");
			texture1.create("Resources/journey.jpg");
			texture2.create("Resources/flow.jpg");
			cubeShader.use();
			cubeShader.setUniform("texture1", 0);
			cubeShader.setUniform("texture2", 1);

			float cubeVertices[] = {
				-0.5, -0.5, -0.5,	 0,  0, -1,		0, 0,
				-0.5, -0.5, -0.5,	 0, -1,  0,		0, 0,
				-0.5, -0.5, -0.5,	-1,  0,  0,		0, 0,
				 0.5, -0.5, -0.5,	 0,  0, -1,		1, 0,
				 0.5, -0.5, -0.5,	 0, -1,  0,		1, 0,
				 0.5, -0.5, -0.5,	 1,  0,  0,		1, 0,
				-0.5,  0.5, -0.5,	 0,  0, -1,		0, 1,
				-0.5,  0.5, -0.5,	 0,  1,  0,		0, 1,
				-0.5,  0.5, -0.5,	-1,  0,  0,		0, 1,
				 0.5,  0.5, -0.5,	 0,  0, -1,		1, 1,
				 0.5,  0.5, -0.5,	 0,  1,  0,		1, 1,
				 0.5,  0.5, -0.5,	 1,  0,  0,		1, 1,
				-0.5, -0.5,  0.5,	 0,  0,  1,		0, 0,
				-0.5, -0.5,  0.5,	 0, -1,  0,		0, 0,
				-0.5, -0.5,  0.5,	-1,  0,  0,		0, 0,
				 0.5, -0.5,  0.5,	 0,  0,  1,		1, 0,
				 0.5, -0.5,  0.5,	 0, -1,  0,		1, 0,
				 0.5, -0.5,  0.5,	 1,  0,  0,		1, 0,
				-0.5,  0.5,  0.5,	 0,  0,  1,		0, 1,
				-0.5,  0.5,  0.5,	 0,  1,  0,		0, 1,
				-0.5,  0.5,  0.5,	-1,  0,  0,		0, 1,
				 0.5,  0.5,  0.5,	 0,  0,  1,		1, 1,
				 0.5,  0.5,  0.5,	 0,  1,  0,		1, 1,
				 0.5,  0.5,  0.5,	 1,  0,  0,		1, 1,
			};

			GLushort indices[] = {
				 0,  6,  3,  3,  6,  9,
				 1,  4, 13, 13,  4, 16,
				 2, 14,  8,  8, 14, 20,
				 5, 11, 17, 17, 11, 23,
				 7, 19, 10, 10, 19, 22,
				12, 15, 18, 18, 15, 21,
			};

			cubeVao.create();
			VertexBufferObject cubeVbo({ 3, 3, 2 }, 24, cubeVertices);
			IndexBufferObject cubeIbo(36, indices);

			lightShader.createFromFiles("Resources/light.vert", "Resources/light.frag");
			lightVao.create();
			glBindBuffer(GL_ARRAY_BUFFER, cubeVbo.getId());
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIbo.getId());
			glEnableVertexAttribArray(0);

			glBindVertexArray(0);
		}

		void update(float deltaSeconds)
		{
			camera.setAspectRatio((float)graphics.window.getWidth() / graphics.window.getHeight());

			if (input.isKeyDown(SDLK_p)) {
				camera.usePerspective(!camera.usesPerspective());
			}

			float cameraSpeed = 5 * deltaSeconds;
			camera.transform.move(
				cameraSpeed * (input.isKeyDown(SDLK_d) - input.isKeyDown(SDLK_a)),
				cameraSpeed * (input.isKeyDown(SDLK_r) - input.isKeyDown(SDLK_f)),
				cameraSpeed * (input.isKeyDown(SDLK_s) - input.isKeyDown(SDLK_w))
			);

			float rollSensitivity = 90 * deltaSeconds;
			camera.transform.roll(rollSensitivity * (input.isKeyDown(SDLK_e) - input.isKeyDown(SDLK_q)));

			float sensitivity = 0.05f;
			camera.transform.yaw(sensitivity * input.getMouseDeltaX());
			camera.transform.pitch(sensitivity * input.getMouseDeltaY());

			camera.setFieldOfView(camera.getFieldOfView() - input.getMouseWheel() * 10);

			for (auto& cube : cubes) {
				cube.rotate(deltaSeconds * 50.0f, { 1.0f, 0.3f, 0.5f });
			}

			light.setPosition(0, 0, 0);
		}

		void draw()
		{
			graphics.clearScreen(0.12f, 0, 0.06f, true);

			drawFont();
			drawScene();
			drawInfo();
		}

		void drawFont()
		{
			fontShader.use();

			glCheck(glActiveTexture(GL_TEXTURE0));
			glCheck(glBindTexture(GL_TEXTURE_2D, graphics.text.getFontTextureId()));

			fontVao.use();
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		void drawScene()
		{
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);

			glm::mat4 view = camera.getViewMatrix();
			glm::mat4 projection = camera.getProjectionMatrix();

			cubeShader.use();
			cubeShader.setUniform("view", view);
			cubeShader.setUniform("projection", projection);
			cubeShader.setUniform("mix", (sin(getGameTimeInSeconds()) + 1) * 0.5f);
			cubeShader.setUniform("lightColor", glm::vec3(1, 1, 1));

			glm::vec3 p = light.getPosition().toVec3();
			cubeShader.setUniform("lightPosition", glm::vec3(view * glm::vec4(p.x, p.y, p.z, 1)));

			texture1.use(0);
			texture2.use(1);

			cubeVao.use();
			for (auto& cube : cubes) {
				glm::mat4 model = cube.getModelMatrix();
				cubeShader.setUniform("model", model);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);
			}

			glm::mat4 model = light.getModelMatrix();

			lightShader.use();
			lightShader.setUniform("model", model);
			lightShader.setUniform("view", view);
			lightShader.setUniform("projection", projection);

			lightVao.use();
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);
		}

		void drawInfo()
		{
			Vector3 position = camera.transform.getPosition();
			glm::ivec3 o = camera.transform.getEulerAngles();

			StringStream stream;
			stream << (int)round(graphics.getFps()) << "fps" << std::endl;
			stream << "x: " << position.x << " y: " << position.y << " z: " << position.z << std::endl;
			stream << "yaw: " << o.y << u8"° pitch: " << o.x << u8"° roll: " << o.z << u8"°";

			graphics.text(0, 0) << stream;
		}
	};
}
