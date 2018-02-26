#include "Graphics.hpp"
#include "System\Error.hpp"

namespace Framework
{
	void Graphics::clearScreen(float r, float g, float b, bool depth) const
	{
		glCheck(glClearColor(r, g, b, 1));
		glCheck(glClear(GL_COLOR_BUFFER_BIT | (depth ? GL_DEPTH_BUFFER_BIT : 0)));
	}

	float Graphics::getFps() const
	{
		return numberOfFrameLengths / frameLengthsTotal;
	}

	int Graphics::openWindow()
	{
		return window.open();
	}

	void Graphics::closeWindow()
	{
		window.close();
	}

	int Graphics::initialize()
	{
		if (window.activateOpenGL() != 0) {
			return 1;
		}

		text.initialize(window.getWidth(), window.getHeight());

		for (int i = 0; i < FPS_BUFFER_SIZE; i++) {
			frameLengths[i] = 0;
		}

		frameTimePoint.setToNow();

		return 0;
	}

	void Graphics::update()
	{
		window.update();

		TimePoint now;
		float frameLength = now.differenceInSeconds(frameTimePoint);
		frameTimePoint = now;

		frameLengthsIndex = (frameLengthsIndex + 1) % FPS_BUFFER_SIZE;
		frameLengthsTotal -= frameLengths[frameLengthsIndex];
		frameLengths[frameLengthsIndex] = frameLength;
		frameLengthsTotal += frameLength;

		if (numberOfFrameLengths < FPS_BUFFER_SIZE) {
			numberOfFrameLengths++;
		}
	}

	void Graphics::onWindowResize(SDL_Event event)
	{
		if (event.type == SDL_WINDOWEVENT) {
			int width = event.window.data1;
			int height = event.window.data2;

			switch (event.window.event) {
			case SDL_WINDOWEVENT_RESIZED:
				window.onResize(width, height);
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				text.onWindowResize(width, height);
			}
		}
	}
}
