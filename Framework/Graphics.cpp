#include "Graphics.h"
#include "Error.h"

namespace Framework
{
	void Graphics::clearScreen(float r, float g, float b, bool depth)
	{
		glCheck(glClearColor(r, g, b, 1.0f));
		glCheck(glClear(GL_COLOR_BUFFER_BIT | (depth ? GL_DEPTH_BUFFER_BIT : 0)));
	}

	float Graphics::getDeltaSeconds()
	{
		return (float)(frameLengths_[frameLengthsIndex_] / NANOSECONDS_PER_SECOND_);
	}

	float Graphics::getTotalSeconds()
	{
		return (float)(nanosecondsSinceStart_ / NANOSECONDS_PER_SECOND_);
	}

	float Graphics::getFps()
	{
		return (float)(numberOfFrameLengths_ * NANOSECONDS_PER_SECOND_ / frameLengthsTotal_);
	}

	bool Graphics::initialize_()
	{
		if (!window.activateOpenGL_()) {
			return false;
		}

		text.initialize_(TEXT_VS_, TEXT_FS_, window.getWidth(), window.getHeight());

		for (int i = 0; i < FPS_BUFFER_SIZE_; i++) {
			frameLengths_[i] = 0;
		}

		frameTimePoint_ = std::chrono::high_resolution_clock::now();

		return true;
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
