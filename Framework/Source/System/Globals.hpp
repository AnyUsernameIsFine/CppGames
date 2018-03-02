#pragma once

#define GLM_FORCE_INLINE

typedef char				sByte;
typedef unsigned char		byte;
typedef unsigned short		uShort;
typedef unsigned int		uInt;
typedef unsigned long		uLong;
typedef long long			longLong;
typedef unsigned long long	uLongLong;

#include <cstdint>
typedef int8_t		int8;
typedef uint8_t		uInt8;
typedef int16_t		int16;
typedef uint16_t	uInt16;
typedef int32_t		int32;
typedef uint32_t	uInt32;
typedef int64_t		int64;
typedef uint64_t	uInt64;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <GL\glew.h>
#include <SDL_error.h>

#include <iostream>

namespace Framework
{
#define console(output)		std::cout << output << std::endl
#define error(error)		Detail::myError(error, __FILE__, __LINE__)
#define glCheck(expr)		{ expr; Detail::myGlCheck(__FILE__, __LINE__, #expr); }
#define sdlCheck(expr)		{ expr; Detail::mySdlCheck(__FILE__, __LINE__, #expr); }
#define glCheckValue(expr)	Detail::myGlCheckValue(expr, __FILE__, __LINE__, #expr)
#define sdlCheckValue(expr)	Detail::mySdlCheckValue(expr, __FILE__, __LINE__, #expr)

	namespace Detail
	{
		const string GL_ERRORS[] = {
			"GL_INVALID_ENUM",
			"GL_INVALID_VALUE",
			"GL_INVALID_OPERATION",
			"GL_STACK_OVERFLOW",
			"GL_STACK_UNDERFLOW",
			"GL_OUT_OF_MEMORY",
			"GL_INVALID_FRAMEBUFFER_OPERATION",
		};

		inline void myError(const string& error, const string& file, int line, const string& expression = string())
		{
			string string(file);
			std::cerr << "\"" << error << "\" error in " << string.substr(string.find_last_of("\\/") + 1) << ":" << line << ".";
			if (!expression.empty()) {
				std::cerr << " (" << expression << ")";
			}
			std::cerr << std::endl;
		}

		inline void myGlCheck(const string& file, int line, const string& expression)
		{
			GLenum e;
			while ((e = glGetError()) != GL_NO_ERROR) {
				myError(GL_ERRORS[e - GL_INVALID_ENUM], file, line, expression);
			}
		}

		inline void mySdlCheck(const string& file, int line, const string& expression)
		{
			const char* e;
			while (std::strlen(e = SDL_GetError()) > 0) {
				myError(e, file, line, expression);
				SDL_ClearError();
			}
		}

		template<typename T>
		inline T myGlCheckValue(T value, const string& file, int line, const string& expression)
		{
			myGlCheck(file, line, expression);
			return value;
		}

		template<typename T>
		inline T mySdlCheckValue(T value, const string& file, int line, const string& expression)
		{
			mySdlCheck(file, line, expression);
			return value;
		}
	}
}
