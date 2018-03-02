#include "StringStream.hpp"

namespace Framework
{
	StringStream::StringStream(void callback(const StringStream&, void*), const void* data, uInt dataLength)
	{
		this->callback = callback;
		this->callbackData = new byte[dataLength];
		memcpy(this->callbackData, data, dataLength);
	}

	StringStream::~StringStream()
	{
		if (callback) {
			callback(*this, callbackData);
			delete[] callbackData;
		}
	}

	string StringStream::getString() const
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

	StringStream::Utf8Converter StringStream::utf8Converter;
}
