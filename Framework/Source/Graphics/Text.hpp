#pragma once

#include "Font.hpp"
#include "OpenGL\ShaderProgram.hpp"
#include "OpenGL\VertexBufferObject.hpp"

namespace Framework
{
	class Text
	{
	public:
		~Text();
		void loadFont(const std::string& filename);
		void setFontFamily(const std::string& family);
		void setFontSize(int size);
		void setFont(const std::string& family, int size);
		void setColor(float r = 0, float g = 0, float b = 0, float a = 1);
		int getFontHeight() const;
		void draw(float x, float y, const std::string& text) const;

	private:
		friend class Graphics;

		struct GlyphQuad
		{
			GLfloat data[24];
		};

		const int MAX_STRING_LENGTH_ = 1 << 16;

		ShaderProgram* program_;
		int windowWidth_;
		int windowHeight_;
		GLuint vao_;
		GlyphQuad* vertices_;
		VertexBufferObject* vbo_;

		std::vector<std::shared_ptr<Font>> fonts_;
		Font* font_ = nullptr;
		int size_ = 30;

		void initialize_(int windowWidth, int windowHeight);
		Font* findFont_(const FT_String* family) const;
	};
}
