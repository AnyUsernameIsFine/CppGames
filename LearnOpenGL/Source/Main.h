#pragma once

#include "Globals.h"

#include <glm\gtc\random.hpp>

#include <ctime>

namespace LearnOpenGL
{
	class Game : public Framework::Game
	{
	public:
		Camera camera;
		Shader fontShader;
		Shader shapeShader;
		Shader lightShader;
		Texture2D texture1;
		Texture2D texture2;
		VertexArray fontVertexArray;
		VertexArray shapeVertexArray;
		VertexArray lightVertexArray;
		std::vector<GameObject> shapes;
		GameObject light;

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

			camera.transform().moveZ(20);

			for (int i = 0; i < 10; i++) {
				GameObject shape;
				shape.setMesh<CubeMesh>();
				shape.transform().setPosition(Vector3::fromVec3(glm::sphericalRand(6.0f)));
				shape.transform().scale(2, 1, 0.5f);
				shape.transform().rotate(i * 20.0f, { -1.0f, 0.3f, 0.5f });

				shapes.emplace_back(shape);
			}

			light.setMesh<TetrahedronMesh>();
			light.transform().scale(0.25f);

			float fontVertices[] = {
				-1, -1,		0, 1,
				 1, -1,		1, 1,
				-1,  1,		0, 0,
				 1,  1,		1, 0,
			};

			fontShader.createFromFiles("Resources/font.vert", "Resources/font.frag");
			fontShader.use();
			fontShader.setUniform("fontTexture", 0);
			fontVertexArray.setVertexBuffer({ 2, 2 }, 4, fontVertices);

			texture1.createFromFile("Resources/journey.jpg");
			texture2.createFromFile("Resources/flow.jpg");

			shapeShader.createFromFiles("Resources/shape.vert", "Resources/shape.frag");
			shapeShader.use();
			shapeShader.setUniform("texture1", 0);
			shapeShader.setUniform("texture2", 1);
			shapeVertexArray.setVertexBuffer({ 3, 3, 2 }, 60);
			shapeVertexArray.setIndexBuffer(108 + 1); // why one more???

			lightShader.createFromFiles("Resources/light.vert", "Resources/light.frag");
			lightVertexArray.setVertexBuffer({ 3, 3, 2 }, light.mesh().getVertices().size(), light.mesh().getVertices().data());
			lightVertexArray.setIndexBuffer(light.mesh().getIndices().size(), light.mesh().getIndices().data());
		}

		void update(float deltaSeconds)
		{
			camera.setAspectRatio((float)graphics.window.getWidth() / graphics.window.getHeight());

			if (input.isKeyDown(SDLK_p)) {
				camera.usePerspective(!camera.usesPerspective());
			}

			float cameraSpeed = 10 * deltaSeconds;
			camera.transform().move(
				cameraSpeed * (input.isKeyDown(SDLK_d) - input.isKeyDown(SDLK_a)),
				cameraSpeed * (input.isKeyDown(SDLK_r) - input.isKeyDown(SDLK_f)),
				cameraSpeed * (input.isKeyDown(SDLK_s) - input.isKeyDown(SDLK_w))
			);

			float rollSensitivity = 90 * deltaSeconds;
			camera.transform().roll(rollSensitivity * (input.isKeyDown(SDLK_e) - input.isKeyDown(SDLK_q)));

			float sensitivity = 0.05f;
			camera.transform().yaw(sensitivity * input.getMouseDeltaX());
			camera.transform().pitch(sensitivity * input.getMouseDeltaY());

			camera.setFieldOfView(camera.getFieldOfView() - input.getMouseWheel() * 10);

			for (auto& shape : shapes) {
				shape.transform().rotate(deltaSeconds * 50.0f, { 1.0f, 0.3f, 0.5f });
			}

			light.transform().setPosition(0, 0, 0);
		}

		void draw()
		{
			graphics.clearScreen(0.12f, 0, 0.06f, true);

			//drawFont();
			drawScene();
			drawInfo();
		}

		void drawFont()
		{
			fontShader.use();
			graphics.text.useFontTexture();
			fontVertexArray.draw(GL_TRIANGLE_STRIP);
		}

		void drawScene()
		{
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);

			glm::mat4 view = camera.getViewMatrix();
			glm::mat4 projection = camera.getProjectionMatrix();

			shapeShader.use();
			shapeShader.setUniform("view", view);
			shapeShader.setUniform("projection", projection);
			shapeShader.setUniform("mix", (sin(getGameTimeInSeconds()) + 1) * 0.5f);
			shapeShader.setUniform("lightColor", glm::vec3(1, 1, 1));

			glm::vec3 p = light.transform().getPosition().toVec3();
			shapeShader.setUniform("lightPosition", glm::vec3(view * glm::vec4(p.x, p.y, p.z, 1)));

			texture1.use(0);
			texture2.use(1);

			for (auto& shape : shapes) {
				glm::mat4 model = shape.transform().getModelMatrix();
				shapeShader.setUniform("model", model);
				auto vertices = shape.mesh().getVertices();
				auto indices = shape.mesh().getIndices();
				shapeVertexArray.updateVertexBuffer(vertices.size(), vertices.data());
				shapeVertexArray.updateIndexBuffer(indices.size(), indices.data());
				shapeVertexArray.draw(GL_TRIANGLES);
			}

			glm::mat4 model = light.transform().getModelMatrix();

			lightShader.use();
			lightShader.setUniform("model", model);
			lightShader.setUniform("view", view);
			lightShader.setUniform("projection", projection);

			lightVertexArray.draw(GL_TRIANGLES);
		}

		void drawInfo()
		{
			Vector3 position = camera.transform().getPosition();
			glm::ivec3 o = camera.transform().getEulerAngles();

			StringStream stream;
			stream << (int)round(graphics.getFps()) << "fps" << std::endl;
			stream << "x: " << position.x << " y: " << position.y << " z: " << position.z << std::endl;
			stream << "yaw: " << o.y << u8"° pitch: " << o.x << u8"° roll: " << o.z << u8"°";

			graphics.text(0, 0) << stream;
		}
	};
}
