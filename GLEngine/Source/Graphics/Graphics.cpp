#include "Graphics.h"

namespace GLEngine
{
	void Graphics::clearScreen(float r, float g, float b, bool depth) const
	{
		checkGL(glClearColor(r, g, b, 1));
		checkGL(glClear(GL_COLOR_BUFFER_BIT | (depth ? GL_DEPTH_BUFFER_BIT : 0)));
	}

	float Graphics::getFps() const
	{
		return std::fmaxf(0, numFrameLengths / frameLengthsTotal);
	}

	bool Graphics::createWindow()
	{
		return window.create();
	}

	bool Graphics::initialize()
	{
		if (!window.activateOpenGL()) {
			return false;
		}

		text.initialize(window.getWidth(), window.getHeight());

		for (int i = 0; i < FPS_BUFFER_SIZE; i++) {
			frameLengths[i] = 0.0f;
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

		if (numFrameLengths < FPS_BUFFER_SIZE) {
			numFrameLengths++;
		}
	}

	void Graphics::windowEventHandler(const SDL_WindowEvent& event)
	{
		int width = event.data1;
		int height = event.data2;

		switch (event.event) {
			case SDL_WINDOWEVENT_RESIZED:
				window.resizedEventHandler(width, height);
				break;

			case SDL_WINDOWEVENT_SIZE_CHANGED:
				text.windowResizedEventHandler(width, height);
				break;
		}
	}
}
