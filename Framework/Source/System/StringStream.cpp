#include "StringStream.hpp"
#include "Error.hpp"

namespace Framework
{
	StringStream::StringStream(void callback(const StringStream&, void*), const void* data, uInt length)
	{
		this->callback = callback;
		this->dataLength = length;

		this->data = new byte[length];
		memcpy(this->data, data, length);
	}

	StringStream::~StringStream()
	{
		if (this->callback) {
			this->callback(*this, data);

			delete[] this->data;
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