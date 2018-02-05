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
		void setFont(const std::string& filename, unsigned int size);
		void draw(float x, float y, const std::string& text);
		void draw(float x, float y, const std::wstring& text);

	private:
		friend class Graphics;

		GLProgram* program_;
		unsigned int windowWidth_;
		unsigned int windowHeight_;
		GLuint vao_;
		VertexBufferObject* vbo_;

		std::map<std::string, Font> fonts_;
		Font* font_;

		void initialize_(const std::string& vsFilename, const std::string& fsFilename, unsigned int windowWidth, unsigned int windowHeight);
	};
}
