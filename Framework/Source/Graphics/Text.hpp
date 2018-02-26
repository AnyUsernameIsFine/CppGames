#pragma once

#include "Font.hpp"
#include "TextStream.hpp"
#include "OpenGL\ShaderProgram.hpp"
#include "OpenGL\VertexBufferObject.hpp"
#include "System\Error.hpp"

namespace Framework
{
	class Text
	{
	public:
		void loadFont(const string& filename);
		void setFontFamily(const string& family);
		void setFontSize(int size);
		void setFont(const string& family, int size);
		void setColor(float r, float g, float b, float a = 1);
		int getFontHeight() const;
		TextStream draw(float x, float y);
		void draw(float x, float y, const std::u32string& text);

	private:
		struct GlyphQuad
		{
			GLfloat data[24];
		};

		static const int MAX_STRING_LENGTH = 65536;

		ShaderProgram* program;
		GLuint vao;
		GlyphQuad* vertices;
		VertexBufferObject* vbo;
		bool windowHasResized = false;
		int windowWidth;
		int windowHeight;
		vector<std::shared_ptr<Font>> fonts;
		Font* font = nullptr;
		int size = 30;

		Text() {}
		void initialize(int windowWidth, int windowHeight);
		void onWindowResize(int width, int height);
		Font* findFont(const FT_String* family) const;
		void applyWindowSize();

		friend class Graphics;
	};
}
