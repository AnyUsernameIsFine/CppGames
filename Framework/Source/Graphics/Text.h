#pragma once

#include "Font.h"
#include "OpenGL\Shader.h"
#include "OpenGL\VertexArray.h"
#include "System\StringStream.h"

namespace Framework
{
	class Text
	{
	public:
		void loadFont(const std::string& filename);
		void loadFont(const std::string& filename, const std::string& family);
		void setFontFamily(const std::string& family);
		void setFontSize(int size);
		void setFont(const std::string& family, int size);
		void setColor(float r, float g, float b, float a = 1.0f);
		int getFontHeight() const;
		void useFontTexture() const;
		StringStream draw(int x, int y);

	private:
		using Fonts = std::unordered_map<std::string, std::shared_ptr<Font>>;

		struct DrawConfiguration
		{
			Text* text;
			int x, y;
		};

		static constexpr int MAX_STRING_LENGTH = 1 << 10;

		Text() {}
		std::shared_ptr<Font> findFont(const std::string& family) const;
		void initialize(int windowWidth, int windowHeight);
		void windowResizedEventHandler(int width, int height);
		void draw(int x, int y, const std::u32string& text);
		void applyWindowSize();

		static void drawFromStreamCallback(const StringStream& stream, void* data);

		int windowWidth;
		int windowHeight;
		std::vector<Font::GlyphQuad> glyphQuads = std::vector<Font::GlyphQuad>(MAX_STRING_LENGTH);
		Shader shader;
		VertexArray vertexArray;
		bool windowHasResized = false;
		std::unordered_map<std::string, std::shared_ptr<Font>> fonts;
		std::shared_ptr<Font> font = nullptr;
		int size = 0;

		friend class Graphics;
	};
}
