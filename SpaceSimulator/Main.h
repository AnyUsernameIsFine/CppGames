#pragma once

#include <Game.h>
#include <GLTexture2D.h>
#include <GLProgram.h>
#include <VertexBufferObject.h>
#include <Error.h>

#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>

#include <string>
#include <sstream>
#include <chrono>
#include <iostream>

namespace Game
{
	using namespace Framework;

	class Game : public Framework::Game
	{
	public:
		GLProgram* program;
		GLTexture2D* texture1;
		GLTexture2D* texture2;
		GLuint VAO;

		glm::vec3 cubePositions[10]{
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		Game()
		{
			graphics.window.setTitle("Space Simulator");
			graphics.window.setSize(960, 540);
			//graphics.window.showCursor(false);
			graphics.window.enableVSync(true);
		}

		void start()
		{
			graphics.text.setFont("Resources/lucon.ttf", 20);
			graphics.text.setColor(1, 1, 1);




			texture1 = new GLTexture2D("Resources/journey.jpg");
			texture2 = new GLTexture2D("Resources/flow.jpg");

			program = new GLProgram("Resources/simple.vert", "Resources/simple.frag");

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

			unsigned int indices[] = {
				0, 2, 1, 1, 2, 3,
				0, 1, 4, 4, 1, 5,
				0, 4, 2, 2, 4, 6,
				1, 3, 5, 5, 3, 7,
				2, 6, 3, 3, 6, 7,
				4, 5, 6, 6, 5, 7,
			};

			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			VertexBufferObject vbo({ 3, 2 }, 8, vertices);

			GLuint EBO;
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		}

		void update()
		{
			if (input.isKeyDown(SDLK_k)) {
				output("k pressed");
			}
		}

		void draw()
		{
			graphics.clearScreen(1, 0, 0.5);

			// OpenGL tutorial
			float seconds = graphics.getTotalSeconds();

			glm::mat4 model = glm::mat4(1);
			model = glm::rotate(model, seconds * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

			glm::mat4 view = glm::mat4(1);
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

			glm::mat4 projection = glm::mat4(1);
			projection = glm::perspective(glm::radians(45.0f), (float)graphics.window.getWidth() / graphics.window.getHeight(), 0.1f, 100.0f);

			program->use();
			program->setUniform("view", view);
			program->setUniform("projection", projection);
			program->setUniform("mix", (sin(seconds) + 1) * 0.5f);

			texture1->use(0);
			texture2->use(1);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);

			glBindVertexArray(VAO);

			for (unsigned int i = 0; i < 10; i++) {
				glm::mat4 model = glm::mat4(1);
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * i;
				model = glm::rotate(model, glm::radians(angle + seconds * 50.0f), glm::vec3(1.0f, 0.3f, 0.5f));
				program->setUniform("model", model);

				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
			}
			//glBindVertexArray(0);
			// end of tutorial

			int fps = (int)round(graphics.getFps());
			graphics.text.draw(0, 0, (std::to_string(fps) + "fps"));
			//graphics.text.draw(0, 0, std::to_wstring(fps) + L"fps\nZażółć gęślą jaźń\nEl veloz murciélago hindú comía feliz cardillo y kiwi.\nLa cigüeña tocaba el saxofón detrás del palenque de paja.");
		}

		void stop()
		{

		}
	};
}
