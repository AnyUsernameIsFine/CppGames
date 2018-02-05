#pragma once

#include "Font.h"
#include "GLProgram.h"
#include "VertexBufferObject.h"

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
		void draw(float x, float y, const std::string& text);
		void draw(float x, float y, const std::wstring& text);

	private:
		friend class Graphics;

		GLProgram* program_;
		int windowWidth_;
		int windowHeight_;
		GLuint vao_;
		VertexBufferObject* vbo_;

		std::map<std::string, Font> fonts_;
		Font* font_;

		void initialize_(const std::string& vsFilename, const std::string& fsFilename, int windowWidth, int windowHeight);
	};
}
