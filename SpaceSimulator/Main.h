#pragma once

#include <Framework.hpp>

namespace Game
{
	typedef int_least64_t Coordinate;

	using namespace Framework;

	class Game : public Framework::Game
	{
		float SCALE = 1000;

	public:
		ShaderProgram * program;
		Texture2D* texture1;
		Texture2D* texture2;
		GLuint VAO;
		Camera<Coordinate> camera;
		std::vector<Transform<Coordinate>> cubes;

		Game()
		{
			graphics.window.setTitle("SpaceSimulator");
			graphics.window.setSize(960, 540);
			graphics.window.hideCursor();
			graphics.window.enableVSync();
		}

		void start()
		{
			for (int i = 0; i < 10; i++) {
				float x = 6 * (float)rand() / RAND_MAX - 3;
				float y = 6 * (float)rand() / RAND_MAX - 3;
				float z = 6 * (float)rand() / RAND_MAX - 3;
				glm::vec3 position(x, y, z);
				position *= SCALE;
				Transform<Coordinate> cube = Transform<Coordinate>(position);
				cube.setScale(SCALE);
				cubes.push_back(cube);
			}

			graphics.text.setFont("Resources/consola.ttf", 16);
			graphics.text.setColor(1, 1, 1);

			camera.setAspectRatio((float)graphics.window.getWidth() / graphics.window.getHeight());
			camera.setClippingPlanes(0.1f * SCALE, 100.0f * SCALE);
			camera.transform.moveZ(6 * SCALE);

			texture1 = new Texture2D("Resources/journey.jpg");
			texture2 = new Texture2D("Resources/flow.jpg");

			program = new ShaderProgram("Resources/simple.vert", "Resources/simple.frag");

			program->use();
			program->setUniform("texture1", 0);
			program->setUniform("texture2", 1);

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

			for (int i = 0; i < (int)cubes.size(); i++) {
				cubes[i].useModelAxes(false);
				cubes[i].moveX(100000000 * SCALE);

				cubes[i].useModelAxes(true);
				cubes[i].scale({ 2, 1, 0.5f });
				cubes[i].rotate(i * 20.0f, { -1.0f, 0.3f, 0.5f });

			}
			camera.transform.moveX(100000000 * SCALE);

			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			VertexBufferObject vbo({ 3, 2 }, 8, vertices);
			IndexBufferObject ibo(36, indices);
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
			float cameraSpeed = 5 * SCALE * graphics.getDeltaSeconds();

			camera.transform.moveX(cameraSpeed * (input.isKeyDown(SDLK_d) - input.isKeyDown(SDLK_a)));
			camera.transform.moveZ(cameraSpeed * (input.isKeyDown(SDLK_s) - input.isKeyDown(SDLK_w)));

			float rollSensitivity = 90 * graphics.getDeltaSeconds();
			camera.transform.roll(rollSensitivity * (input.isKeyDown(SDLK_e) - input.isKeyDown(SDLK_q)));
		}

		void draw()
		{
			graphics.clearScreen(1, 0, 0.5);

			Position<Coordinate> position = camera.transform.getPosition();
			glm::mat4 view = camera.getViewMatrix(true);
			glm::mat4 projection = camera.getProjectionMatrix();

			program->use();
			program->setUniform("view", view);
			program->setUniform("projection", projection);
			program->setUniform("mix", (sin(graphics.getTotalSeconds()) + 1) * 0.5f);

			texture1->use(0);
			texture2->use(1);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);

			glBindVertexArray(VAO);

			for (int i = 0; i < (int)cubes.size(); i++) {
				cubes[i].rotate(graphics.getDeltaSeconds() * 50.0f, { 1.0f, 0.3f, 0.5f });

				glm::mat4 model = cubes[i].getModelMatrix(position);

				program->setUniform("model", model);

				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
			}

			int fps = (int)round(graphics.getFps());
			glm::ivec3 o = camera.transform.getOrientation();

			std::wstring fpsString = std::to_wstring(fps) + L"fps";
			std::wstring positionString = L"x: " + std::to_wstring(position.x) + L" y: " + std::to_wstring(position.y) + L" z: " + std::to_wstring(position.z);
			std::wstring orientatonString = L"yaw: " + std::to_wstring(o.y) + L"° pitch: " + std::to_wstring(o.x) + L"° roll: " + std::to_wstring(o.z) + L"°";

			graphics.text.draw(0, 0, fpsString + L"\n" + positionString + L"\n" + orientatonString);
		}
	};
}
