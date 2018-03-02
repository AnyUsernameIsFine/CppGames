#include "Graphics.hpp"

namespace Framework
{
	void Graphics::clearScreen(float r, float g, float b, bool depth) const
	{
		glCheck(glClearColor(r, g, b, 1));
		glCheck(glClear(GL_COLOR_BUFFER_BIT | (depth ? GL_DEPTH_BUFFER_BIT : 0)));
	}

	float Graphics::getFps() const
	{
		return std::fmaxf(0, numberOfFrameLengths / frameLengthsTotal);
	}

	bool Graphics::openWindow()
	{
		return window.open();
	}

	void Graphics::closeWindow()
	{
		window.close();
	}

	bool Graphics::initialize()
	{
		if (!window.activateOpenGL()) {
			return false;
		}

		text.initialize(window.getWidth(), window.getHeight());

		for (int i = 0; i < FPS_BUFFER_SIZE; i++) {
			frameLengths[i] = 0;
		}

		frameTimePoint.setToNow();

		return true;
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

	void Graphics::windowEventHandler(SDL_WindowEvent event)
	{
		int width = event.data1;
		int height = event.data2;

		switch (event.event) {
		case SDL_WINDOWEVENT_RESIZED:
			window.resizedEventHandler(width, height);
			break;
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			text.windowResizedEventHandler(width, height);
		}
	}
}
