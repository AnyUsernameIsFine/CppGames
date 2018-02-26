#pragma once

#include "System\Globals.hpp"

#include <locale>
#include <codecvt>

namespace Framework
{
	class Text;

	class TextStream
	{
#if _MSC_VER >= 1900
		typedef std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> Utf32Converter;
		typedef std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> Utf16Converter;
#else
		typedef std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> Utf32Converter;
		typedef std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> Utf16Converter;
#endif

	public:
		TextStream() {}
		~TextStream();
		TextStream& operator<<(const TextStream& stream);
		TextStream& operator<<(const std::string& text);
		TextStream& operator<<(const std::u16string& text);
		TextStream& operator<<(const std::u32string& text);
		TextStream& operator<<(const char* text);
		TextStream& operator<<(const char16_t* text);
		TextStream& operator<<(const char32_t* text);
		TextStream& operator<<(char character);
		TextStream& operator<<(char16_t character);
		TextStream& operator<<(char32_t character);

		template<typename T> TextStream& operator<<(T text)
		{
			return *this << std::to_string(text);
		}

	private:
		Text* parent;
		float x, y;
		std::u32string text;

		TextStream(Text* parent, float x, float y);

		friend class Text;

		static Utf32Converter utf32converter;
		static Utf16Converter utf16converter;
	};
}