#pragma once

#include "Transform.h"

#include <Error.h>

namespace Framework
{
	class Camera
	{
	public:
		Transform transform;

		const bool isPerspective() const;
		const float getFieldOfView() const;
		void setPerspective(bool perspective = true);
		void setAspectRatio(float ratio);
		void setFieldOfView(float fov);
		void setSize(int size);
		const glm::mat4 getViewMatrix() const;
		const glm::mat4 getProjectionMatrix() const;

	private:
		bool perspective_ = true;
		float ratio_ = 1.0f;
		float fov_ = 60.0f;
		float near_ = 0.1f;
		float far_ = 100.0f;
		int size_ = 10;
	};
}
