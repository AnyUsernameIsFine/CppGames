#pragma once

#include "Font.hpp"
#include "System\StringStream.hpp"
#include "OpenGL\ShaderProgram.hpp"
#include "OpenGL\VertexBufferObject.hpp"
#include "System\Error.hpp"

namespace Framework
{
	class Text
	{
	public:
		void loadFont(const string& filename, const string& family = string());
		void setFontFamily(const string& family);
		void setFontSize(int size);
		void setFont(const string& family, int size);
		void setColor(float r, float g, float b, float a = 1);
		int getFontHeight() const;
		GLuint getFontTextureId() const;
		StringStream operator()(float x, float y);

	private:
		struct GlyphQuad
		{
			GLfloat data[24];
		};

		struct DrawConfiguration
		{
			Text* text;
			float x, y;
		};

		static const int MAX_STRING_LENGTH = 65536;

		ShaderProgram* program;
		GLuint vao;
		GlyphQuad* vertices;
		VertexBufferObject* vbo;
		bool windowHasResized = false;
		int windowWidth;
		int windowHeight;
		std::unordered_map<string, std::shared_ptr<Font>> fonts;
		Font* font = nullptr;
		int size = 30;

		Text() {}
		void initialize(int windowWidth, int windowHeight);
		void onWindowResize(int width, int height);
		Font* findFont(const string& family) const;
		void draw(float x, float y, const std::u32string& text);
		void applyWindowSize();

		static void drawFromStream(const StringStream& stream, void* params);

		friend class Graphics;
	};
}
