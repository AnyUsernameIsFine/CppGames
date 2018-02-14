#pragma once

#include "Font.hpp"
#include "OpenGL\ShaderProgram.hpp"
#include "OpenGL\VertexBufferObject.hpp"

#include <GL\glew.h>

#include <string>
#include <map>

namespace Framework
{
	class Text
	{
	public:
		void setColor(float r = 0, float g = 0, float b = 0, float a = 1);
		void setFont(const std::string& filename, int size);
		void draw(float x, float y, const std::string& text) const;
		void draw(float x, float y, const std::wstring& text) const;

	private:
		friend class Graphics;

		ShaderProgram* program_;
		int windowWidth_;
		int windowHeight_;
		GLuint vao_;
		VertexBufferObject* vbo_;

		std::map<std::string, Font> fonts_;
		Font* font_;

		void initialize_(int windowWidth, int windowHeight);
	};
}
