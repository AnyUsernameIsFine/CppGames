#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <glad\glad.h>
#include <SDL_error.h>

#include "Globals.h"

#define console(output)		std::cout << output << std::endl
#define error(error)		Utility::outputError(error, __FILE__, __LINE__)
#define checkGL(expr)		{ expr; Utility::outputOpenGLError(nullptr,__FILE__, __LINE__, #expr); }
#define checkSDL(expr)		{ expr; Utility::outputSDLError(nullptr, __FILE__, __LINE__, #expr); }
#define checkGLValue(expr)	Utility::outputOpenGLError(expr, __FILE__, __LINE__, #expr)
#define checkSDLValue(expr)	Utility::outputSDLError(expr, __FILE__, __LINE__, #expr)

namespace GLEngine
{
	namespace Utility
	{
		template<void constructor()>
		struct StaticConstructor
		{
			struct Constructor { Constructor() { constructor(); } };

			static Constructor c;
		};

		template<void constructor()>
		typename StaticConstructor<constructor>::Constructor StaticConstructor<constructor>::c;

		inline void outputError(const std::string& error, const std::string& file, int line, const std::string& expression = "")
		{
			static std::vector<std::string> uniqueErrors;

			std::string hash = error + file + std::to_string(line);
			if (std::find(uniqueErrors.begin(), uniqueErrors.end(), hash) == uniqueErrors.end()) {
				std::cerr << "\"" << error << "\" error in " << file.substr(file.find_last_of("\\/") + 1) << ":" << line << ".";

				if (!expression.empty()) {
					std::cerr << " (" << expression << ")";
				}

				std::cerr << std::endl;

				uniqueErrors.emplace_back(hash);
			}
		}

		template<typename T>
		inline T outputOpenGLError(T value, const std::string& file, int line, const std::string& expression)
		{
			static const std::string OPEN_GL_ERRORS[] = {
				"GL_INVALID_ENUM",
				"GL_INVALID_VALUE",
				"GL_INVALID_OPERATION",
				"GL_STACK_OVERFLOW",
				"GL_STACK_UNDERFLOW",
				"GL_OUT_OF_MEMORY",
				"GL_INVALID_FRAMEBUFFER_OPERATION",
				"GL_CONTEXT_LOST"
			};

			GLenum error;
			while ((error = glGetError()) != GL_NO_ERROR) {
				outputError(OPEN_GL_ERRORS[error - GL_INVALID_ENUM], file, line, expression);
			}

			return value;
		}

		template<typename T>
		inline T outputSDLError(T value, const std::string& file, int line, const std::string& expression)
		{
			const char* error;
			while (std::strlen(error = SDL_GetError()) > 0) {
				outputError(error, file, line, expression);
				SDL_ClearError();
			}

			return value;
		}
	}
}
