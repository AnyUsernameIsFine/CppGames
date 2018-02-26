#include "TextStream.hpp"
#include "Text.hpp"

namespace Framework
{
	TextStream::TextStream(Text* parent, float x, float y)
	{
		this->parent = parent;
		this->x = x;
		this->y = y;
	}

	TextStream::~TextStream()
	{
		if (this->parent) {
			this->parent->draw(x, y, text);
		}
	}

	TextStream& TextStream::operator<<(const TextStream& stream)
	{
		text += stream.text;
		return *this;
	}

	TextStream& TextStream::operator<<(const string& text)
	{
		try {
#if _MSC_VER >= 1900
			auto p = utf32converter.from_bytes(text);
			this->text += reinterpret_cast<const char32_t*>(p.data());
#else
			this->text += utf32converter.from_bytes(text);
#endif
		}
		catch (std::range_error) {
			error("String conversion failed");
		}
		return *this;
	}

	TextStream& TextStream::operator<<(const std::u16string& text)
	{
#if _MSC_VER >= 1900
		auto p = reinterpret_cast<const int16_t*>(text.data());
		return *this << utf16converter.to_bytes(p, p + text.size());
#else
		return *this << utf16converter.to_bytes(text);
#endif
	}

	TextStream& TextStream::operator<<(const std::u32string& text)
	{
		this->text += text;
		return *this;
	}

	TextStream& TextStream::operator<<(const char* text)
	{
		return *this << (string)text;
	}

	TextStream& TextStream::operator<<(const char16_t* text)
	{
		return *this << (std::u16string)text;
	}

	TextStream& TextStream::operator<<(const char32_t* text)
	{
		return *this << (std::u32string)text;
	}

	TextStream& TextStream::operator<<(char character)
	{
		text += character;
		return *this;
	}

	TextStream& TextStream::operator<<(char16_t character)
	{
		text += character;
		return *this;
	}

	TextStream& TextStream::operator<<(char32_t character)
	{
		text += character;
		return *this;
	}

	TextStream::Utf32Converter TextStream::utf32converter;
	TextStream::Utf16Converter TextStream::utf16converter;
}