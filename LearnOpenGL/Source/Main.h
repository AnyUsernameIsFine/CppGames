#pragma once

#include <ctime>

#include <glm\gtc\random.hpp>
#include "Globals.h"

namespace LearnOpenGL
{
	class Game : public GLEngine::Game
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
			//graphics.window.setSize(1920, 1080);
			//graphics.window.enableFullscreen();
			//graphics.window.enableResizing();
			//graphics.window.enableCursor();
			//graphics.window.enableVSync(false);
			graphics.window.enableAntiAliasing();
		}

		void initialize()
		{
			Random::setRandSeed(static_cast<UInt>(getGameTimeInSeconds()));

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

			struct Vertex { float x, y, u, v; };
			std::vector<Vertex> fontVertices = {
				{ -1, -1,	0, 1 },
				{  1, -1,	1, 1 },
				{ -1,  1,	0, 0 },
				{  1,  1,	1, 0 },
			};

			fontShader.createFromFiles("Resources/font.vert", "Resources/font.frag");
			fontShader.use();
			fontShader.setUniform("fontTexture", 0);
			fontVertexArray.setVertexBuffer({ 2, 2 }, fontVertices);

			texture1.createFromFile("Resources/journey.jpg");
			texture2.createFromFile("Resources/flow.jpg");

			shapeShader.createFromFiles("Resources/shape.vert", "Resources/shape.frag");
			shapeShader.use();
			shapeShader.setUniform("texture1", 0);
			shapeShader.setUniform("texture2", 1);
			shapeVertexArray.setVertexBuffer({ 3, 3, 2 }, 60);
			shapeVertexArray.setIndexBuffer(108 + 1); // why one more???

			lightShader.createFromFiles("Resources/light.vert", "Resources/light.frag");
			lightVertexArray.setVertexBuffer({ 3, 3, 2 }, light.mesh().getVertices());
			lightVertexArray.setIndexBuffer(light.mesh().getIndices());
		}

		void update(float deltaSeconds)
		{
			camera.setAspectRatio(graphics.window.getWidth(), graphics.window.getHeight());

			if (input.keyboard.key(SDLK_p)) {
				camera.usePerspective(!camera.usesPerspective());
			}

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
				shapeVertexArray.updateVertexBuffer(shape.mesh().getVertices());
				shapeVertexArray.updateIndexBuffer(shape.mesh().getIndices());
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
			stream << static_cast<int>(round(graphics.getFps())) << "fps" << std::endl;
			stream << "x: " << position.x << " y: " << position.y << " z: " << position.z << std::endl;
			stream << "yaw: " << o.y << u8"° pitch: " << o.x << u8"° roll: " << o.z << u8"°";

			graphics.text.draw(0, 0) << stream;
		}
	};
}
