#include "StringStream.h"

namespace Framework
{
	StringStream::StringStream(void callback(const StringStream&, void*), const void* data, int dataLength)
	{
		this->callback = callback;
		this->callbackData = new Byte[dataLength];
		memcpy(this->callbackData, data, dataLength);
	}

	StringStream::~StringStream()
	{
		if (callback) {
			callback(*this, callbackData);
			delete[] callbackData;
		}
	}

	StringStream& StringStream::operator<<(const StringStream& stream)
	{
		oStringStream << stream.oStringStream.str();
		return *this;
	}

	StringStream& StringStream::operator<<(std::ostream& function(std::ostream&))
	{
		function(oStringStream);
		return *this;
	}

	std::string StringStream::getString() const
	{
		return oStringStream.str();
	}

	std::u32string StringStream::getUtf8String() const
	{
		try {
#if _MSC_VER >= 1900
			auto p = utf8Converter.from_bytes(oStringStream.str());
			return reinterpret_cast<const char32_t*>(p.data());
#else
			return utf8Converter.from_bytes(oStringStream.str());
#endif
		}
		catch (std::range_error) {
			error("String conversion failed");
			return U"";
		}
	}

	StringStream::Utf8Converter StringStream::utf8Converter;
}
