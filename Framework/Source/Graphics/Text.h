#pragma once

#include "Font.h"
#include "System\StringStream.h"
#include "OpenGL\Shader.h"
#include "OpenGL\VertexArrayObject.h"

namespace Framework
{
	class Text
	{
	public:
		void loadFont(const string& filename);
		void loadFont(const string& filename, const string& family);
		void setFontFamily(const string& family);
		void setFontSize(int size);
		void setFont(const string& family, int size);
		void setColor(float r, float g, float b, float a = 1);
		int getFontHeight() const;
		void useFontTexture() const;
		StringStream operator()(float x, float y);

	private:
		typedef std::unordered_map<string, std::shared_ptr<Font>> Fonts;

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

		int windowWidth;
		int windowHeight;
		vector<GlyphQuad> vertices = vector<GlyphQuad>(MAX_STRING_LENGTH);
		Shader shader;
		VertexArrayObject vao;
		VertexBufferObject vbo;
		bool windowHasResized = false;
		std::unordered_map<string, std::shared_ptr<Font>> fonts;
		std::shared_ptr<Font> font = nullptr;
		int size = 30;

		Text() {}
		std::shared_ptr<Font> findFont(const string& family) const;
		void initialize(int windowWidth, int windowHeight);
		void windowResizedEventHandler(int width, int height);
		void draw(float x, float y, const std::u32string& text);
		void applyWindowSize();

		static void drawFromStreamCallback(const StringStream& stream, void* data);

		friend class Graphics;
	};
}
