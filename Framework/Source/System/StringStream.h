#pragma once

#include "System\Globals.h"

#include <sstream>
#include <locale>
#include <codecvt>

namespace Framework
{
	class StringStream
	{
	public:
		StringStream() {}
		StringStream(void callback(const StringStream&, void*), const void* data, uInt dataLength);
		~StringStream();
		string getString() const;
		std::u32string getUtf8String() const;
		StringStream& operator<<(const StringStream& stream);
		StringStream& operator<<(std::ostream& function(std::ostream&));

		template<typename T>
		StringStream& operator<<(const T& value)
		{
			oStringStream << value;
			return *this;
		}

	private:
		std::ostringstream oStringStream;
		void(*callback)(const StringStream&, void*);
		void* callbackData;

#if _MSC_VER >= 1900
		typedef std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> Utf8Converter;
#else
		typedef std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> Utf8Converter;
#endif

		static Utf8Converter utf8Converter;
	};
}
