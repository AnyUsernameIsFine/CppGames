#pragma once

#include <Framework.hpp>

namespace Game
{
	using namespace Framework;

	class Game : public Framework::Game
	{
	public:
		Camera<> camera;

		ShaderProgram* cubeShader;
		ShaderProgram* lightShader;
		Texture2D* texture1;
		Texture2D* texture2;
		GLuint cubeVao;
		GLuint lightVao;
		std::vector<Transform<>> cubes;
		Transform<> light;

		Game()
		{
			graphics.window.setTitle("Learn OpenGL");
			graphics.window.setSize(800, 600);
			graphics.window.hideCursor();
			graphics.window.enableVSync();
		}

		void start()
		{
			graphics.text.setFont("Resources/consola.ttf", 16);
			graphics.text.setColor(1, 1, 1);

			camera.setAspectRatio((float)graphics.window.getWidth() / graphics.window.getHeight());
			camera.transform.moveZ(6);

			for (int i = 0; i < 10; i++) {
				float x = 6 * (float)rand() / RAND_MAX - 3;
				float y = 6 * (float)rand() / RAND_MAX - 3;
				float z = 6 * (float)rand() / RAND_MAX - 3;

				Transform<> cube({ x, y, z });
				cube.scale({ 2, 1, 0.5f });
				cube.rotate(i * 20.0f, { -1.0f, 0.3f, 0.5f });

				cubes.push_back(cube);
			}

			light.setPosition(1.2f, 1.0f, 2.0f);
			light.scale(0.2f);

			texture1 = new Texture2D("Resources/journey.jpg");
			texture2 = new Texture2D("Resources/flow.jpg");

			cubeShader = new ShaderProgram("Resources/cube.vert", "Resources/cube.frag");

			cubeShader->use();
			cubeShader->setUniform("texture1", 0);
			cubeShader->setUniform("texture2", 1);

			lightShader = new ShaderProgram("Resources/light.vert", "Resources/light.frag");

			float vertices[] = {
				-0.5, -0.5, -0.5,	0, 0,
				 0.5, -0.5, -0.5,	1, 0,
				-0.5,  0.5, -0.5,	0, 1,
				 0.5,  0.5, -0.5,	1, 1,
				-0.5, -0.5,  0.5,	0, 0,
				 0.5, -0.5,  0.5,	1, 0,
				-0.5,  0.5,  0.5,	0, 1,
				 0.5,  0.5,  0.5,	1, 1,
			};

			int indices[] = {
				0, 2, 1, 1, 2, 3,
				0, 1, 4, 4, 1, 5,
				0, 4, 2, 2, 4, 6,
				1, 3, 5, 5, 3, 7,
				2, 6, 3, 3, 6, 7,
				4, 5, 6, 6, 5, 7,
			};

			glGenVertexArrays(1, &cubeVao);
			glBindVertexArray(cubeVao);
			VertexBufferObject vbo({ 3, 2 }, 8, vertices);
			IndexBufferObject ibo(36, indices);
			glBindVertexArray(0);

			glGenVertexArrays(1, &lightVao);
			glBindVertexArray(lightVao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo.getId());
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.getId());
			glEnableVertexAttribArray(0);
			glBindVertexArray(0);
		}

		void onKeyDown(SDL_Keycode key)
		{
			if (key == SDLK_p) {
				camera.setPerspective(!camera.isPerspective());
			}
		}

		void onMouseMove(int x, int y)
		{
			float sensitivity = 10 * graphics.getDeltaSeconds();

			camera.transform.yaw(sensitivity * x);
			camera.transform.pitch(sensitivity * y);
		}

		void onMouseWheel(int y)
		{
			camera.setFieldOfView(camera.getFieldOfView() - y * 10);
		}

		void update()
		{
			float cameraSpeed = 5 * graphics.getDeltaSeconds();

			camera.transform.moveX(cameraSpeed * (input.isKeyDown(SDLK_d) - input.isKeyDown(SDLK_a)));
			camera.transform.moveZ(cameraSpeed * (input.isKeyDown(SDLK_s) - input.isKeyDown(SDLK_w)));

			float rollSensitivity = 90 * graphics.getDeltaSeconds();
			camera.transform.roll(rollSensitivity * (input.isKeyDown(SDLK_e) - input.isKeyDown(SDLK_q)));
		}

		void draw()
		{
			graphics.clearScreen(0, 0, 0);

			glm::mat4 view = camera.getViewMatrix();
			glm::mat4 projection = camera.getProjectionMatrix();

			cubeShader->use();
			cubeShader->setUniform("view", view);
			cubeShader->setUniform("projection", projection);
			cubeShader->setUniform("mix", (sin(graphics.getTotalSeconds()) + 1) * 0.5f);
			cubeShader->setUniform("lightColor", glm::vec4(1, 1, 1, 1));

			texture1->use(0);
			texture2->use(1);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);

			glBindVertexArray(cubeVao);

			for (int i = 0; i < (int)cubes.size(); i++) {
				cubes[i].rotate(graphics.getDeltaSeconds() * 50.0f, { 1.0f, 0.3f, 0.5f });
				glm::mat4 model = cubes[i].getModelMatrix();
				cubeShader->setUniform("model", model);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
			}

			lightShader->use();
			glm::mat4 model = light.getModelMatrix();
			lightShader->setUniform("model", model);
			lightShader->setUniform("view", view);
			lightShader->setUniform("projection", projection);
			glBindVertexArray(lightVao);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

			int fps = (int)round(graphics.getFps());
			Vector3<> position = camera.transform.getPosition();
			glm::ivec3 o = camera.transform.getEulerAngles();

			std::wstring fpsString = std::to_wstring(fps) + L"fps";
			std::wstring positionString = L"x: " + std::to_wstring(position.x) + L" y: " + std::to_wstring(position.y) + L" z: " + std::to_wstring(position.z);
			std::wstring orientatonString = L"yaw: " + std::to_wstring(o.y) + L"° pitch: " + std::to_wstring(o.x) + L"° roll: " + std::to_wstring(o.z) + L"°";

			graphics.text.draw(0, 0, fpsString + L"\n" + positionString + L"\n" + orientatonString);
		}
	};
}
