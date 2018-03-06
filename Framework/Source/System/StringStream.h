#pragma once

#include <sstream>
#include <locale>
#include <codecvt>

#include "System\Globals.h"

namespace Framework
{
	class StringStream
	{
	public:
		StringStream() {}
		StringStream(void callback(const StringStream&, void*), const void* data, int dataLength);
		~StringStream();
		StringStream& operator<<(const StringStream& stream);
		StringStream& operator<<(std::ostream& function(std::ostream&));
		template<typename T> StringStream& operator<<(const T& value);
		std::string getString() const;
		std::u32string getUtf8String() const;

	private:
#if _MSC_VER >= 1900
		using Utf8Converter = std::wstring_convert<std::codecvt_utf8<Int32>, Int32>;
#else
		using Utf8Converter = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>;
#endif

		std::ostringstream oStringStream;
		void(*callback)(const StringStream&, void*);
		void* callbackData;

		static Utf8Converter utf8Converter;
	};

	template<typename T>
	StringStream& StringStream::operator<<(const T& value)
	{
		oStringStream << value;
		return *this;
	}
}
