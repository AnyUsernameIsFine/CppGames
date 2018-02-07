#pragma once

#include <GL\glew.h>
#include <SDL_error.h>

#include <string>
#include <iostream>

#define output(output) std::cout << output << std::endl
#define error(error) Framework::private_::error_(error, __FILE__, __LINE__)
#define glCheckV(expr) Framework::private_::glCheckV_(expr, __FILE__, __LINE__, #expr)
#define glCheck(expr) { expr; Framework::private_::glCheck_(__FILE__, __LINE__, #expr); }
#define sdlCheckV(expr) Framework::private_::sdlCheckV_(expr, __FILE__, __LINE__, #expr)
#define sdlCheck(expr) { expr; Framework::private_::sdlCheck_(__FILE__, __LINE__, #expr); }

namespace
{
	const std::string GL_ERRORS[] = {
		"GL_INVALID_ENUM",
		"GL_INVALID_VALUE",
		"GL_INVALID_OPERATION",
		"GL_STACK_OVERFLOW",
		"GL_STACK_UNDERFLOW",
		"GL_OUT_OF_MEMORY",
		"GL_INVALID_FRAMEBUFFER_OPERATION",
	};
}

namespace Framework
{
	namespace private_
	{
		inline void error_(const std::string& error, const std::string& file, int line, const std::string& expression = std::string())
		{
			std::string string(file);
			std::cerr << "\"" << error << "\" error in " << string.substr(string.find_last_of("\\/") + 1) << ":" << line << ".";
			if (!expression.empty()) {
				std::cerr << " (" << expression << ")";
			}
			std::cerr << std::endl;
		}

		template<typename T>
		inline T glCheckV_(T value, const std::string& file, int line, const std::string& expression)
		{
			glCheck_(file, line, expression);
			return value;
		}

		inline void glCheck_(const std::string& file, int line, const std::string& expression)
		{
			GLenum e;
			while ((e = glGetError()) != GL_NO_ERROR) {
				error_(GL_ERRORS[e - GL_INVALID_ENUM], file, line, expression);
			}
		}

		template<typename T>
		inline T sdlCheckV_(T value, const std::string& file, int line, const std::string& expression)
		{
			sdlCheck_(file, line, expression);
			return value;
		}

		inline void sdlCheck_(const std::string& file, int line, const std::string& expression)
		{
			const char* e;
			while (std::strlen(e = SDL_GetError()) > 0) {
				error_(e, file, line, expression);
				SDL_ClearError();
			}
		}
	}
}
