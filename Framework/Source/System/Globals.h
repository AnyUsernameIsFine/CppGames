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

#include "Utility.h"
#define console(output)		std::cout << output << std::endl
#define error(error)		Utility::outputError(error, __FILE__, __LINE__)
#define checkGL(expr)		{ expr; Utility::outputOpenGLError(nullptr,__FILE__, __LINE__, #expr); }
#define checkSDL(expr)		{ expr; Utility::outputSDLError(nullptr, __FILE__, __LINE__, #expr); }
#define checkGLValue(expr)	Utility::outputOpenGLError(expr, __FILE__, __LINE__, #expr)
#define checkSDLValue(expr)	Utility::outputSDLError(expr, __FILE__, __LINE__, #expr)
