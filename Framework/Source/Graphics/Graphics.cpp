#include "Graphics.hpp"
#include "System\Error.hpp"

#include <thread>

namespace Framework
{
	void Graphics::clearScreen(float r, float g, float b, bool depth) const
	{
		glCheck(glClearColor(r, g, b, 1.0f));
		glCheck(glClear(GL_COLOR_BUFFER_BIT | (depth ? GL_DEPTH_BUFFER_BIT : 0)));
	}

	float Graphics::getDeltaSeconds() const
	{
		return (float)(frameLengths_[frameLengthsIndex_] / NANOSECONDS_PER_SECOND_);
	}

	float Graphics::getTotalSeconds() const
	{
		return (float)(nanosecondsSinceStart_ / NANOSECONDS_PER_SECOND_);
	}

	float Graphics::getFps() const
	{
		return (float)(numberOfFrameLengths_ * NANOSECONDS_PER_SECOND_ / frameLengthsTotal_);
	}

	int Graphics::openWindow_()
	{
		return window.open_();
	}

	void Graphics::closeWindow_()
	{
		window.close_();
	}

	int Graphics::initialize_()
	{
		if (window.activateOpenGL_() != 0) {
			return 1;
		}

		text.initialize_(window.getWidth(), window.getHeight());

		for (int i = 0; i < FPS_BUFFER_SIZE_; i++) {
			frameLengths_[i] = 0;
		}

		frameTimePoint_ = std::chrono::high_resolution_clock::now();

		return 0;
	}

	void Graphics::update_()
	{
		window.update_();

		auto now = std::chrono::high_resolution_clock::now();
		auto frameLength = std::chrono::duration_cast<std::chrono::nanoseconds>(now - frameTimePoint_).count();
		frameTimePoint_ = now;

		nanosecondsSinceStart_ += frameLength;

		frameLengthsIndex_ = (frameLengthsIndex_ + 1) % FPS_BUFFER_SIZE_;
		frameLengthsTotal_ -= frameLengths_[frameLengthsIndex_];
		frameLengths_[frameLengthsIndex_] = frameLength;
		frameLengthsTotal_ += frameLength;

		if (numberOfFrameLengths_ < FPS_BUFFER_SIZE_) {
			numberOfFrameLengths_++;
		}
	}
}